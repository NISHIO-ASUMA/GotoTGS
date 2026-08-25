//=========================================================
//
// 社長がプレイヤーを疑っている状態のクラス [ bossstatedoubt.h ]
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
#include "bossstatebase.h"
#include "enemydoubtgauge.h"

//*********************************************************
// 社長がプレイヤーを疑う状態クラスを定義
//*********************************************************
class CBossStateDoubt : public CBossStateBase
{
public:

	CBossStateDoubt();
	~CBossStateDoubt();

	void OnStart(void) override;
	void OnUpdate(void) override;
	void OnExit(void) override;

private:

	int m_nDoubtCount;			// 疑いカウントの上限
	CEnemyDoubtGauge* m_pGauge;	// 疑いゲージ ( 敵のやつそのまま流用可能 )

private:

	//***********************************
	// 定数構造体宣言
	//***********************************
	struct Config
	{
		static constexpr int MAX_DOUBT_COUNT = 120; // 2秒判定
		static constexpr float VALUE_HEIGHT = 80.0f;
		static constexpr float SIZE = 50.0f;
	};
};