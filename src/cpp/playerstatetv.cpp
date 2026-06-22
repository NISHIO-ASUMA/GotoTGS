//=========================================================
//
// TV見ているサボりの状態クラス処理 [ playerstatetv.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "playerstatetv.h"
#include "playerstateneutral.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateTV::CPlayerStateTV()
{
	SetID(ID_TV);
}
//=========================================================
// デストラクタ
//=========================================================
CPlayerStateTV::~CPlayerStateTV()
{

}
//=========================================================
// 開始関数
//=========================================================
void CPlayerStateTV::OnStart(void)
{
	// 情報取得
	auto Pos = m_pPlayer->GetPos();

	// 座標を保存しておく
	m_pPlayer->SetPrevPos(Pos);

	// プレイヤーの現在座標を椅子の上にセットする
	m_pPlayer->MathTVRotation();
}
//=========================================================
// 更新関数
//=========================================================
void CPlayerStateTV::OnUpdate(void)
{
	// モーションが違ったら
	if (m_pPlayer->TV != m_pPlayer->GetMotion()->GetMotionType())
	{
		// モーション変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION::TV, true, 5);
	}

	if (!m_pPlayer->GetAfkTV())
	{
		// 状態遷移
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}
//=========================================================
// 終了関数
//=========================================================
void CPlayerStateTV::OnExit(void)
{
	// 保存していた座標にもどる
	m_pPlayer->SetPos(m_pPlayer->GetPrevPos());
}