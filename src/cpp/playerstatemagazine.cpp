//=========================================================
//
// プレイヤーステートアクションクラス処理 [ playerstateaction.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstatemagazine.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateMagazine::CPlayerStateMagazine()
{
	//SetID(ID_ACTION);
}

//=========================================================
// デストラクタ
//=========================================================
CPlayerStateMagazine::~CPlayerStateMagazine()
{

}

//=========================================================
// 状態開始
//=========================================================
void CPlayerStateMagazine::OnStart()
{
	// プレイヤーの手に雑誌を持たせる
	m_pPlayer->AddItemSet
	(
		"data/MODEL/STAGEOBJ/magaziner_002.x", // 対象モデルファイル
		CModel::PARTTYPE_LEFT_HAND,			   // 持たせる場所
		D3DXVECTOR3(-1.57f, 0.0f, 0.0f),
		D3DXVECTOR3(5.0f, 0.0f, -7.0f)
	);

	m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::MAGAZINE);

}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateMagazine::OnUpdate()
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
void CPlayerStateMagazine::OnExit()
{
	m_pPlayer->DeleteItem();
}