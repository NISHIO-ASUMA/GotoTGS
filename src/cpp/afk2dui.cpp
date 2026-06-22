//=========================================================
//
// AFK2DUI処理 [ afk2dui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afk2dui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "spherecollider.h"
#include "collisionsphere.h"
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
namespace AFK2DUI
{
	const D3DXVECTOR3 Pos = { 640.0f, 640.0f, 0.0f };			// 2D画像の座標
	const D3DXVECTOR2 Apper = { 0.15f, 0.05f };					// 初期のサイズ
	const D3DXVECTOR2 Dest = { 0.25f, 0.1f };					// 目標のサイズ
	constexpr float fWidth = 250.0f;							// 横幅
	constexpr float fHeight = 75.0f;							// 縦幅
	constexpr float fMaxFrame = 60.0f;							// マックスフレーム
	constexpr const char* Button_NAME = "AfkButton.png";		// チュートリアルuiのテクスチャ名
	constexpr const char* GameCenter_NAME = "gamecenter.png";	// ゲームセンターuiのテクスチャ名
	constexpr const char* Smoke_NAME = "smoke.png";				// たばこUIのテクスチャ名
};

//=================================================
// 静的メンバ変数
//=================================================
CAfk2DUI* CAfk2DUI::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CAfk2DUI::CAfk2DUI() : m_pos(VECTOR3_NULL),
m_fMaxFrame(NULL),
m_fCountFrame(NULL),
m_bAfkButton(false),
m_bEasing(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CAfk2DUI::~CAfk2DUI()
{

}
//=========================================================
// 生成処理
//=========================================================
CAfk2DUI* CAfk2DUI::Create(void)
{
	// インスタンス生成
	CAfk2DUI* pAfk2DUI = new CAfk2DUI;
	if (pAfk2DUI == nullptr) return nullptr;

	// オブジェクトセット
	pAfk2DUI->SetPos(AFK2DUI::Pos);
	pAfk2DUI->SetAnchor(ANCHORTYPE_CENTER);
	pAfk2DUI->SetTexture(AFK2DUI::Button_NAME);
	pAfk2DUI->SetSize(AFK2DUI::fWidth, AFK2DUI::fHeight);

	// 初期化失敗時
	if (FAILED(pAfk2DUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pAfk2DUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfk2DUI::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// イージングの設定値
	m_fCountFrame = NULL;
	m_fMaxFrame = AFK2DUI::fMaxFrame;
	
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfk2DUI::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfk2DUI::Update(void)
{
	// 親クラスの更新処理
	CObject2D::Update();

	// イージング
	EasingSine();

	// プレイヤーがさぼっているか判定用の変数
	bool bAfkSmoke = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkSmoke();
	bool bAfkTV = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkTV();
	bool bAfkMagazine = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkMagazine();
	bool bAfkGameCenter = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkGameCenter();

	if (!bAfkSmoke && !bAfkTV && !bAfkMagazine && !bAfkGameCenter)
	{// さぼっていなかったら
		SetTexture(AFK2DUI::Button_NAME);
	}
	
	// ゲームセンターでさぼっていたら
	else if(bAfkGameCenter) SetTexture(AFK2DUI::GameCenter_NAME);
	
	// たばこでさぼっていたら
	else if (bAfkSmoke)SetTexture(AFK2DUI::Smoke_NAME);
	
}
//=========================================================
// 描画処理
//=========================================================
void CAfk2DUI::Draw(void)
{
	auto bAfkSmoke = CAfkManager::Instance()->GetAfkSmoke()->GetAfk();
	auto bAfkTV = CAfkManager::Instance()->GetAfkTV()->GetAfk();
	auto bAfkMagazine = CAfkManager::Instance()->GetAfkMagazine()->GetAfk();
	auto bAfkGameCenter = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk();

	// 親クラスの描画処理
	if(bAfkSmoke || bAfkTV || bAfkMagazine || bAfkGameCenter)CObject2D::Draw();
}
//=========================================================
// インスタンス取得処理
//=========================================================
CAfk2DUI* CAfk2DUI::Instance(void)
{
	// nullチェック
	if (m_pInstance == nullptr)m_pInstance = new CAfk2DUI;

	// 生成されたインスタンスを返す
	return m_pInstance;
}
//=========================================================
// イージングサイン使用関数
//=========================================================
void CAfk2DUI::EasingSine(void)
{
	// 位置の取得
	auto pos = GetPos();

	// 初期の大きさ
	D3DXVECTOR2 Apper = { AFK2DUI::Apper.x * pos.x, AFK2DUI::Apper.y * pos.y };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { AFK2DUI::Dest.x * pos.x,AFK2DUI::Dest.y * pos.y };

	// イージング判定が無効なら
	if (m_bEasing == false)
	{
		// アニメーションカウンターを進める
		m_fCountFrame++;

		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = CEasing::EaseInOutSine(m_fCountFrame / m_fMaxFrame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		// サイズの設定
		CObject2D::SetSize(Size.x, Size.y);

	}
	// イージング判定が有効なら
	else if (m_bEasing == true)
	{
		// アニメーションカウンターを進める
		m_fCountFrame--;

		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = CEasing::EaseInOutSine(m_fCountFrame / m_fMaxFrame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x + Apper.x,Dest.y + Apper.y };
		// 今の大きさを計算
		Size = Apper - Diff * Ratio;

		// サイズの設定
		CObject2D::SetSize(Size.x, Size.y);
	}
	// フレームカウントがマックスフレームと一緒になったら
	else if (m_fCountFrame == m_fMaxFrame)
	{
		// イージング判定を有効にする
		m_bEasing = true;
	}
	// フレームカウントが0.0fなら
	else if (m_fCountFrame == 0.0f)
	{
		// イージング判定を無効にする
		m_bEasing = false;
	}
}