//=========================================================
//
// ゲームセンターサボり状態クラス処理 [ playerstategamecenter.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstategamecenter.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateGameCenter::CPlayerStateGameCenter()
{
	SetID(ID_MAGAZINE);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateGameCenter::~CPlayerStateGameCenter()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateGameCenter::OnStart()
{
	// モーション変更
	m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::MAGAZINE);
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateGameCenter::OnUpdate()
{
	if (!m_pPlayer->GetAfkMagazine())
	{
		// ステートを移動にチェンジ
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}

//=========================================================
// 状態終了
//=========================================================
void CPlayerStateGameCenter::OnExit()
{
	m_pPlayer->DeleteItem();
}
