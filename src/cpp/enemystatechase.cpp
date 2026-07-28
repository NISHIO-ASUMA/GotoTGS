//=========================================================
//
// プレイヤーを追いかける状態のクラス[ enemystatechase.cpp ]
// Author: Asuma Nishio
// ここの関数修正する
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
	m_nStayCount = 0;
	m_isDoubt = false;
}
//=========================================================
// 更新関数
//=========================================================
void CEnemyStateChase::OnUpdate(void)
{
	// すでに視界から外れて停止中の処理
	if (m_isDoubt)
	{
		// 停止時間中にもう一度プレイヤーが視界に入ったかチェック
		if (m_pEnemy->CheckEyesight())
		{
			// Doubt状態を解除して追跡に戻す
			m_isDoubt = false;
			m_nStayCount = 0;

			// 追跡モーションに戻す
			m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::CHASEDASH);
		}
		else
		{
			m_nStayCount++;

			// 2秒間経過したらニュートラルへ遷移
			if (m_nStayCount >= 120)
			{
				// ステートマシン変更
				m_pEnemy->ChangeState(new CEnemyStateNeutral(), ID_NEUTRAL);
				return;
			}

			// 視界外のまま停止している間はこれ以上追跡を行わない
			return;
		}
	}

	// 通常の追跡 
	if (!m_pEnemy->CheckEyesight())
	{
		// フラグ起動
		m_isDoubt = true;
		m_nStayCount = 0; 

		// モーション変更
		m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::DOUBT);
		m_pEnemy->SetMove(VECTOR3_NULL);
	}
	else
	{
		// 視界内にいる間は通常通り追いかける
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