//=========================================================
//
// ビルボードマルチテクスチャ処理 [ billboardmulti.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "billboardmulti.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CBillboardMulti::CBillboardMulti(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fWidth(0.0f),
m_fHeight(0.0f),
m_FlashCount(NULL),
m_nCountAnim(NULL),
m_nPatterAnim(NULL),
m_nIdxTexture(-1),
m_col(COLOR_WHITE),
m_isTests(true),
m_pVtxBuff(nullptr),
m_mtxWorld{}
{
	m_apTexture.clear();
}
//=========================================================
// デストラクタ
//=========================================================
CBillboardMulti::~CBillboardMulti()
{

}
//=========================================================
// 生成処理
//=========================================================
CBillboardMulti* CBillboardMulti::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	float fWidth, 
	float fHeight, 
	const char* pTexName1, 
	const char* pTexName2
)
{
	// インスタンス生成
	CBillboardMulti* pBillboard = new CBillboardMulti;
	if (pBillboard == nullptr) return nullptr;

	// オブジェクトセット
	pBillboard->SetPos(pos);
	pBillboard->SetSize(fWidth, fHeight);
	pBillboard->SetRot(rot);
	pBillboard->SetTexture(pTexName1,0);
	pBillboard->SetTexture(pTexName2,1);

	// 初期化失敗時
	if (FAILED(pBillboard->Init())) return nullptr;

	// ビルボードのポインタを返す
	return pBillboard;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBillboardMulti::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D_MULTI* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = 
	pVtx[1].pos = 
	pVtx[2].pos = 
	pVtx[3].pos = VECTOR3_NULL;

	// 法線情報の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = COLOR_WHITE;

	// 1枚目
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 2枚目
	pVtx[0].texMulti = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMulti = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texMulti = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].texMulti = D3DXVECTOR2(1.0f, 0.5f);

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBillboardMulti::Uninit(void)
{
	// テクスチャクラスの破棄
	m_apTexture.clear();

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CBillboardMulti::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D_MULTI* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	// アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 描画処理
//=========================================================
void CBillboardMulti::Draw(void)
{
	// nullチェック
	if (m_pVtxBuff == nullptr) return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTextureManager = CManager::GetInstance()->GetTexture();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストの設定（変更前の状態を覚えておく）
	if (m_isTests)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得しカメラの逆回転を設定
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

	// テクスチャ設定ループ
	for (int nCnt = 0; nCnt < static_cast<int>(m_apTexture.size()); nCnt++)
	{
		int nIdx = m_apTexture[nCnt];
		if (nIdx < 0) continue;

		LPDIRECT3DTEXTURE9 pTexData = pTextureManager->GetAddress(nIdx);
		if (pTexData == nullptr) continue;

		pDevice->SetTexture(static_cast<DWORD>(nCnt), pTexData);

		if (nCnt == 0)
		{
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		}
		else
		{
			// テクスチャの乗算設定
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG1, D3DTA_TEXTURE); // 2枚目の色
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG2, D3DTA_CURRENT); // 1枚目の結果

			// アルファ値の乗算
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		}
	}

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	for (int i = 0; i < static_cast<int>(m_apTexture.size()); i++)
	{
		pDevice->SetTexture(i, nullptr);
	}

	for (int i = 1; i < static_cast<int>(m_apTexture.size()); i++)
	{
		pDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	// 標準の Direct3D9 設定に戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// Z書き込み許可を必ず TRUE に戻す
	if (m_isTests)
	{
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//=========================================================
// テクスチャセット ( 基底クラスから取得 )
//=========================================================
void CBillboardMulti::SetTexture(const char* pRegisterName, const int& nIdx)
{
	// 例外チェック
	if (nIdx < 0) return;
	if (pRegisterName == nullptr) return;

	// テクスチャクラス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パスを短縮する
	std::string TexName = "data/TEXTURE/";
	TexName += pRegisterName;

	// テクスチャ登録
	int nIdxTexture = pTexture->Register(TexName.c_str());
	if (nIdxTexture == -1) return;

	// 配列サイズを設定
	if (nIdx >= static_cast<int>(m_apTexture.size()))
	{
		m_apTexture.resize(nIdx + 1, -1);
	}

	// 指定したステージにインデックスを格納
	m_apTexture[nIdx] = nIdxTexture;
}
//=========================================================
// UV設定処理
//=========================================================
void CBillboardMulti::SetUV(const float fRatio)
{
	// クランプ処理
	Clump(fRatio, 0.0f, 1.0f);

	// 頂点情報のポインタ 
	VERTEX_3D_MULTI* pVtx = nullptr;

	if (m_pVtxBuff == nullptr) return;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定 ( ベース )
	pVtx[0].tex.x = 0.0f; pVtx[0].tex.y = 0.0f;
	pVtx[1].tex.x = 1.0f; pVtx[1].tex.y = 0.0f;
	pVtx[2].tex.x = 0.0f; pVtx[2].tex.y = 1.0f;
	pVtx[3].tex.x = 1.0f; pVtx[3].tex.y = 1.0f;

	// uv座標のオフセット
	float vOffset = fRatio * 0.5f;

	// 2枚目の設定 ( マルチ側 )
	// 上端の頂点
	pVtx[0].texMulti.x = 0.0f; pVtx[0].texMulti.y = 0.0f + vOffset;
	pVtx[1].texMulti.x = 1.0f; pVtx[1].texMulti.y = 0.0f + vOffset;

	// 下端の頂点
	pVtx[2].texMulti.x = 0.0f; pVtx[2].texMulti.y = 0.5f + vOffset;
	pVtx[3].texMulti.x = 1.0f; pVtx[3].texMulti.y = 0.5f + vOffset;

	// アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 点滅処理
//=========================================================
void CBillboardMulti::Flash(const int nMaxFlashTime, const int Digittime)
{
	// 頂点情報のポインタ
	VERTEX_3D_MULTI* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 点滅カウントを加算
	m_FlashCount++;

	// カラー変数
	D3DXCOLOR col = COLOR_WHITE;

	// 点滅カウントと一致したとき
	if (m_FlashCount == Digittime)
	{
		// 頂点カラーの設定
		col = COLOR_GLAY;

		// カラーセット
		SetCol(col);
	}
	else if (m_FlashCount == nMaxFlashTime)
	{
		// 頂点カラーの設定
		col = COLOR_WHITE;

		// カラーセット
		SetCol(col);

		// 初期値に戻す
		m_FlashCount = NULL;
	}

	//アンロック
	m_pVtxBuff->Unlock();
}