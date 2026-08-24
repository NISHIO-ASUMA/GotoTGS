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

//=========================================================
// コンストラクタ
//=========================================================
CEnemyStateDoubt::CEnemyStateDoubt() : CEnemyStateBase(),
m_nDoubtCount(0),
m_pGauge(nullptr)
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
	// ui生成 ( ？のゲージ )
	auto CreatePos = D3DXVECTOR3(m_pEnemy->GetPos().x, m_pEnemy->GetPos().y + Config::VALUE_HEIGHT, m_pEnemy->GetPos().z);
	m_pGauge = CEnemyDoubtGauge::Create(CreatePos, Config::SIZE, Config::SIZE, "hatena.png", "gauge_enemyside.png");
}
//=========================================================
// 更新関数
//=========================================================
void CEnemyStateDoubt::OnUpdate(void)
{
	// 頭上のゲージの位置の更新
	if (m_pGauge && m_pEnemy)
	{
		D3DXVECTOR3 headPos = m_pEnemy->GetPos();
		headPos.y += Config::VALUE_HEIGHT;
		m_pGauge->SetTargetPos(headPos);
	}

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
		m_pGauge->SetIsDraw(true);
		m_pGauge->SetUpGauge(true);

		// 疑いモーションセット
		m_pEnemy->GetMotion()->SetMotion(CEnemy::MOTION::DOUBT, true, 3);

		// この時にレベルを徐徐に加算する
		m_pEnemy->AddLevel(0.2f);
	}
	else
	{
		// ゲージのクリア
		m_pGauge->SetUpGauge(false);
		m_pGauge->SetRatio(0.0020f);

		// もし完全クリアなら状態を元に戻す
		if (m_pGauge->GetNormalFlag())
		{
			m_pEnemy->ChangeState(new CEnemyStateNeutral(), ID_NEUTRAL);
			return;
		}
	}

	// もし上限値を超えていたら
	if (m_nDoubtCount >= Config::MAX_DOUBT_COUNT && m_pGauge->GetIsComplete())
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

	// uiを破棄する
	m_pGauge->Uninit();
}