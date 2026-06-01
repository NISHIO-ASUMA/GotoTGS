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
CAfk::CAfk(): m_pCollider(nullptr),
m_pos(VECTOR3_NULL),
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
HRESULT CAfk::Init(void)
{
	// 球形コライダーを生成
	m_pCollider = CSphereCollider::Create(m_pos, m_fRadius);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfk::Uninit(void)
{
	// スフィアコライダーの破棄
	m_pCollider.reset();

}
//=========================================================
// 更新処理
//=========================================================
void CAfk::Update(void)
{
	// プレイヤーの情報を取得し判定を生成
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();

	// コライダー取得とnullチェック
	CSphereCollider* Collider = Player->GetSphereCollider();
	if (Collider == nullptr) return;

	// 当たり判定の実行
	if (Collision(Collider))m_bAfk = true;
	else m_bAfk = false;
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CAfk::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pCollider.get(), pOther);
}