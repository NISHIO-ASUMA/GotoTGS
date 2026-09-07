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
m_nIntervalCount(-1),
m_pDestCharactorPointer(nullptr)
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
CEnemy* CEnemyManager::CreateManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const CEnemy::MOVETYPE& MoveType)
{
	// インスタンス生成
	CEnemy* pEnemy = CEnemy::Create(pos,rot, MoveType);

	if (pEnemy)
	{
		// 判定をするポインタを設定
		pEnemy->SetCharactorPointer(m_pDestCharactorPointer);

		// 動的確保
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
HRESULT CEnemyManager::Init(CPlayer* pCharactor)
{
	// 配列の切り離し
	m_pEnemys.clear();

	// ポインタセット
	m_pDestCharactorPointer = pCharactor;

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
}
//=========================================================
// 敵のレベルアップ処理
//=========================================================
void CEnemyManager::SetLevleUpper(void)
{
	// 出現している敵だけレベルアップする
	for (int nCnt = 0; nCnt < static_cast<int>(m_pEnemys.size()); nCnt++)
	{
		// 加算
		m_pEnemys[nCnt]->AddLevel(100.0f);
	}
}
//=========================================================
// 敵の生成追加関数
//=========================================================
void CEnemyManager::AddEnemy(const D3DXVECTOR3& pos)
{
	// 敵のポインタを生成
	if (m_pDestCharactorPointer != nullptr)
		m_pEnemys.back()->SetCharactorPointer(m_pDestCharactorPointer);

	// 新規追加生成
	m_pEnemys.push_back(CEnemy::Create(pos, VECTOR3_NULL, CEnemy::MOVETYPE_NORMAL));
}