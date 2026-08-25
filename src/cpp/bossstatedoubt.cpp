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
m_nDoubtCount(0),
m_pGauge(nullptr)
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
	// ui生成 ( ?のゲージ )
	auto CreatePos = D3DXVECTOR3(m_pBoss->GetPos().x, m_pBoss->GetPos().y + Config::VALUE_HEIGHT, m_pBoss->GetPos().z);
	m_pGauge = CEnemyDoubtGauge::Create(CreatePos, Config::SIZE, Config::SIZE);
}
//=========================================================
// 更新関数
//=========================================================
void CBossStateDoubt::OnUpdate(void)
{
	// 頭上のゲージの位置の更新
	if (m_pGauge && m_pBoss)
	{
		D3DXVECTOR3 headPos = m_pBoss->GetPos();
		headPos.y += Config::VALUE_HEIGHT;
		m_pGauge->SetTargetPos(headPos);
	}

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
		m_pGauge->SetIsDraw(true);
		m_pGauge->SetUpGauge(true);

		// 疑いモーションセット
		m_pBoss->GetMotion()->SetMotion(CBoss::MOTION::DOUBT, true, 3);
	}
	else
	{
		// ゲージのクリア
		m_pGauge->SetUpGauge(false);
		m_pGauge->SetRatio(0.0040f);

		// もし完全クリアなら状態を元に戻す
		if (m_pGauge->GetNormalFlag())
		{
			m_pBoss->ChangeState(new CBossStateNeutral(), ID_NEUTRAL);
			return;
		}
	}

	// もし上限値を超えていたら
	if (m_nDoubtCount >= Config::MAX_DOUBT_COUNT && m_pGauge->GetIsComplete())
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

	// uiを破棄する
	m_pGauge->Uninit();
}