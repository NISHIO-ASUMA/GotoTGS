//=========================================================
//
// 3D数字処理 [ number3d.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "number3d.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CNumber3D::CNumber3D() : m_pos(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_col(COLOR_WHITE),
m_nIdxTexture(-1),
m_pVtxBuff(nullptr),
m_TexU1(NULL),
m_TexU(NULL),
m_TexV(NULL),
n_nColorCount(NULL),
m_isUse(true)
{
}
//=========================================================
// デストラクタ
//=========================================================
CNumber3D::~CNumber3D()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CNumber3D::Init(const D3DXVECTOR3& pos, float fwidth, float fheight)
{
	// 値を格納
	m_pos = pos;
	m_fWidth = fwidth;
	m_fHeight = fheight;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 3D頂点バッファの作成
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// バッファ生成チェック
	if (FAILED(hr)) return hr;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// カラーの設定
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = m_col;

	// テクスチャ座標の設定(あくまで初期値をいれる)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Config::DIGIT_VALUE, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Config::END_FLOAT);
	pVtx[3].tex = D3DXVECTOR2(Config::DIGIT_VALUE, Config::END_FLOAT);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CNumber3D::Uninit(void)
{
	// 頂点バッファの解放
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CNumber3D::Update(void)
{
	if (!m_isUse) return;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxView, mtxBillboard, mtxTrans, mtxWorld;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxBillboard);

	mtxBillboard._11 = mtxView._11; mtxBillboard._12 = mtxView._12; mtxBillboard._13 = mtxView._13;
	mtxBillboard._21 = mtxView._21; mtxBillboard._22 = mtxView._22; mtxBillboard._23 = mtxView._23;
	mtxBillboard._31 = mtxView._31; mtxBillboard._32 = mtxView._32; mtxBillboard._33 = mtxView._33;

	//
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// ワールド行列の合成
	D3DXMatrixMultiply(&mtxWorld, &mtxBillboard, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}
//=========================================================
// 行列設定処理
//=========================================================
void CNumber3D::SetMtx(void)
{
	if (!m_isUse) return;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxView, mtxBillboard, mtxTrans, mtxWorld;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// マトリックスの設定
	D3DXMatrixIdentity(&mtxBillboard);
	mtxBillboard._11 = mtxView._11; mtxBillboard._12 = mtxView._12; mtxBillboard._13 = mtxView._13;
	mtxBillboard._21 = mtxView._21; mtxBillboard._22 = mtxView._22; mtxBillboard._23 = mtxView._23;
	mtxBillboard._31 = mtxView._31; mtxBillboard._32 = mtxView._32; mtxBillboard._33 = mtxView._33;

	// 位置を設定
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// 行列計算
	D3DXMatrixMultiply(&mtxWorld, &mtxBillboard, &mtxTrans);

	// デバイスにワールドマトリックスを適用
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}
//=========================================================
// 描画処理
//=========================================================
void CNumber3D::Draw(void)
{
	// 未使用 または 頂点バッファのnull
	if (!m_isUse || m_pVtxBuff == nullptr) return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=========================================================
// テクスチャ設定処理
//=========================================================
void CNumber3D::SetTexture(const char* pTexName)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パス設定
	std::string TexPath = "data/TEXTURE/";
	TexPath += pTexName;

	// テクスチャ割り当て
	m_nIdxTexture = pTexture->Register(TexPath.c_str());
}
//=========================================================
// 大きさ設定処理
//=========================================================
void CNumber3D::SetSize(float fWidth, float fHeight)
{
	// メンバに格納
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// 頂点バッファがnullなら
	if (m_pVtxBuff == nullptr) return;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点のロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標等の設定の変更
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	// 頂点のアンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 桁数設定処理
//=========================================================
void CNumber3D::SetDigit(int nDigit)
{
	// メンバ変数への格納
	m_TexU = nDigit * Config::DIGIT_VALUE;
	m_TexU1 = m_TexU + Config::DIGIT_VALUE;
	m_TexV = 0.0f;

	// 頂点バッファがnullなら
	if (m_pVtxBuff == nullptr) return;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点のロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_TexU, m_TexV);
	pVtx[1].tex = D3DXVECTOR2(m_TexU1, m_TexV);
	pVtx[2].tex = D3DXVECTOR2(m_TexU, m_TexV + Config::END_FLOAT);
	pVtx[3].tex = D3DXVECTOR2(m_TexU1, m_TexV + Config::END_FLOAT);

	// 頂点のアンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// カラー設定処理
//=========================================================
void CNumber3D::SetCol(const D3DXCOLOR& col)
{
	// 値をセット
	m_col = col;

	// 頂点バッファがnullなら
	if (m_pVtxBuff == nullptr) return;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点のロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラー設定
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = m_col;

	// アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 点滅処理
//=========================================================
void CNumber3D::SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col)
{
}