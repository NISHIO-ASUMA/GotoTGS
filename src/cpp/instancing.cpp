//=========================================================
// 
// インスタンシングシェーダークラス [ instancing.cpp ]
// Author : Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "instancing.h"

//=========================================================
// コンストラクタ
//=========================================================
CInstancing::CInstancing() : m_pInstanceDevice{},
m_pInstancing(nullptr), 
m_pDeclaration{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CInstancing::~CInstancing()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CInstancing::Init(const char* pShaderFile)
{
	// ローカル変数
	LPD3DXBUFFER pErr = nullptr;

	// 頂点宣言作成
	D3DVERTEXELEMENT9 declElems[] =
	{
		// ストリーム0
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// 座標
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// uv
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0},	// 法線

		// ストリーム1
		{ 1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// ワールド位置
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// ワールド位置
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// ワールド位置
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	// ワールド位置

		// 終了
		D3DDECL_END()
	};

	// デクレーション作成
	m_pInstanceDevice->CreateVertexDeclaration(declElems, &m_pDeclaration);

	// シェーダーファイル読み込み
	HRESULT hr = D3DXCreateEffectFromFile
	(
		m_pInstanceDevice,		// デバイス
		pShaderFile,			// 読み込み対象ファイル
		nullptr,
		nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&m_pInstancing,			// エフェクトポインタ
		&pErr
	);

	// 読み込み失敗時
	if (FAILED(hr))
	{
		if (pErr)
		{
			// エラーメッセージ表示
			MessageBoxA
			(
				nullptr,
				(char*)pErr->GetBufferPointer(),
				pShaderFile,
				MB_OK
			);

			// 解放
			pErr->Release();
		}

		return hr;
	}

	return S_OK;
}
//=========================================================
// インスタンシング開始
//=========================================================
void CInstancing::BeginInstancing
(
	int instanceCount,
	LPDIRECT3DVERTEXBUFFER9 meshVB,
	UINT meshStride,
	LPDIRECT3DINDEXBUFFER9 meshIB,
	LPDIRECT3DVERTEXBUFFER9 instanceVB,
	UINT instanceStride)
{
	//================================
	// インスタンス宣言
	//================================
	m_pInstanceDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | instanceCount);
	m_pInstanceDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//================================
	// デクレーションバッファを設定
	//================================
	m_pInstanceDevice->SetVertexDeclaration(m_pDeclaration);

	//===============================
	// メッシュバッファをストリーム0に設定
	//===============================
	m_pInstanceDevice->SetStreamSource(0, meshVB, 0, meshStride);

	//===============================
	// インスタンスバッファをストリーム1に設定
	//===============================
	m_pInstanceDevice->SetStreamSource(1, instanceVB, 0, instanceStride);

	//===============================
	// インデックスを設定する
	//===============================
	m_pInstanceDevice->SetIndices(meshIB);
}
//=========================================================
// インスタンシングパラメーター設定
//=========================================================
void CInstancing::SetInstancingParam(const LPDIRECT3DTEXTURE9 &Texture, const D3DXVECTOR4& color)
{
	// ポインタが無かったら
	if (!m_pInstancing) return;

	D3DXMATRIX mtxProj, mtxView;
	m_pInstanceDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);	// プロジェクションマトリックス
	m_pInstanceDevice->GetTransform(D3DTS_VIEW, &mtxView);			// ビューマトリックス

	// シェーダーのテクニック宣言を取得
	D3DXHANDLE hTech = m_pInstancing->GetTechniqueByName("Instancing");
	m_pInstancing->SetTechnique(hTech);

	// シェーダーパラメータを設定
	m_pInstancing->SetMatrix("g_mtxview", &mtxView);		// ビューマトリックス
	m_pInstancing->SetMatrix("g_mtxprojection", &mtxProj);	// プロジェクションマトリックス
	m_pInstancing->SetVector("g_MatColor", &color);			// マテリアル
	m_pInstancing->SetTexture("g_TexCharactor", Texture);	// テクスチャ

	// コミット切り替え
	m_pInstancing->CommitChanges();
}
//=========================================================
// 終了処理
//=========================================================
void CInstancing::Uninit(void)
{
	// エフェクトポインタの破棄
	if (m_pInstancing)
	{
		m_pInstancing->Release();
		m_pInstancing = nullptr;
	}
}
//=========================================================
// シェーダー開始関数
//=========================================================
void CInstancing::Begin(void)
{
	m_pInstancing->Begin(0,0);
}
//=========================================================
// パス開始関数
//=========================================================
void CInstancing::BeginPass(const int nPass)
{
	m_pInstancing->BeginPass(nPass);
}
//=========================================================
// インスタンシング終了
//=========================================================
void CInstancing::EndInstancing(void)
{
	m_pInstanceDevice->SetStreamSourceFreq(0, 1);
	m_pInstanceDevice->SetStreamSourceFreq(1, 1);
}
//=========================================================
// パス終了関数
//=========================================================
void CInstancing::EndPass(void)
{
	m_pInstancing->EndPass();
}
//=========================================================
// シェーダー終了関数
//=========================================================
void CInstancing::End(void)
{
	m_pInstancing->End();
}