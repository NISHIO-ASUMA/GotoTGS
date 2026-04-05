//=========================================================
//
// 世界の壁を複数管理するクラス [ worldwallmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "worldwallmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "jsonmanager.h"
#include "gamewallmodel.h"

//=========================================================
// コンストラクタ
//=========================================================
CWorldWallManager::CWorldWallManager() : m_pWall{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CWorldWallManager::~CWorldWallManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorldWallManager::Init(const char* pLoadName)
{
	// 配列初期化
	m_pWall.clear();

	// jsonマネージャーから読み込み
	auto JsonManager = CManager::GetInstance()->GetJsonManager();
	JsonManager->Load(pLoadName);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWorldWallManager::Uninit(void)
{
	// 配列クリア
	m_pWall.clear();
}
//=========================================================
// 生成処理
//=========================================================
CGameWallModel* CWorldWallManager::CreateManager
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const D3DXVECTOR3& scale, 
	const char* pModelName
)
{
	// インスタンス生成
	CGameWallModel* pWall = CGameWallModel::Create(pos, rot, scale, pModelName);
	if (pWall) m_pWall.push_back(pWall);

	return pWall;
}