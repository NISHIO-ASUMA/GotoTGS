//=========================================================
//
// 監査人が疑っている状態のクラス [ auditorstatedoubt.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditorstatedoubt.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "auditor.h"
#include "auditorstateneutral.h"
#include "auditorstatechase.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CAuditorStateDoubt::CAuditorStateDoubt() : CAuditorStateBase(),
m_nDoubtCount(0),
m_pGauge(nullptr)
{
	// IDセット
	SetID(ID_DOUBT);
}
//=========================================================
// デストラクタ
//=========================================================
CAuditorStateDoubt::~CAuditorStateDoubt()
{

}
//=========================================================
// 開始関数
//=========================================================
void CAuditorStateDoubt::OnStart(void)
{
	// ui生成 ( 頭の?ゲージ )
	auto CreatePos = D3DXVECTOR3(m_pAuditor->GetPos().x, m_pAuditor->GetPos().y + Config::VALUE_HEIGHT, m_pAuditor->GetPos().z);
	m_pGauge = CEnemyDoubtGauge::Create(CreatePos, Config::SIZE, Config::SIZE);
}
//=========================================================
// 更新関数
//=========================================================
void CAuditorStateDoubt::OnUpdate(void)
{
	// 頭上のゲージの位置の更新
	if (m_pGauge && m_pAuditor)
	{
		D3DXVECTOR3 headPos = m_pAuditor->GetPos();
		headPos.y += Config::VALUE_HEIGHT;
		m_pGauge->SetTargetPos(headPos);
	}

	// ビルボードの描画フラグoff
	const auto& icon = m_pAuditor->GetChaseIcon();
	if (icon)
		icon->SetDrawFlags(false);

	// もし視界内に入っていたら
	if (m_pAuditor->CheckRayToAngleRange())
	{
		// カウントを加算
		m_nDoubtCount++;

		// ui表示(はてなマーク)
		m_pGauge->SetIsDraw(true);
		m_pGauge->SetUpGauge(true);

		// 疑いモーションセット
		m_pAuditor->GetMotion()->SetMotion(CAuditor::MOTION::DOUBT, true, 3);
	}
	else
	{
		// ゲージのクリア
		m_pGauge->SetUpGauge(false);
		m_pGauge->SetRatio(0.0040f);

		// もし完全クリアなら状態を元に戻す
		if (m_pGauge->GetNormalFlag())
		{
			m_pAuditor->ChangeState(new CAuditorStateNeutral(), ID_NEUTRAL);
			return;
		}
	}

	// もし上限値を超えていたら
	if (m_nDoubtCount >= Config::MAX_DOUBT_COUNT && m_pGauge->GetIsComplete())
	{
		// 猛追ステートに変更する
		m_pAuditor->ChangeState(new CAuditorStateChase(), ID_CHASE);
		return;
	}
}
//=========================================================
// 終了関数
//=========================================================
void CAuditorStateDoubt::OnExit(void)
{
	// カウントリセット
	m_nDoubtCount = 0;

	// uiを破棄する
	m_pGauge->Uninit();
}