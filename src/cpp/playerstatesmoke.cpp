//=========================================================
//
// 煙草のサボり状態管理クラス処理 [ playerstatesmoke.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstatesmoke.h"
#include "playerstateneutral.h"
#include "player.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace STATE_SMOKE
{
	const D3DXVECTOR3 PosDest = { 321.0f, 50.0f, 320.0f };	// たばこのごみ箱の位置
}

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

	// たばこごみ箱とプレイヤーの距離の差
	D3DXVECTOR3 diff = m_pPlayer->GetPos() - STATE_SMOKE::PosDest;

	// 角度
	float fAngle = atan2f(diff.x, diff.z);

	// 角度の設定
	m_pPlayer->SetRotDest(D3DXVECTOR3(NULL, fAngle, NULL));
}

//=========================================================
// 状態更新
//=========================================================
void CPlayerStateSmoke::OnUpdate()
{
	// モーションが違ったら
	if (m_pPlayer->SMOKE != m_pPlayer->GetMotion()->GetMotionType())
	{
		// モーション変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::SMOKE);
	}

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