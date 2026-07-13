//=========================================================
//
// キー入力で開く両開きドアの管理クラス [ sideopendoormanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "slideopendoormanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "camera.h"
#include "gamesceneobject.h"
#include "player.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SIDEMANAGER
{
	constexpr const char* SIDEMODELNAME = "STAGEOBJ/Door.x";				// 両開きドアのファイル名
	constexpr const char* TOILEMODELNAME = "STAGEOBJ/Man_toilet_door.x";	// トイレのドアのファイル名
};

//=========================================================
// コンストラクタ
//=========================================================
CSideOpenDoorManager::CSideOpenDoorManager() : m_pSideOpenDoors{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CSideOpenDoorManager::~CSideOpenDoorManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSideOpenDoorManager::Init(void)
{
	// 配列初期化
	m_pSideOpenDoors.clear();

	// 開くドアを配列に追加する
	m_pSideOpenDoors.push_back(CSideOpenDoor::Create
								(D3DXVECTOR3(482.0f,34.0f,132.0f),
								(D3DXVECTOR3(0.0f,-1.57f,0.0f)),
								(D3DXVECTOR3(1.25f, 1.0f,1.8f)),
								SIDEMANAGER::SIDEMODELNAME,
								CSideOpenDoor::OPENTYPE_LEFT));

	m_pSideOpenDoors.push_back(CSideOpenDoor::Create
								(D3DXVECTOR3(482.0f, 34.0f, 88.0f),
								(D3DXVECTOR3(0.0f,1.57f,0.0f)),
								(D3DXVECTOR3(1.25f, 1.0f, 1.8f)),
								SIDEMANAGER::SIDEMODELNAME,
								CSideOpenDoor::OPENTYPE_RIGHT));

	// トイレのドア
	m_pSideOpenDoors.push_back(CSideOpenDoor::Create
								(D3DXVECTOR3(609.0f, 49.0f, -122.0f),
								(D3DXVECTOR3(0.0f, 3.14f, 0.0f)),
								(D3DXVECTOR3(3.0f, 3.0f, 1.0f)),
								SIDEMANAGER::TOILEMODELNAME,
								CSideOpenDoor::OPENTYPE_RIGHT));

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSideOpenDoorManager::Uninit(void)
{
	// 配列クリア
	m_pSideOpenDoors.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CSideOpenDoorManager::Update(void)
{
	// カメラ取得
	const auto& Camera = CManager::GetInstance()->GetCamera();
	if (Camera == nullptr) return;

	// 判定式生成
	for (auto Door : m_pSideOpenDoors)
	{
		// カメラの透過条件に入っているかチェック
		if (Camera->CollisionToraySide(Door))
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
// ドアを開く命令を飛ばす処理
//=========================================================
void CSideOpenDoorManager::OpenSideDoor(const std::vector<int>& targetIndices)
{
	// 全体のドアの最大数を取得
	int nTotalDoors = GetAll();

	// 渡されたターゲットの配列をループで回す
	for (int nIdx : targetIndices)
	{
		// nullチェック
		if (nIdx >= 0 && nIdx < nTotalDoors && m_pSideOpenDoors[nIdx] != nullptr)
		{
			// 開く命令を実行
			m_pSideOpenDoors[nIdx]->RotationDoorFlag();
		}
	}
}