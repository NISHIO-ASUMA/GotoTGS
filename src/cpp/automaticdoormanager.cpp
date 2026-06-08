//=========================================================
//
// 自動ドア管理処理 [ automaticdoormanager.cpp ]
// Author: Asuma Nishio
// 
// TODO : 当たり判定の処理をメンバ変数のクラスに持たせるように変更する 
// or 世界のコライダー判別クラスに入れる
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

//*********************************************************
// 定数名前空間
//*********************************************************
namespace AUTO_DOOR
{
	constexpr const char* MODEL_NAME = "STAGEOBJ/automaticdoor.x"; // ファイル名
	const D3DXVECTOR3 ROT = { 0.0f,1.57f,0.0f };				   // 配置角度
};

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoorManager::CAutoMaticDoorManager() : m_pAutoDoors{}
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

	// 自動ドアの生成 ( オフィス側 )
	m_pAutoDoors.push_back(
		CAutoMaticDoor::Create(D3DXVECTOR3(645.0f, 34.0f, 132.0f),
							   AUTO_DOOR::ROT,
							   INITSCALE, 
							   AUTO_DOOR::MODEL_NAME,
								CAutoMaticDoor::MOVETYPE_LEFT)
						  );

	m_pAutoDoors.push_back(
		CAutoMaticDoor::Create(D3DXVECTOR3(645.0f, 34.0f, 90.5f),
								AUTO_DOOR::ROT,
								INITSCALE,
								AUTO_DOOR::MODEL_NAME,
								CAutoMaticDoor::MOVETYPE_RIGHT)
								);
	
#if 0
	// 自動ドアの生成 ( ゲームセンター側 )
	m_pAutoDoors.push_back(
		CAutoMaticDoor::Create(D3DXVECTOR3(645.0f, 34.0f, 132.0f),
			AUTO_DOOR::ROT,
			INITSCALE,
			AUTO_DOOR::MODEL_NAME,
			CAutoMaticDoor::MOVETYPE_LEFT)
	);
#endif

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
