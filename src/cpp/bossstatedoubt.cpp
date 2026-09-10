//=========================================================
//
// 社長がプレイヤーを疑っている状態のクラス [ bossstatedoubt.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "bossstatedoubt.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boss.h"
#include "manager.h"
#include "bossstateneutral.h"
#include "billboard.h"
#include "bossstatechase.h"

//=========================================================
// コンストラクタ
//=========================================================
CBossStateDoubt::CBossStateDoubt() : CBossStateBase(),
m_nDoubtCount(0)
{
	// IDセット
	SetID(ID_DOUBT);
}
//=========================================================
// デストラクタ
//=========================================================
CBossStateDoubt::~CBossStateDoubt()
{

}
//=========================================================
// 開始関数
//=========================================================
void CBossStateDoubt::OnStart(void)
{

}
//=========================================================
// 更新関数
//=========================================================
void CBossStateDoubt::OnUpdate(void)
{
	// ゲージチェック
	CEnemyDoubtGauge* pGauge = m_pBoss->GetGauge();
	if (!pGauge) return;

	// フラグ変更
	pGauge->SetNormal(false);

	// 描画フラグoff
	const auto& icon = m_pBoss->GetChaseIcon();
	if (icon)
		icon->SetDrawFlags(false);

	// もし視界内に入っていたら
	if (m_pBoss->CheckRayToAngleRange())
	{
		// カウントを加算
		m_nDoubtCount++;

		// ui表示(はてなマーク)
		pGauge->SetIsDraw(true);
		pGauge->SetUpGauge(true);

		// 疑いモーションセット
		m_pBoss->GetMotion()->SetMotion(CBoss::MOTION::DOUBT, true, 3);
	}
	else
	{
		// ゲージのクリア
		pGauge->SetUpGauge(false);
		pGauge->SetRatio(0.0010f);

		// もし完全クリアなら状態を元に戻す
		if (pGauge->GetNormalFlag())
		{
			m_pBoss->ChangeState(new CBossStateNeutral(), ID_NEUTRAL);
			return;
		}
	}

	// もし上限値を超えていたら
	if (m_nDoubtCount >= Config::MAX_DOUBT_COUNT && pGauge->GetIsComplete())
	{
		// 猛追ステートに変更する
		m_pBoss->ChangeState(new CBossStateChase(), ID_CHASE);
		return;
	}
}
//=========================================================
// 終了関数
//=========================================================
void CBossStateDoubt::OnExit(void)
{
	// カウントリセット
	m_nDoubtCount = 0;
}