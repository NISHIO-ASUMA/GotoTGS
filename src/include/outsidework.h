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

	// 受付人に話しかけた時の設定
	void SetOutside(void);

	// 外出から帰ってきた時の処理
	void EndOutside(void);

	// タスク中の処理
	void TaskSystem(void);

	// 情報取得処理
	bool GetGoOutside(void) { return m_bGoOutside; }
	bool GetTaskNow(void) { return m_bTaskNow; }

	// 生成処理
	static COutsideWork* Create(const bool& bUse = false);

private:
	// メンバ変数
	bool m_bGoOutside;	// 外出できるかどうか
	bool m_bTaskNow;		// タスク中かどうか
};