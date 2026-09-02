//=========================================================
//
// 監査人が疑っている状態のクラス [ auditorstatedoubt.h ]
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
#include "auditorstatebase.h"
#include "enemydoubtgauge.h"	// そのまま使う

//*********************************************************
// 監査人の疑い状態クラスを定義
//*********************************************************
class CAuditorStateDoubt : public CAuditorStateBase
{
public:

	CAuditorStateDoubt();
	~CAuditorStateDoubt();

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
		static constexpr int MAX_DOUBT_COUNT = 120;		// 2秒判定
		static constexpr float VALUE_HEIGHT = 80.0f;	// 頭の上
		static constexpr float SIZE = 50.0f;			// UIの大きさ
	};
};