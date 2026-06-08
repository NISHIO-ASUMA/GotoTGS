//=========================================================
//
// AFK処理 [ afk.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afk.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "collisionbox.h"
#include "gamesceneobject.h"
#include "player.h"

namespace AFK
{
	
}

//=========================================================
// コンストラクタ
//=========================================================
CAfk::CAfk(): m_pSphereCollider(nullptr),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_Size(VECTOR3_NULL),
m_fRadius(NULL),
m_Collison(COLLISION::SPHERE),
m_bAfk(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfk::~CAfk()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfk::Init(const D3DXVECTOR3& pos,const float& fRadius, const COLLISION& collison)
{
	// 座標と半径の設定
	SetPos(pos);
	SetRadius(fRadius);
	SetCollision(collison);
	D3DXMATRIX matRot;

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(m_pos, m_posOld, m_Size, matRot);

	// 球形コライダーを生成
	m_pSphereCollider = CSphereCollider::Create(m_pos, m_fRadius);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfk::Uninit(void)
{
	// ボックスコライダーの破棄
	m_pBoxCollider.reset();

	// スフィアコライダーの破棄
	m_pSphereCollider.reset();
}
//=========================================================
// 更新処理
//=========================================================
void CAfk::Update(void)
{
	// プレイヤーの情報を取得し判定を生成
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();
	if (Player == nullptr) return;

	if (m_Collison == COLLISION::SPHERE)
	{
		// スフィアコライダー取得とnullチェック
		CSphereCollider* SphereCollider = Player->GetSphereCollider();
		if (SphereCollider == nullptr) return;

		// 当たり判定の実行
		if (CollisionSphere(SphereCollider))m_bAfk = true;
		else m_bAfk = false;
	}

	if (m_Collison == COLLISION::BOX)
	{
		// ボックスコライダー取得とnullチェック
		CBoxCollider* BoxCollider = Player->GetBoxCollider();
		if (BoxCollider == nullptr) return;

		// 当たり判定の実行
		if (CollisionBox(BoxCollider, &m_pos))m_bAfk = true;
		else m_bAfk = false;
	}
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CAfk::CollisionSphere(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pSphereCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(), pOther);
}
//=========================================================
// 当たり判定
//=========================================================
bool CAfk::CollisionBox(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::CollisionEx(m_pBoxCollider.get(), pOther, OutPos);
}