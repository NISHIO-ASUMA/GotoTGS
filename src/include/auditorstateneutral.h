//=========================================================
//
// 監査人の通常状態クラス [ auditorstateneutral.h ]
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

//*********************************************************
// 敵の通常状態クラスを定義
//*********************************************************
class CAuditorStateNeutral : public CAuditorStateBase
{
public:
	CAuditorStateNeutral();
	~CAuditorStateNeutral();

	void OnStart(void) override;
	void OnUpdate(void) override;
	void OnExit(void) override;

private:

};