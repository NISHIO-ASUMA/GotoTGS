//=========================================================
//
// インスタンシング適用モデル管理クラス [ instancemodelmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "instancemodelmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <fstream>
#include "json.hpp"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json; // json空間

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
int CInstanceModelManager::m_nNumAll = NULL; // 総数管理

//=========================================================
// コンストラクタ
//=========================================================
CInstanceModelManager::CInstanceModelManager()
{

}
//=========================================================
// デストラクタ
//=========================================================
CInstanceModelManager::~CInstanceModelManager()
{
	// 全破棄
	UnLoad();
}
//=========================================================
// 読み込み処理
//=========================================================
HRESULT CInstanceModelManager::Load(void)
{
	// jsonファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 全データ破棄処理
//=========================================================
void CInstanceModelManager::UnLoad(void)
{
	// 格納情報の破棄
	for (auto iter = m_aModelInstData.begin(); iter != m_aModelInstData.end(); iter++)
	{
		// メッシュの破棄
		if ((*iter).pMesh != nullptr)
		{
			(*iter).pMesh->Release();
			(*iter).pMesh = nullptr;
		}

		// マテリアルポインタの破棄
		if ((*iter).pBuffMat != nullptr)
		{
			(*iter).pBuffMat->Release();
			(*iter).pBuffMat = nullptr;
		}

		// テクスチャの破棄
		(*iter).pTexture.clear();

		// 動的ファイルパスの破棄
		if (!(*iter).FilePath.empty())
		{
			(*iter).FilePath.clear();
		}

		// マテリアルデータの破棄
		if ((*iter).dwNumMat != NULL)
		{
			(*iter).dwNumMat = 0;
		}

		// 頂点バッファの破棄
		if ((*iter).InstanceData.VtxBuffer != nullptr)
		{
			(*iter).InstanceData.VtxBuffer->Release();
			(*iter).InstanceData.VtxBuffer = nullptr;
		}

		// インデックスバッファの破棄
		if ((*iter).InstanceData.IndexBuffer != nullptr)
		{
			(*iter).InstanceData.IndexBuffer->Release();
			(*iter).InstanceData.IndexBuffer = nullptr;
		}

		// 動的確保したマテリアルのクリア
		(*iter).Materials.clear();
	}

	// 配列クリア
	m_aModelInstData.clear();
}
//=========================================================
// モデル登録
//=========================================================
int CInstanceModelManager::Register(const char* pModelName)
{
	// すでに登録済みならそのインデックスを返す
	for (int nCnt = 0; nCnt < static_cast<int>(m_aModelInstData.size()); nCnt++)
	{
		if (m_aModelInstData[nCnt].FilePath == pModelName)
			return nCnt;
	}

	// ないなら新規登録処理
	InstanceModelInfo NewModelInfo = {};
	NewModelInfo.FilePath = pModelName;

	//===============================================================
	// モデル読み込み
	//===============================================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice)
		return -1;

	// Xファイルを生成する
	HRESULT hr = D3DXLoadMeshFromX
	(
		pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&NewModelInfo.pBuffMat,
		NULL,
		&NewModelInfo.dwNumMat,
		&NewModelInfo.pMesh
	);

	// 例外メッセージ
	if (FAILED(hr))
	{
		std::string message = "モデル読み込みに失敗しました: ";
		message += pModelName;
		MessageBox(GetActiveWindow(), message.c_str(), "CModelManager", MB_OK);
		return -1;
	}

	//===============================================================
	// テクスチャ登録
	//===============================================================
	NewModelInfo.pTexture.clear();
	NewModelInfo.pTexture.resize(NewModelInfo.dwNumMat);

	if (NewModelInfo.pBuffMat)
	{
		// マテリアルデータのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)NewModelInfo.pBuffMat->GetBufferPointer();

		// テクスチャポインタ取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(NewModelInfo.dwNumMat); nCnt++)
		{
			if (pMat[nCnt].pTextureFilename)
			{
				// テクスチャID登録
				int nTexID = pTexture->Register(pMat[nCnt].pTextureFilename);
				NewModelInfo.pTexture[nCnt] = nTexID;
			}
			else
			{
				// 例外値をセット
				NewModelInfo.pTexture[nCnt] = -1;
			}
		}
	}

	// 配列に登録する
	m_aModelInstData.push_back(NewModelInfo);

	// 総数のインデックスを返す
	return m_nNumAll++;
}
//=========================================================
// json読み込み
//=========================================================
HRESULT CInstanceModelManager::LoadJson(void)
{
	// ファイルオープン
	std::ifstream openfile(LOAD_FILE);

	// 例外処理
	if (!openfile.is_open())
	{
		MessageBox(GetActiveWindow(), "Model.json が開けません", "CModelmanager", MB_OK);
		return E_FAIL;
	}

	// json読み込み
	json loadjson;
	openfile >> loadjson;
	openfile.close();

	// 情報がなかったら
	if (loadjson.is_null() || loadjson.empty())
	{
		MessageBox(GetActiveWindow(), "Model.json にデータがありません", "CModelmanager", MB_OK);
		return E_FAIL;
	}

	// 配列クリア
	m_aModelInstData.clear();

	// 情報をセットしていく
	for (const auto& entry : loadjson)
	{
		// 指定の文字がなかったら
		if (!entry.contains("Model") || entry["Model"].is_null()) continue;

		// jsonからファイルパス取得
		std::string filePath = entry["Model"].get<std::string>();
		if (!entry.contains("Instancing")) continue;
		if (filePath.empty()) continue;

		// モデル読み込み
		LoadModel(filePath.c_str());

		// 総数加算
		m_nNumAll++;
	}

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 関数分け読み込み
//=========================================================
void CInstanceModelManager::LoadModel(const char* pModelName)
{
	// 格納用変数
	InstanceModelInfo NewModelInfo = {};
	NewModelInfo.FilePath = pModelName;

	//===============================================================
	// モデル読み込み
	//===============================================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return;

	// Xファイルを生成する
	HRESULT hr = D3DXLoadMeshFromX
	(
		pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&NewModelInfo.pBuffMat,
		NULL,
		&NewModelInfo.dwNumMat,
		&NewModelInfo.pMesh
	);

	// 例外メッセージ
	if (FAILED(hr))
	{
		std::string message = "モデル読み込みに失敗しました: ";
		message += pModelName;
		MessageBox(GetActiveWindow(), message.c_str(), "CModelManager", MB_OK);
		return;
	}

	//===============================================================
	// 法線のスムース化を実行
	//===============================================================
	LPD3DXMESH pTempMesh = NewModelInfo.pMesh;
	std::vector<DWORD> adjacency;

	// 隣接情報用バッファ確保
	DWORD FaceNum = pTempMesh->GetNumFaces();

	// バッファのサイズを動的生成
	adjacency.resize(FaceNum * 3);

	// 隣接情報生成
	pTempMesh->GenerateAdjacency(0.0001f, adjacency.data());

	// 法線生成
	D3DXComputeNormals(pTempMesh, adjacency.data());

	// 法線付きFVFでクローン
	DWORD fvf = pTempMesh->GetFVF();

	// 複製先ポインタ
	LPD3DXMESH pCloneMesh = nullptr;

	// クローンメッシュ作成
	HRESULT hrClone = pTempMesh->CloneMeshFVF
	(
		D3DXMESH_SYSTEMMEM,
		fvf | D3DFVF_NORMAL,
		CManager::GetInstance()->GetRenderer()->GetDevice(),
		&pCloneMesh
	);

	// クローン作製成功時
	if (SUCCEEDED(hrClone))
	{
		// 元メッシュ解放と法線情報の差し替え
		pTempMesh->Release();
		pTempMesh = pCloneMesh;

		// 隣接情報を再生成
		adjacency.clear();
	}

	// メッシュを法線計算したメッシュに差し替え
	NewModelInfo.pMesh = pTempMesh;

	// VERTEX/INDEXバッファ取得
	LPDIRECT3DVERTEXBUFFER9 vtxbuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexbuffer = nullptr;

	// 元モデルから取得する
	NewModelInfo.pMesh->GetVertexBuffer(&vtxbuffer);
	NewModelInfo.pMesh->GetIndexBuffer(&indexbuffer);

	// インデックスと頂点数を設定
	NewModelInfo.InstanceData.VtxBuffer = vtxbuffer;
	NewModelInfo.InstanceData.IndexBuffer = indexbuffer;

	// 頂点数とポリゴン数を設定
	NewModelInfo.InstanceData.vtxCount = NewModelInfo.pMesh->GetNumVertices();
	NewModelInfo.InstanceData.PrimCount = NewModelInfo.pMesh->GetNumFaces();

	//===============================================================
	// マテリアル情報の設定
	//===============================================================
	NewModelInfo.pTexture.clear();
	NewModelInfo.Materials.clear();

	NewModelInfo.pTexture.resize(NewModelInfo.dwNumMat); // テクスチャバッファサイズ
	NewModelInfo.Materials.resize(NewModelInfo.dwNumMat);// マテリアルバッファサイズ

	if (NewModelInfo.pBuffMat)
	{
		// マテリアルのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)NewModelInfo.pBuffMat->GetBufferPointer();

		// テクスチャ管理クラス
		CTexture* pTexture = CManager::GetInstance()->GetTexture();

		// マテリアル配列で回す
		for (DWORD nCnt = 0; nCnt < NewModelInfo.dwNumMat; nCnt++)
		{
			//=========================
			// マテリアル本体コピー
			//=========================
			NewModelInfo.Materials[nCnt] = pMat[nCnt].MatD3D;

			//=========================
			// カラー登録
			//=========================
			NewModelInfo.Materials[nCnt].Ambient = NewModelInfo.Materials[nCnt].Diffuse;

			//=========================
			// テクスチャ登録
			//=========================
			if (pMat[nCnt].pTextureFilename)
			{
				int nTexID = pTexture->Register(pMat[nCnt].pTextureFilename);
				NewModelInfo.pTexture[nCnt] = nTexID;
			}
			else
			{
				NewModelInfo.pTexture[nCnt] = -1;
			}
		}
	}

	// 配列に登録する
	m_aModelInstData.push_back(NewModelInfo);
}