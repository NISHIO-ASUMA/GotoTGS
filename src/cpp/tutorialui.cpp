//=========================================================
//
// チュートリアルUI処理 [ tutorialui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "player.h"
#include "gamesceneobject.h"
#include "gametime.h"
#include "easing.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace UI
{
	float fMaxFrame = 120.0f;	// イージング用マックスフレーム変数
	int nDeleteTime = 570;		// チュートリアル用UIの表示の仕方を切り替えるための時間
};

//=========================================================
// コンストラクタ
//=========================================================
CTutorialUI::CTutorialUI(int nPriority) : CBillboard(nPriority),
m_pCollider(nullptr),
m_bLook(false),
m_bTime(true),
m_bEasing(false),
m_fCountFrame(NULL),
m_fMaxFrame(NULL)
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialUI::~CTutorialUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CTutorialUI* CTutorialUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fWidth, float fHeight, const char* pTexName)
{
	// インスタンス生成
	CTutorialUI* pTutorialUI = new CTutorialUI;
	if (pTutorialUI == nullptr) return nullptr;

	// オブジェクトセット
	pTutorialUI->SetPos(pos);
	pTutorialUI->SetSize(fWidth, fHeight);
	pTutorialUI->SetRot(rot);
	pTutorialUI->SetTexture(pTexName);
	pTutorialUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pTutorialUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pTutorialUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialUI::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	// マックスフレームの設定
	m_fMaxFrame = UI::fMaxFrame;

	// 球形コライダーを生成
	m_pCollider = CSphereCollider::Create(GetPos(),75.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialUI::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();

	// スフィアコライダーの破棄
	m_pCollider.reset();

}
//=========================================================
// 更新処理
//=========================================================
void CTutorialUI::Update(void)
{
	// プレイヤーの情報を取得し判定を生成
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();

	// コライダー取得とnullチェック
	CSphereCollider* Collider = Player->GetSphereCollider();
	if (Collider == nullptr) return;

	// 当たり判定の実行
	if (Collision(Collider))m_bLook = true;
	else m_bLook = false;
	
	// 現在の時間を取得
	int nNowTime = CGameSceneObject::GetInstance()->GetTime()->GetAllTime();

	// タイムが570秒以下だったら
	if (nNowTime <= UI::nDeleteTime)m_bTime = false;
	
	//=====================================
	// イージング関数
	// NOTE:調整中(近田)
	// Easing(GetWidth(),GetHeight());
	//=====================================

	// 親クラスの更新処理
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutorialUI::Draw(void)
{
	// 親クラスの描画処理
	if(m_bLook || m_bTime)	CBillboard::Draw();
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CTutorialUI::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pCollider.get(),pOther);
}
//=========================================================
// イージング処理
//=========================================================
void CTutorialUI::Easing(float fWidth, float fHeight)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { 0.075f * fWidth, 0.075f * fHeight };

	// 目標の大きさ
	D3DXVECTOR2 Dest = { 0.05f * fWidth, 0.05f * fHeight };

	// イージング用変数
	D3DXVECTOR2 Size = {};

	// イージング判定が無効なら
	if (!m_bEasing)
	{
		// アニメーションカウンターを進める
		m_fCountFrame++;

		// 今のアニメーションの進行割合を計算
		float Ratio = CEasing::EaseInOutSine(m_fCountFrame / m_fMaxFrame);

		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x, Dest.y - Apper.y };
		
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;
	}
	// イージング判定が有効なら
	else if (m_bEasing)
	{
		// アニメーションカウンターを進める
		m_fCountFrame--;

		// 今のアニメーションの進行割合を計算
		float Ratio = CEasing::EaseInOutSine(m_fCountFrame / m_fMaxFrame);

		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x + Apper.x, Dest.y + Apper.y };

		// 今の大きさを計算
		Size = Apper - Diff * Ratio;

	}

	// サイズの設定
	SetSize(Size.x, Size.y);

	// フレームカウントがマックスフレームと一緒になったら
	if (m_fCountFrame == m_fMaxFrame)m_bEasing = true;
	
	// フレームカウントが0.0fなら
	else if (m_fCountFrame == 0.0f)m_bEasing = false;
}