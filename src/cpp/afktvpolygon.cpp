//=========================================================
//
// AFK2DUI処理 [ afk2dui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afktvpolygon.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "manager.h"
#include "gamesceneobject.h"
#include "afkmanager.h"
#include "player.h"
#include "afk.h"
#include "afktv.h"

//=================================================
// 名前空間
//=================================================
namespace AFKTVPOLYGON
{
	const D3DXVECTOR3 Pos = { 1000.0f, 45.0f, 300.0f };			// 3D画像の座標
	constexpr float fWidth = 100.0f;							// 横幅
	constexpr float fHeight = 50.0f;							// 縦幅
	constexpr const char* Button_NAME = "AfkButton.png";		// チュートリアルuiのテクスチャ名
};

//=================================================
// 静的メンバ変数
//=================================================
CAfkTVPolygon* CAfkTVPolygon::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CAfkTVPolygon::CAfkTVPolygon() : m_pVtxBuff(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_nIdxTexture(-1),
m_bDisplay(true),
m_mtxWorld{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CAfkTVPolygon::~CAfkTVPolygon()
{

}
//=========================================================
// 生成処理
//=========================================================
CAfkTVPolygon* CAfkTVPolygon::Create(void)
{
	// インスタンス生成
	CAfkTVPolygon* pAfkTVPolygon = new CAfkTVPolygon;
	if (pAfkTVPolygon == nullptr) return nullptr;

	// オブジェクトセット
	pAfkTVPolygon->SetPos(AFKTVPOLYGON::Pos);
	pAfkTVPolygon->SetTexture(AFKTVPOLYGON::Button_NAME);

	// 初期化失敗時
	if (FAILED(pAfkTVPolygon->Init())) return nullptr;

	// UIのポインタを返す
	return pAfkTVPolygon;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkTVPolygon::Init(void)
{
	SetPos(AFKTVPOLYGON::Pos);
	SetRot(VECTOR3_NULL);
	SetTexture(AFKTVPOLYGON::Button_NAME);

	// デバイスポインタを宣言
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
void CAfkTVPolygon::Uninit(void)
{
	// バッファ解放
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// シングルトンの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CAfkTVPolygon::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-AFKTVPOLYGON::fWidth, AFKTVPOLYGON::fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(AFKTVPOLYGON::fWidth, AFKTVPOLYGON::fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-AFKTVPOLYGON::fWidth, -AFKTVPOLYGON::fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(AFKTVPOLYGON::fWidth, -AFKTVPOLYGON::fHeight, 0.0f);

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
}
//=========================================================
// 描画処理
//=========================================================
void CAfkTVPolygon::Draw(void)
{
	if (!m_bDisplay) return;

	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	// テクスチャを消す
	pDevice->SetTexture(0, nullptr);
}
//=========================================================
// インスタンス取得処理
//=========================================================
CAfkTVPolygon* CAfkTVPolygon::Instance(void)
{
	// nullチェック
	if (m_pInstance == nullptr)m_pInstance = new CAfkTVPolygon;

	// 生成されたインスタンスを返す
	return m_pInstance;
}
//=========================================================
// テクスチャ割り当て
//=========================================================
void CAfkTVPolygon::SetTexture(const char* pTexName)
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