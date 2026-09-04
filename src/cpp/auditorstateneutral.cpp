//=========================================================
//
// 監査人の通常状態クラス [ auditorstateneutral.cpp ]
// Author : Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditorstateneutral.h"
#include "auditorstatedoubt.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "auditor.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CAuditorStateNeutral::CAuditorStateNeutral()
{
	SetID(ID_NEUTRAL);
}
//=========================================================
// デストラクタ
//=========================================================
CAuditorStateNeutral::~CAuditorStateNeutral()
{

}
//=========================================================
// 状態開始
//=========================================================
void CAuditorStateNeutral::OnStart()
{
	// モーションセット
	m_pAuditor->GetMotion()->SetMotion(CAuditor::MOTION::NEUTRAL, true, 3);
}
//=========================================================
// 状態更新
//=========================================================
void CAuditorStateNeutral::OnUpdate()
{
	// nullなら
	if (!m_pAuditor) return;

	// 描画フラグoff
	const auto& icon = m_pAuditor->GetChaseIcon();
	if (icon)
		icon->SetDrawFlags(false);

	// もし自身の視界内に入っていたら
	if (m_pAuditor->CheckRayToAngleRange())
	{
		// 疑いステートに変更
		m_pAuditor->ChangeState(new CAuditorStateDoubt(), ID_DOUBT);
		return;
	}

	// 基本の通常移動
	m_pAuditor->MovingTypeOutSide();
}
//=========================================================
// 状態終了
//=========================================================
void CAuditorStateNeutral::OnExit()
{

}