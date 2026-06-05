//=========================================================
//
// 自動ドア管理処理 [ automaticdoormanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "automaticdoormanager.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "gamesceneobject.h"
#include "spherecollider.h"
#include "collisionsphere.h"

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoorManager::CAutoMaticDoorManager() : m_pSphereCollider(nullptr),
m_pAutoDoors{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CAutoMaticDoorManager::~CAutoMaticDoorManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAutoMaticDoorManager::Init(void)
{
	// 配列のクリア
	m_pAutoDoors.clear();

	// 自動ドアの生成
	m_pAutoDoors.push_back(
		CAutoMaticDoor::Create(D3DXVECTOR3(645.0f, 34.0f, 132.0f),
							   D3DXVECTOR3(0.0f, 1.57f, 0.0f), 
							   INITSCALE, 
							   "STAGEOBJ/automaticdoor.x", 
								CAutoMaticDoor::MOVETYPE_LEFT)
						  );

	m_pAutoDoors.push_back(
		CAutoMaticDoor::Create(D3DXVECTOR3(645.0f, 34.0f, 90.5f),
								D3DXVECTOR3(0.0f, 1.57f, 0.0f),
								INITSCALE,
								"STAGEOBJ/automaticdoor.x",
								CAutoMaticDoor::MOVETYPE_RIGHT)
								);
	
	// 球コライダー生成
	m_pSphereCollider = CSphereCollider::Create(D3DXVECTOR3(645.0f, 40.0f, 105.0f), 30.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAutoMaticDoorManager::Uninit(void)
{
	// 配列のクリア
	m_pAutoDoors.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CAutoMaticDoorManager::Update(void)
{
	// プレイヤー取得
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();
	if (Player == nullptr) return;

	// カメラ取得
	const auto& Camera = CManager::GetInstance()->GetCamera();

	// 判定式生成
	for (auto Door : m_pAutoDoors)
	{
		// カメラの透過条件に入っているかチェック
		if (Camera->CollisionTorayDoor(Player, Door))
		{
			// 対象モデルの透明度を設定する
			Door->SetCol(D3DCOLORVALUE(1.0f, 1.0f, 1.0f, 0.3f));
			Door->SetZEneble(true);
		}
		else
		{
			// デフォルトのカラーを設定
			Door->SetCol();
			Door->SetZEneble(false);
		}
	}
}
//=========================================================
// 球の当たり判定
//=========================================================
bool CAutoMaticDoorManager::CollisionSphere(CSphereCollider* pOther)
{
	// ポインタがnull
	if (!m_pSphereCollider) return false;

	// 球同士の当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(),pOther);
}