//=========================================================
//
// プレイヤーステートディッチクラス処理 [ playerstateditch.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstatesmoke.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateSmoke::CPlayerStateSmoke()
{
	SetID(ID_SMOKE);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateSmoke::~CPlayerStateSmoke()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateSmoke::OnStart()
{
	// プレイヤーの手に煙草を持たせる
	m_pPlayer->AddItemSet
	(
		"data/MODEL/STAGEOBJ/smoke.x",	// 対象モデルファイル
		CModel::PARTTYPE_RIGHT_HAND,	// 持たせる場所
		D3DXVECTOR3(0.0f,0.0f,1.57f),
		D3DXVECTOR3(-3.0f, 0.0f,3.0f)
	);

	// モーション変更
	m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::SMOKE);
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateSmoke::OnUpdate()
{
	if (!m_pPlayer->GetAfkSmoke())
	{
		// ステートを移動にチェンジ
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}

//=========================================================
// 状態終了
//=========================================================
void CPlayerStateSmoke::OnExit()
{
	m_pPlayer->DeleteItem();
}