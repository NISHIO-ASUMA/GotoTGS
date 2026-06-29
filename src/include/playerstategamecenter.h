//=========================================================
//
// ゲームセンターサボり状態クラス処理 [ playerstategamecenter.h ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "playerstatebase.h"

//*********************************************************
// ゲームセンターサボり状態クラスを定義
//*********************************************************
class CPlayerStateGameCenter : public CPlayerStateBase
{
public:
	CPlayerStateGameCenter();
	~CPlayerStateGameCenter();

	void OnStart(void) override;
	void OnUpdate(void) override;
	void OnExit(void) override;

private:
};