//=========================================================
//
// プレイヤーを疑っている状態のクラス [ enemystatedoubt.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemystatedoubt.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "manager.h"
#include "enemystateneutral.h"
#include "enemystatechase.h"
#include "billboard.h"
#include "enemymanager.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyStateDoubt::CEnemyStateDoubt() : CEnemyStateBase(),
m_nDoubtCount(0)
{
	// IDセット
	SetID(ID_DOUBT);
}
//=========================================================
// デストラクタ
//=========================================================
CEnemyStateDoubt::~CEnemyStateDoubt()
{
	
}
//=========================================================
// 開始関数
//=========================================================
void CEnemyStateDoubt::OnStart(void)
{
	// 特になし
}
//=========================================================
// 更新関数
//=========================================================
void CEnemyStateDoubt::OnUpdate(void)
{
	// ゲージチェック
	CEnemyDoubtGauge* pGauge = m_pEnemy->GetGauge();
	if (!pGauge) return;

	// フラグ変更
	pGauge->SetNormal(false);

	// フラグoff
	const auto& icon = m_pEnemy->GetChaseIcon();
	if (icon)
		icon->SetDrawFlags(false);

	// もし視界内に入っていたら
	if (m_pEnemy->CheckRayToAngleRange())
	{
		// カウントを加算
		m_nDoubtCount++;

		// ui表示(はてなマーク)
		pGauge->SetIsDraw(true);
		pGauge->SetUpGauge(true);
		
		// 疑いモーションセット
		m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::DOUBT, true, 3);

		// この時にレベルを徐徐に加算する
		m_pEnemy->AddLevel(0.2f);
	}
	else
	{
		// ゲージのクリア
		pGauge->SetUpGauge(false);
		pGauge->SetRatio(0.0010f);

		// もし完全クリアなら状態を元に戻す
		if (pGauge->GetNormalFlag())
		{
			// 通常状態の設定
			m_pEnemy->ChangeState(new CEnemyStateNeutral(), ID_NEUTRAL);

			// ゲージのクリアを挟む
			pGauge->SetRatioZero();
			return;
		}
	}

	// もし上限値を超えていたら
	if (m_nDoubtCount >= Config::MAX_DOUBT_COUNT && pGauge->GetIsComplete())
	{
		// 猛追ステートに変更する
		m_pEnemy->ChangeState(new CEnemyStateChase(), ID_CHASE);
		return;
	}
}
//=========================================================
// 終了関数
//=========================================================
void CEnemyStateDoubt::OnExit(void)
{
	// カウントリセット
	m_nDoubtCount = 0;
}