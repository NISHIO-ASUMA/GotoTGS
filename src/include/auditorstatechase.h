//=========================================================
//
// 監査人が追いかける状態のクラス[ auditorstatechase.h ]
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
// 監査人がプレイヤーを猛追する状態のクラスを定義
//*********************************************************
class CAuditorStateChase : public CAuditorStateBase
{
public:

	CAuditorStateChase();
	~CAuditorStateChase();

	void OnStart(void) override;
	void OnUpdate(void) override;
	void OnExit(void) override;

private:

	int m_nStayCount;	// 停止中のカウント
	bool m_isDoubt;		// 疑い状態かどうか
};