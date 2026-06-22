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
	constexpr const char* MODEL_NAME = "STAGEOBJ/automaticdoor.x"; // ファイル名
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
	// プレイヤー取得
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();
	if (Player == nullptr) return;

	// カメラ取得
	const auto& Camera = CManager::GetInstance()->GetCamera();

	// 判定式生成
	for (auto Door : m_pSideOpenDoors)
	{
		// カメラの透過条件に入っているかチェック
		if (Camera->CollisionToraySide(Player, Door))
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