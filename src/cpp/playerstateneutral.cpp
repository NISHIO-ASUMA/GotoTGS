//=========================================================
//
// プレイヤーステートニュートラルクラス処理 [ playerstateneutral.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstateneutral.h"
#include "playerstatemove.h"
#include "playerstatesmoke.h"
#include "playerstatemagazine.h"
#include "playerstatetv.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateNeutral::CPlayerStateNeutral()
{
	SetID(ID_NEUTRAL);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateNeutral::~CPlayerStateNeutral()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateNeutral::OnStart()
{
	m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL,true,5);
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateNeutral::OnUpdate()
{
	if (m_pPlayer->GetAfkSmoke())
	{
		// ステートを煙草サボりにチェンジ
		m_pPlayer->ChangeState(new CPlayerStateSmoke(), ID_SMOKE);
	}
	else if (m_pPlayer->GetAfkTV())
	{
		// ステートをテレビに変更
		//m_pPlayer->ChangeState(new CPlayerStateTV(), ID_TV);
	}
	else if (m_pPlayer->GetAfkMagazine())
	{
		// ステートを雑誌サボりにチェンジ
		m_pPlayer->ChangeState(new CPlayerStateMagazine(), ID_MAGAZINE);
	}
}

//=========================================================
// 状態終了
//=========================================================
void CPlayerStateNeutral::OnExit()
{
	m_pPlayer->DeleteItem();
}