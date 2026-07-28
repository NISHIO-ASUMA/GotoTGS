//=========================================================
//
// 敵の通常状態クラス [ enemystateneutral.cpp ]
// Author : Asuma Nishio
// 
// TODO : state doubt ( ダウト )
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemystateneutral.h"
#include "enemystateserch.h"
#include "enemystatedoubt.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "manager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyStateNeutral::CEnemyStateNeutral()
{
	SetID(ID_NEUTRAL);
}
//=========================================================
// デストラクタ
//=========================================================
CEnemyStateNeutral::~CEnemyStateNeutral()
{

}
//=========================================================
// 状態開始
//=========================================================
void CEnemyStateNeutral::OnStart()
{
	// モーションセット
	m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::NEUTRAL, true,3);
}
//=========================================================
// 状態更新
//=========================================================
void CEnemyStateNeutral::OnUpdate()
{
	// nullなら
	if (!m_pEnemy) return;

	// もし索敵範囲に入っていたら、疑い状態に変更する
	if (m_pEnemy->CheckEyesight())
	{
		// ステート生成
		m_pEnemy->ChangeState(new CEnemyStateDoubt(),ID_DOUBT);
		return;
	}

	// 通常のポインタ移動をする
	switch (m_pEnemy->GetMoveType())
	{
	case CEnemy::MOVETYPE_NORMAL:
		m_pEnemy->UpdateMoveViewPoint();	// PC辺り
		break;

	case CEnemy::MOVETYPE_SMOKE:
		m_pEnemy->UpdateMovingSmoke();		// 煙草辺り
		break;

	case CEnemy::MOVETYPE_TV:
		m_pEnemy->UpdateMovingTV();			// TV辺り
		break;

	default:
		m_pEnemy->UpdateMoveViewPoint();
		break;
	}
}
//=========================================================
// 状態終了
//=========================================================
void CEnemyStateNeutral::OnExit()
{

}