//=========================================================
//
// ベンチさぼり状態クラス処理 [ playerstatebench.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstatebench.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateBench::CPlayerStateBench()
{
	SetID(ID_BENCH);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateBench::~CPlayerStateBench()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateBench::OnStart()
{
	// プレイヤーの現在座標をベンチの上にセットする
	m_pPlayer->MathBenchRotation();
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateBench::OnUpdate()
{
	// モーションが違ったら
	if (m_pPlayer->BENCH != m_pPlayer->GetMotion()->GetMotionType())
	{
		// モーション変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::BENCH, true, 3);
	}

	if (!m_pPlayer->GetAfkBench(0) && !m_pPlayer->GetAfkBench(1)
		&& !m_pPlayer->GetAfkBench(2) && !m_pPlayer->GetAfkBench(3))
	{
		// ステートを移動にチェンジ
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}

//=========================================================
// 状態終了
//=========================================================
void CPlayerStateBench::OnExit()
{
	// 固定の座標にもどる
	m_pPlayer->SetPos(m_pPlayer->GetPosOld());
}
