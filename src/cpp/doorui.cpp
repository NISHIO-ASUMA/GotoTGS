//=========================================================
//
// AFK2DUI処理 [ afk2dui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "doorui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "gamesceneobject.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "easing.h"
#include "player.h"

//=================================================
// 名前空間
//=================================================
namespace DoorUI
{
	const D3DXVECTOR3 Pos = { 640.0f, 640.0f, 0.0f };		// ドアUIの座標
	const D3DXVECTOR3 DoorPos = { 482.0f, 0.0f,132.0f };		// ドアの座標
	const D3DXVECTOR2 Apper = { 0.15f, 0.05f };				// 初期のサイズ
	const D3DXVECTOR2 Dest = { 0.25f, 0.1f };				// 目標のサイズ
	constexpr float fRadius = 25.0f;						// 半径
	constexpr float fWidth = 250.0f;						// 横幅
	constexpr float fHeight = 75.0f;						// 縦幅
	constexpr float fMaxFrame = 60.0f;						// マックスフレーム
	constexpr const char* DoorUI_NAME = "dooropen.png";		// ドアUIのテクスチャ名
};

//=================================================
// 静的メンバ変数
//=================================================
CDoorUI* CDoorUI::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CDoorUI::CDoorUI() : m_pSphereCollider(nullptr), 
m_pos(VECTOR3_NULL),
m_fMaxFrame(NULL),
m_fCountFrame(NULL),
m_bEasing(false),
m_bDisplay(false),
m_bUse(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CDoorUI::~CDoorUI()
{

}
//=========================================================
// 生成処理
//=========================================================
CDoorUI* CDoorUI::Create(void)
{
	// インスタンス生成
	CDoorUI* pDoorUI = new CDoorUI;
	if (pDoorUI == nullptr) return nullptr;

	// オブジェクトセット
	pDoorUI->SetPos(DoorUI::Pos);
	pDoorUI->SetAnchor(ANCHORTYPE_CENTER);
	pDoorUI->SetTexture(DoorUI::DoorUI_NAME);
	pDoorUI->SetSize(DoorUI::fWidth, DoorUI::fHeight);

	// 初期化失敗時
	if (FAILED(pDoorUI->Init())) return nullptr;

	// UIのポインタを返す
	return pDoorUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDoorUI::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// イージングの設定値
	m_fCountFrame = NULL;
	m_fMaxFrame = DoorUI::fMaxFrame;

	// 球形コライダーを生成
	m_pSphereCollider = CSphereCollider::Create(DoorUI::DoorPos, DoorUI::fRadius);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CDoorUI::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();

	// スフィアコライダーの破棄
	m_pSphereCollider.reset();
}
//=========================================================
// 更新処理
//=========================================================
void CDoorUI::Update(void)
{
	if (m_bUse)
	{
		Uninit();
		return;
	}

	// 親クラスの更新処理
	CObject2D::Update();

	// プレイヤーの情報を取得し判定を生成
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();
	if (Player == nullptr) return;

	// スフィアコライダー取得とnullチェック
	CSphereCollider* SphereCollider = Player->GetSphereCollider();
	if (SphereCollider == nullptr) return;

	// 当たり判定の実行
	if (CollisionSphere(SphereCollider))m_bDisplay = true;
	else m_bDisplay = false;

	// イージング
	EasingSine();
}
//=========================================================
// 描画処理
//=========================================================
void CDoorUI::Draw(void)
{
	if(m_bDisplay) CObject2D::Draw();
}
//=========================================================
// インスタンス取得処理
//=========================================================
CDoorUI* CDoorUI::Instance(void)
{
	// nullチェック
	if (m_pInstance == nullptr)m_pInstance = new CDoorUI;

	// 生成されたインスタンスを返す
	return m_pInstance;
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CDoorUI::CollisionSphere(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pSphereCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(), pOther);
}
//=========================================================
// イージングサイン使用関数
//=========================================================
void CDoorUI::EasingSine(void)
{
	// 位置の取得
	auto pos = GetPos();

	// 初期の大きさ
	D3DXVECTOR2 Apper = { DoorUI::Apper.x * pos.x, DoorUI::Apper.y * pos.y };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { DoorUI::Dest.x * pos.x,DoorUI::Dest.y * pos.y };

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