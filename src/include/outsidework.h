//=========================================================
//
// 外出タスク処理 [ outsidework.h ]
// Author: Takahashi Misaki
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルード
//*********************************************************
#include "deskworkUImanager.h"

//*********************************************************
// 外出タスククラスを定義
//*********************************************************
class COutsideWork :public CDeskworkUIManager
{
public:

	COutsideWork();
	~COutsideWork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 外出タスクをこなした時の設定
	void SetOutside(void);

	// 外出するときの処理
	void GoOut(void);

	// 生成処理
	static COutsideWork* Create(const bool& bUse = false);

private:
	// メンバ変数
	bool bGoOut;	// 外出できるかどうか
	bool bTask;		// タスク中かどうか
};