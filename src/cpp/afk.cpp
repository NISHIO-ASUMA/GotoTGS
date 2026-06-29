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
#include "spherecollider.h"
#include "collisionsphere.h"
#include "gamesceneobject.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CAfk::CAfk(): m_pSphereCollider(nullptr),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_Size(VECTOR3_NULL),
m_fRadius(NULL),
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
HRESULT CAfk::Init(const D3DXVECTOR3& pos,const float& fRadius)
{
	// 座標と半径の設定
	SetPos(pos);
	SetRadius(fRadius);

	// 球形コライダーを生成
	m_pSphereCollider = CSphereCollider::Create(m_pos, m_fRadius);
	
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfk::Uninit(void)
{
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

	// スフィアコライダー取得とnullチェック
	CSphereCollider* SphereCollider = Player->GetSphereCollider();
	if (SphereCollider == nullptr) return;

	// 当たり判定の実行
	if (CollisionSphere(SphereCollider))m_bAfk = true;
	else m_bAfk = false;
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