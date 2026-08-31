//=========================================================
//
// 受付人のui処理 [ receptionUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "receptionUI.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "gamesceneobject.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "easing.h"
#include "player.h"
#include "deskwork.h"
#include "outsidework.h"
#include "receptionlineUI.h"

//=================================================
// 名前空間
//=================================================
namespace ReceptionUI
{
	const D3DXVECTOR3 Pos = { 640.0f, 640.0f, 0.0f };					// UIの座標
	const D3DXVECTOR3 ReceptionPos = { 360.0f, 30.0f, 215.0f };			// 対象の座標
	const D3DXVECTOR3 LinePos = { 360.0f, 80.0f, 210.0f };				// セリフの座標
	const D3DXVECTOR2 Apper = { 0.15f, 0.05f };							// 初期のサイズ
	const D3DXVECTOR2 Dest = { 0.25f, 0.1f };							// 目標のサイズ
	constexpr float fRadius = 25.0f;									// 半径
	constexpr float fWidth = 250.0f;									// 横幅
	constexpr float fHeight = 75.0f;									// 縦幅
	constexpr float fMaxFrame = 60.0f;									// 最大フレーム
	constexpr const char* OPEN_Texture = "work_outside.png";			// 開錠時のテクスチャ名
	//constexpr const char* CLOSE_Texture = "work_outside.png";			// 閉錠時のテクスチャ名
	constexpr const char* LINE_Texture = "start_outsidetask000.png";	// セリフのテクスチャ名
};

//=========================================================
// コンストラクタ
//=========================================================
CReceptionUI::CReceptionUI() : m_pSphereCollider(nullptr),
m_pos(VECTOR3_NULL),
m_fMaxFrame(NULL),
m_fCountFrame(NULL),
m_bEasing(false),
m_bDisplay(false),
m_bUse(false),
m_pPlayerOwner(nullptr),
m_pLineUI(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CReceptionUI::~CReceptionUI()
{

}
//=========================================================
// 生成処理
//=========================================================
CReceptionUI* CReceptionUI::Create(CPlayer* pPlayer)
{
	// インスタンス生成
	CReceptionUI* pReceptionUI = new CReceptionUI;
	if (pReceptionUI == nullptr) return nullptr;

	// オブジェクトセット
	pReceptionUI->SetPos(ReceptionUI::Pos);
	pReceptionUI->SetAnchor(ANCHORTYPE_CENTER);
	pReceptionUI->SetTexture(ReceptionUI::OPEN_Texture);
	pReceptionUI->SetSize(ReceptionUI::fWidth, ReceptionUI::fHeight);
	pReceptionUI->SetPlayerOwner(pPlayer);

	// 初期化失敗時
	if (FAILED(pReceptionUI->Init())) return nullptr;

	// UIのポインタを返す
	return pReceptionUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CReceptionUI::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// イージングの設定値
	m_fCountFrame = NULL;
	m_fMaxFrame = ReceptionUI::fMaxFrame;

	// 球形コライダーを生成
	m_pSphereCollider = CSphereCollider::Create(ReceptionUI::ReceptionPos, ReceptionUI::fRadius);

	// セリフUI生成
	m_pLineUI = CReceptionlineUI::Create(ReceptionUI::LinePos, ReceptionUI::LINE_Texture);
	m_pLineUI->SetDrawFlags(false);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CReceptionUI::Uninit(void)
{
	// スフィアコライダーの破棄
	m_pSphereCollider.reset();

	// セリフUIの破棄
	m_pLineUI = nullptr;

	// 親クラスの終了処理
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CReceptionUI::Update(void)
{
	// ポインタがnullなら
	if (!m_pPlayerOwner) return;

	// スフィアコライダー取得とnullチェック
	CSphereCollider* SphereCollider = m_pPlayerOwner->GetSphereCollider();
	if (SphereCollider == nullptr) return;

	// タスクの情報を取得
	auto* pDesk = CGameSceneObject::GetInstance()->GetDesk();
	if (pDesk == nullptr) return;

	// 当たっていない
	if (!CollisionSphere(SphereCollider) || 
		!pDesk->GetOutsideDesk()->GetGoOutside())
	{
		// 描画しない
		m_bDisplay = false;

		return;
	}

	// 描画する
	m_bDisplay = true;

	// 外に出ていない場合
	if (!pDesk->GetOutsideDesk()->GetGoOutside())
	{
		// 開錠時のテクスチャ
		SetTexture(ReceptionUI::OPEN_Texture);

		// セリフUIの更新処理
		m_pLineUI->Update();

	}
	//else if (!pDesk->GetOutsideDesk()->GetTaskNow())
	//{
	//	// 閉錠時のテクスチャ
	//	SetTexture(ReceptionUI::CLOSE_Texture);
	//}

	// イージング
	EasingSine();

	// 親クラスの更新処理
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CReceptionUI::Draw(void)
{
	// セリフUIの描画処理
	m_pLineUI->Draw();

	if (m_bDisplay) CObject2D::Draw();
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CReceptionUI::CollisionSphere(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pSphereCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(), pOther);
}
//=========================================================
// イージングサイン使用関数
//=========================================================
void CReceptionUI::EasingSine(void)
{
	// 位置の取得
	auto pos = GetPos();

	// 初期の大きさ
	D3DXVECTOR2 Apper = { ReceptionUI::Apper.x * pos.x, ReceptionUI::Apper.y * pos.y };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { ReceptionUI::Dest.x * pos.x,ReceptionUI::Dest.y * pos.y };

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