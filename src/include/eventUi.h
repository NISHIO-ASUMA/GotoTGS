//=========================================================
//
// イベントUI処理 [ eventUI.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// イベントUIに対するクラスを定義
//*********************************************************
class CEventUI :public CObject2D
{
public:

	CEventUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CEventUI();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CEventUI* Create(void);

private:

	// メンバ変数

};