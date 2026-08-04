//=========================================================
//
// プレイヤーを追いかける状態のクラス[ enemystatechase.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemystatechase.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "enemystateneutral.h"
#include "player.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyStateChase::CEnemyStateChase() : CEnemyStateBase(),
m_nStayCount(NULL),
m_isDoubt(false)
{
	SetID(ID_CHASE);
}
//=========================================================
// デストラクタ
//=========================================================
CEnemyStateChase::~CEnemyStateChase()
{
	
}
//=========================================================
// 開始関数
//=========================================================
void CEnemyStateChase::OnStart(void)
{
	// まずレベルを1上げる
	m_pEnemy->SetMoveSpeed(1);
	m_pEnemy->SetEyeAngle(1);
	m_pEnemy->StartChase(true);

	m_nStayCount = 0;
	m_isDoubt = false;
}
//=========================================================
// 更新関数
//=========================================================
void CEnemyStateChase::OnUpdate(void)
{// 「プレイヤーがタスクを起動したら追いかけを終了する」

	// プレイヤー取得
	CPlayer* pPlayer = m_pEnemy->GetInCharactor();
	if (!pPlayer) return;

	// タスク起動後の停止期間
	if (m_isDoubt)
	{
		// 滞在カウントを加算
		m_nStayCount++;

		// アイコンオフ
		m_pEnemy->GetChaseIcon()->SetDrawFlags(false);

		// 2秒間経過したらニュートラルへ遷移
		if (m_nStayCount >= 120)
		{
			m_pEnemy->ChangeState(new CEnemyStateNeutral(), ID_NEUTRAL);
			m_pEnemy->StartChase(false);
		}
		return; // 停止維持
	}

	// 追跡中の処理
	if (pPlayer->IsTaskWorking())
	{
		// タスク起動を検知
		m_isDoubt = true;
		m_nStayCount = 0;

		// モーション変更
		m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::DOUBT);
		m_pEnemy->SetMove(VECTOR3_NULL);

		// アイコンオフ
		m_pEnemy->GetChaseIcon()->SetDrawFlags(false);
	}
	else
	{
		// タスクが起動されるまでは追いかける
		m_pEnemy->ChaseMoving();
	}
}
//=========================================================
// 終了関数
//=========================================================
void CEnemyStateChase::OnExit(void)
{
	// カウント初期化
	m_nStayCount = 0;
	m_isDoubt = false;
}