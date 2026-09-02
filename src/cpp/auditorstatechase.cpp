//=========================================================
//
// 監査人が追いかける状態のクラス[ auditorstatechase.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditorstatechase.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "auditor.h"
#include "auditorstateneutral.h"
#include "player.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CAuditorStateChase::CAuditorStateChase() : CAuditorStateBase(),
m_nStayCount(NULL),
m_isDoubt(false)
{
	SetID(ID_CHASE);
}
//=========================================================
// デストラクタ
//=========================================================
CAuditorStateChase::~CAuditorStateChase()
{

}
//=========================================================
// 開始関数
//=========================================================
void CAuditorStateChase::OnStart(void)
{
	// 追跡状態にする
	m_pAuditor->StartChase(true);

	// 変数初期化
	m_nStayCount = 0;
	m_isDoubt = false;
}
//=========================================================
// 更新関数
//=========================================================
void CAuditorStateChase::OnUpdate(void)
{// 「プレイヤーがタスクを起動したら追いかけを終了する」

	// プレイヤー取得
	CPlayer* pPlayer = m_pAuditor->GetPlayer();
	if (!pPlayer) return;

	// タスク起動後の停止期間
	if (m_isDoubt)
	{
		// 滞在カウントを加算
		m_nStayCount++;

		// アイコンオフ
		m_pAuditor->GetChaseIcon()->SetDrawFlags(false);

		// 2秒間経過したらニュートラルへ遷移
		if (m_nStayCount >= 120)
		{
			m_pAuditor->StartChase(false);	// フラグ初期化
			m_pAuditor->ChangeState(new CAuditorStateNeutral(), ID_NEUTRAL);
			return;
		}
		return; // 停止維持
	}

	// 追跡中の処理
	if (pPlayer->IsTaskWorking() == true)
	{
		// タスク起動を検知
		m_isDoubt = true;
		m_nStayCount = 0;

		// モーション変更
		m_pAuditor->GetMotion()->SetMotion(CAuditor::MOTION::DOUBT,true,3);
		m_pAuditor->SetMove(VECTOR3_NULL);

		// アイコンオフ
		m_pAuditor->GetChaseIcon()->SetDrawFlags(false);
	}
	else
	{
		// タスクが起動されるまでは追いかける
		m_pAuditor->ChaseMove();
	}
}
//=========================================================
// 終了関数
//=========================================================
void CAuditorStateChase::OnExit(void)
{
	// カウント初期化
	m_nStayCount = 0;
	m_isDoubt = false;
}