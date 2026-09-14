//=========================================================
//
// 回転適用する3Dポリゴンクラス [ object3drotation.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "object3drotation.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "renderer.h"

//=========================================================
// コンストラクタ
//=========================================================
CObjectRotation3D::CObjectRotation3D(int nPriority) : CObject(nPriority),
m_col(COLOR_WHITE),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_mtxWorld{},
m_nIdxTexture(-1),
m_pVtxBuff(nullptr),
m_isDraw(true)
{

}
//=========================================================
// デストラクタ
//=========================================================
CObjectRotation3D::~CObjectRotation3D()
{

}
//=========================================================
// ポインタ生成処理
//=========================================================
CObjectRotation3D* CObjectRotation3D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CObjectRotation3D* pObject = new CObjectRotation3D;
	if (!pObject) return nullptr;

	// 初期化失敗時
	if (FAILED(pObject->Init()))
		return nullptr;

	// 各種設定
	pObject->SetPos(pos);
	pObject->SetRot(rot);
	pObject->SetSize(100.0f, 100.0f);
	pObject->SetTexture("arrow_base.png");

	// 生成されたポインタを返す
	return pObject;
}

//=========================================================
// 初期化処理 ( ここは単純に頂点生成と初期値を格納するだけ )
//=========================================================
HRESULT CObjectRotation3D::Init(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos =
	pVtx[1].pos =
	pVtx[2].pos =
	pVtx[3].pos = VECTOR3_NULL;

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = COLOR_WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CObjectRotation3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理 ( 実際の適用と角度の適用とマトリックスの更新 )
//=========================================================
void CObjectRotation3D::Update(void)
{
	// 参考例(回転しない時)
#if 0
		// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fWidth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fWidth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, -m_fWidth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, -m_fWidth);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// 行列計算
	m_mtxWorld = mtxRot * mtxTrans;
#else
	// バッファチェック
	if (!m_pVtxBuff) return;

	// 頂点バッファのポインタを宣言
	VERTEX_3D* pVtx = nullptr;

	// ロックに成功したら
	if (SUCCEEDED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, D3DLOCK_DISCARD)))
	{
		// 半分の値
		float fHalfW = m_fWidth * 0.5f;
		float fHalfH = m_fHeight * 0.5f;

		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(-fHalfW, 0.0f, fHalfH);
		pVtx[1].pos = D3DXVECTOR3(fHalfW, 0.0f, fHalfH);
		pVtx[2].pos = D3DXVECTOR3(-fHalfW, 0.0f, -fHalfH);
		pVtx[3].pos = D3DXVECTOR3(fHalfW, 0.0f, -fHalfH);

		// 頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		// 法線の設定
		pVtx[0].nor = 
		pVtx[1].nor = 
		pVtx[2].nor = 
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// バッファのアンロック
		m_pVtxBuff->Unlock();
	}
	
	// ワールド変換行列の合成
	D3DXMATRIX mtxRot, mtxTrans;

	// 回転行列生成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 移動行列生成
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// ワールドマトリックスの設定
	m_mtxWorld = mtxRot * mtxTrans;
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CObjectRotation3D::Draw(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αテスト実行
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ割り当て
	if (m_nIdxTexture == -1)
	{
		pDevice->SetTexture(0, nullptr);
	}
	else
	{
		// テクスチャクラス取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		if (pTexture == nullptr) return;

		// テクスチャセット
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// α設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// テクスチャを消す
	pDevice->SetTexture(0, nullptr);
}
//=========================================================
// テクスチャ処理
//=========================================================
void CObjectRotation3D::SetTexture(const char* pTexName)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// 文字列を連結
	std::string TexPath = "data/TEXTURE/";
	TexPath += pTexName;

	// 割り当て
	m_nIdxTexture = pTexture->Register(TexPath.c_str());
}