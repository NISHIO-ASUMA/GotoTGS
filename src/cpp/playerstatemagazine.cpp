//=========================================================
//
// 雑誌読みサボり状態クラス処理 [ playerstatemagazine.cpp ]
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
	SetID(ID_MAGAZINE);
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
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateMagazine::OnUpdate()
{
	// モーションが違ったら
	if (m_pPlayer->MAGAZINE != m_pPlayer->GetMotion()->GetMotionType())
	{
		// モーション変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::MAGAZINE);
	}

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
