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
// 名前空間
//=========================================================
namespace AFKGAMECENTER
{
	const D3DXVECTOR3 PosDest = { 1705.0f, 0.0f, 166.0f };	// ゲーム機の位置
}

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateGameCenter::CPlayerStateGameCenter()
{
	SetID(ID_GAMECENTER);
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
	// ゲーム機とプレイヤーの距離の差
	D3DXVECTOR3 diff = m_pPlayer->GetPos() - AFKGAMECENTER::PosDest;

	// 角度
	float fAngle = atan2f(diff.x, diff.z);

	// 角度の設定
	m_pPlayer->SetRotDest(D3DXVECTOR3(NULL,fAngle,NULL));
	
	// モーション変更
	m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::GAME);
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateGameCenter::OnUpdate()
{
	if (!m_pPlayer->GetAfkGameCenter())
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
