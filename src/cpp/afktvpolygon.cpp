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
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "gamesceneobject.h"
#include "easing.h"
#include "afkmanager.h"
#include "player.h"
#include "afk.h"
#include "afksmoke.h"
#include "afktv.h"
#include "afkmagazine.h"
#include "afkgamecenter.h"

//=================================================
// 名前空間
//=================================================
namespace AFKTVPOLYGON
{
	const D3DXVECTOR3 Pos = { -248.0f, 45.0f, 360.0f };			// 3D画像の座標
	constexpr float fWidth = 250.0f;							// 横幅
	constexpr float fHeight = 75.0f;							// 縦幅
	constexpr const char* Button_NAME = "AfkButton.png";		// チュートリアルuiのテクスチャ名
};

//=================================================
// 静的メンバ変数
//=================================================
CAfkTVPolygon* CAfkTVPolygon::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CAfkTVPolygon::CAfkTVPolygon() : m_pos(VECTOR3_NULL),
m_bDisplay(true)
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
	pAfkTVPolygon->SetSize(AFKTVPOLYGON::fWidth, AFKTVPOLYGON::fHeight);

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
	// 親クラスの初期化処理
	CObject3D::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkTVPolygon::Uninit(void)
{
	// 親クラスの終了処理
	CObject3D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkTVPolygon::Update(void)
{
	// 親クラスの更新処理
	CObject3D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CAfkTVPolygon::Draw(void)
{
	if (!m_bDisplay) return;

	// 親クラスの描画処理
	CObject3D::Draw();
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