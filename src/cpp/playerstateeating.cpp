//=========================================================
//
// ゲームセンターサボり状態クラス処理 [ playerstategamecenter.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstateeating.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// 名前空間
//=========================================================
namespace AFKEATING
{
	const D3DXVECTOR3 PosDest = { -195.0f, 14.0, -202.0f };	// 目的の位置
}

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateEating::CPlayerStateEating()
{
	SetID(ID_FOOD);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateEating::~CPlayerStateEating()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateEating::OnStart()
{
	// ゲーム機とプレイヤーの距離の差
	D3DXVECTOR3 diff = m_pPlayer->GetPos() - AFKEATING::PosDest;

	// 角度
	float fAngle = atan2f(diff.x, diff.z);

	// 角度の設定
	m_pPlayer->SetRotDest(D3DXVECTOR3(NULL, fAngle, NULL));

}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateEating::OnUpdate()
{
	// モーションが違ったら
	if (m_pPlayer->FOOD != m_pPlayer->GetMotion()->GetMotionType())
	{
		// モーション変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::FOOD,true,3);
	}

	// 近田のバグがいい感じになっていたからこのままいってみよう
	if (!m_pPlayer->GetAfkGameCenter())
	{
		// ステートを移動にチェンジ
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}

//=========================================================
// 状態終了
//=========================================================
void CPlayerStateEating::OnExit()
{

}
