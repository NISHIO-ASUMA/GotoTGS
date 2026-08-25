//=========================================================
//
// 社長がプレイヤーを追いかける状態のクラス[ bossstatechase.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "bossstatechase.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boss.h"
#include "bossstateneutral.h"
#include "player.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CBossStateChase::CBossStateChase() : CBossStateBase(),
m_nStayCount(NULL),
m_isDoubt(false)
{
	SetID(ID_CHASE);
}
//=========================================================
// デストラクタ
//=========================================================
CBossStateChase::~CBossStateChase()
{

}
//=========================================================
// 開始関数
//=========================================================
void CBossStateChase::OnStart(void)
{
	// 追跡状態にする
	m_pBoss->StartChase(true);

	m_nStayCount = 0;
	m_isDoubt = false;
}
//=========================================================
// 更新関数
//=========================================================
void CBossStateChase::OnUpdate(void)
{// 「プレイヤーがタスクを起動したら追いかけを終了する」

	// プレイヤー取得
	CPlayer* pPlayer = m_pBoss->GetInCharactor();
	if (!pPlayer) return;

	// タスク起動後の停止期間
	if (m_isDoubt)
	{
		// 滞在カウントを加算
		m_nStayCount++;

		// アイコンオフ
		m_pBoss->GetChaseIcon()->SetDrawFlags(false);

		// 2秒間経過したらニュートラルへ遷移
		if (m_nStayCount >= 120)
		{
			m_pBoss->StartChase(false);	// フラグ初期化
			m_pBoss->ChangeState(new CBossStateNeutral(), ID_NEUTRAL);
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
		m_pBoss->GetMotion()->SetMotion(CBoss::MOTION::DOUBT);
		m_pBoss->SetMove(VECTOR3_NULL);

		// アイコンオフ
		m_pBoss->GetChaseIcon()->SetDrawFlags(false);
	}
	else
	{
		// タスクが起動されるまでは追いかける
		m_pBoss->ChasePlayer();
	}
}
//=========================================================
// 終了関数
//=========================================================
void CBossStateChase::OnExit(void)
{
	// カウント初期化
	m_nStayCount = 0;
	m_isDoubt = false;
}