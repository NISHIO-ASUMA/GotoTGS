//=========================================================
//
// モブキャラクター管理クラス [ mobcharactormanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "mobcharactormanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "walkingfriend.h"
#include "jsonmanager.h"
#include "manager.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace MOB_MANAGER
{
	constexpr const char* FILE = "data/Json/WalkingCharactorData.json"; // 外部ファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CMobCharactorManager::CMobCharactorManager() : m_pWalkings{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMobCharactorManager::~CMobCharactorManager()
{
	Uninit();
}
//=========================================================
// 実際の生成処理
//=========================================================
CWalkFriend* CMobCharactorManager::CreateManager
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const char* pMotionName, 
	const int nMoveType, 
	const float fMoveValue
)
{
	// インスタンス生成
	CWalkFriend* pMob = CWalkFriend::Create(pos, rot, pMotionName,nMoveType,fMoveValue);

	// 動的配列内に追加
	if (pMob)
	{
		m_pWalkings.push_back(pMob);
	}

	// デフォルトで返す
	return pMob;
}
//=========================================================
// 外部ファイル設定
//=========================================================
void CMobCharactorManager::LoadJson(void)
{
	// jsonmanagerを取得
	const auto& json = CManager::GetInstance()->GetJsonManager();

	// 敵を実際に読み込む
	json->Load(MOB_MANAGER::FILE);
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMobCharactorManager::Init(void)
{
	// 配列の切り離し
	m_pWalkings.clear();

	// 外部ファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMobCharactorManager::Uninit(void)
{
	// 配列クリア
	m_pWalkings.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CMobCharactorManager::Update(void)
{

}