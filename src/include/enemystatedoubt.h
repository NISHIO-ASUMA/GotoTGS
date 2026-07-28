//=========================================================
//
// プレイヤーを疑っている状態のクラス [ enemystatedoubt.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemystatebase.h"
#include "enemydoubtgauge.h"

//*********************************************************
// 疑い状態クラスを定義
//*********************************************************
class CEnemyStateDoubt : public CEnemyStateBase
{
public:

	CEnemyStateDoubt();
	~CEnemyStateDoubt();

	void OnStart(void) override;
	void OnUpdate(void) override;
	void OnExit(void) override;

private:

	int m_nDoubtCount;			// 疑いカウントの上限
	CEnemyDoubtGauge* m_pGauge;	// 疑いゲージ

private:

	//***********************************
	// 定数構造体宣言
	//***********************************
	struct Config
	{
		static constexpr int MAX_DOUBT_COUNT = 120; // 2秒判定
	};
};