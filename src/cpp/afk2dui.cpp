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
#include "gamesceneobject.h"
#include "easing.h"
#include "afkmanager.h"
#include "player.h"
#include "afk.h"
#include "afksmoke.h"
#include "afktv.h"
#include "afkmagazine.h"
#include "afkgamecenter.h"
#include "afkeating.h"
#include "afkbench.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace AFK2DUI
{
	const D3DXVECTOR3 Pos = { 640.0f, 640.0f, 0.0f };			// 2D画像の座標
	const D3DXVECTOR2 Apper = { 0.15f, 0.05f };					// 初期のサイズ
	const D3DXVECTOR2 Dest = { 0.25f, 0.1f };					// 目標のサイズ
	constexpr float fWidth = 250.0f;							// 横幅
	constexpr float fHeight = 75.0f;							// 縦幅
	constexpr float fMaxFrame = 60.0f;							// マックスフレーム
	constexpr const char* Button_NAME = "AfkButton.png";		// サボりuiのテクスチャ名
};

//=========================================================
// コンストラクタ
//=========================================================
CAfk2DUI::CAfk2DUI(int nPriority) : CObject2D(nPriority)
,m_fMaxFrame(NULL),
m_fCountFrame(NULL),
m_bAfkButton(false),
m_bEasing(false),
m_bDisplay(true)
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

	// UIのポインタを返す
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
	// 非表示ならスキップする
	if (!m_bDisplay) return;

	// イージング
	EasingSine();

	// 親クラスの更新処理
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CAfk2DUI::Draw(void)
{
	// 非表示ならスキップする
	if (!m_bDisplay) return;

	// 親クラスの描画
	CObject2D::Draw();
}
//=========================================================
// イージングサイン使用関数
//=========================================================
void CAfk2DUI::EasingSine(void)
{
	// カウントの増減
	if (!m_bEasing)
	{
		m_fCountFrame += 1.0f;
		if (m_fCountFrame >= m_fMaxFrame)
		{
			m_fCountFrame = m_fMaxFrame;
			m_bEasing = true; // 反転
		}
	}
	else
	{
		m_fCountFrame -= 1.0f;
		if (m_fCountFrame <= 0.0f)
		{
			m_fCountFrame = 0.0f;
			m_bEasing = false; // 反転
		}
	}

	// 座標を取得
	auto pos = GetPos();

	// 開始時
	D3DXVECTOR2 Apper = { AFK2DUI::Apper.x * pos.x, AFK2DUI::Apper.y * pos.y };

	// 目的地
	D3DXVECTOR2 Dest = { AFK2DUI::Dest.x * pos.x,  AFK2DUI::Dest.y * pos.y };

	// イージングの割合計算
	float Ratio = CEasing::EaseInOutSine(m_fCountFrame / m_fMaxFrame);

	// 差分と大きさの計算
	D3DXVECTOR2 Diff = { Dest.x - Apper.x, Dest.y - Apper.y };
	D3DXVECTOR2 Size = Apper + Diff * Ratio;

	// オブジェクトのサイズの設定
	CObject2D::SetSize(Size.x, Size.y);
}