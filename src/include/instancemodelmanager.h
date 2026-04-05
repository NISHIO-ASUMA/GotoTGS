//=========================================================
//
// インスタンシング適用モデル管理クラス [ instancemodelmanager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>
#include <vector>

//*********************************************************
// インスタンシング適用モデル管理クラスを定義
//*********************************************************
class CInstanceModelManager
{
public:

	//*************************
	// インスタンシングデータ構造体
	//*************************
	struct INSTANCEDATA
	{
		LPDIRECT3DVERTEXBUFFER9 VtxBuffer;		// 頂点バッファ
		LPDIRECT3DINDEXBUFFER9  IndexBuffer;	// インデックスバッファ
		UINT vtxCount;							// 頂点カウント
		UINT PrimCount;							// ポリゴンカウント
	};

	//*************************
	// 読み込み用構造体
	//*************************
	struct InstanceModelInfo
	{
		LPD3DXMESH pMesh;			// メッシュのポインタ
		LPD3DXBUFFER pBuffMat;		// マテリアルのポインタ
		DWORD dwNumMat;				// マテリアル数
		std::vector<int>pTexture;	// テクスチャの動的ポインタ
		std::string FilePath;		// ファイル名
		int nParnent;				// 親を持つ
		std::vector<D3DMATERIAL9> Materials; // マテリアルの動的確保配列
		INSTANCEDATA InstanceData;	// 構造体変数
	};

	CInstanceModelManager();
	~CInstanceModelManager();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// モデルの登録
	/// </summary>
	/// <param name="pModelName">モデル名</param>
	/// <returns></returns>
	int Register(const char* pModelName);

	/// <summary>
	/// 配列番号を指定して情報を取得
	/// </summary>
	/// <param name="nIdx">取得するオブジェクトインデックス</param>
	/// <returns>インデックス番号に応じたデータ</returns>
	const InstanceModelInfo& GetInfo(int nIdx) const { return m_aModelInstData[nIdx]; }

	/// <summary>
	/// 動的配列の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>配列ポインタ</returns>
	const std::vector<InstanceModelInfo>& GetList(void) const { return m_aModelInstData; }

private:

	HRESULT LoadJson(void);							// jsonfile読み込み関数
	void LoadModel(const char* pModelName);			// モデル登録関数
	
	std::vector<InstanceModelInfo>m_aModelInstData;	// モデルデータ管理配列
	static int m_nNumAll;							// モデルの総数

	static constexpr const char* LOAD_FILE = "data/JSON/InstanceModel.json"; // 読み込み対象ファイル名
};