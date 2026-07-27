//=========================================================
//
// 敵の複数管理クラス [ enemymanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemymanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "jsonmanager.h"
#include "manager.h"
#include "gametime.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace ENEMY_MANAGER
{
	constexpr const char* FILE = "data/Json/GameEnemy.json"; // 外部ファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CEnemyManager::CEnemyManager() : m_pEnemys{},
m_nStageCount(NULL),
m_nIntervalCount(-1)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemyManager::~CEnemyManager()
{
	Uninit();
}
//=========================================================
// 実際の生成処理
//=========================================================
CEnemy* CEnemyManager::CreateManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CEnemy* pEnemy = CEnemy::Create(pos,rot);

	// 動的配列内追加
	if (pEnemy)
	{
		m_pEnemys.push_back(pEnemy);
	}

	// デフォルトで返す
	return pEnemy;
}
//=========================================================
// 外部ファイル設定
//=========================================================
void CEnemyManager::LoadJson(void)
{
	// jsonmanagerを取得
	const auto& json = CManager::GetInstance()->GetJsonManager();

	// 敵を実際に読み込む
	json->SetEnemyManager(GetInstance());
	json->Load(ENEMY_MANAGER::FILE);
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemyManager::Init(void)
{
	// 配列の切り離し
	m_pEnemys.clear();

	// 外部ファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemyManager::Uninit(void)
{
	// 配列クリア
	m_pEnemys.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemyManager::Update(void)
{
	// nullチェック
	if (!m_pTimeContainer) return;

	// 最大時間を取得
	int nAllTime = m_pTimeContainer->GetAllTime();

	// NOTE : 現在は一旦20秒に一体追加で行こうかな
	if (nAllTime > 0 && (nAllTime % 60) == 0 && nAllTime != m_nIntervalCount && nAllTime != CGametime::Config::NUMTIME)
	{
		// 敵を生成する ( ここの座標は後々変更 )
		AddEnemy(VECTOR3_NULL);

		// 生成時間を変更して、このフレームでは一回のみ入るようにする
		m_nIntervalCount = nAllTime;
	}
}
//=========================================================
// 敵の生成追加関数
//=========================================================
void CEnemyManager::AddEnemy(const D3DXVECTOR3& pos)
{
	// 新規追加生成
	m_pEnemys.push_back(CEnemy::Create(pos, VECTOR3_NULL));
}