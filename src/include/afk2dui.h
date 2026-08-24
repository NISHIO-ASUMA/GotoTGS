//=========================================================
//
// AFK2DUI処理 [ afk2dui.h ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// サボりの2DUIオブジェクトクラスを定義
//*********************************************************
class CAfk2DUI : public CObject2D
{
public:

	CAfk2DUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CAfk2DUI();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CAfk2DUI* Create(void);

public:

	void SetisDisplay(const bool isDisplay) { m_bDisplay = isDisplay; }
	bool GetIsDisplay(void) const { return m_bDisplay; }
	bool GetAfkButton(void) const { return m_bAfkButton; }

private:
	void EasingSine(void);

private:

	float m_fCountFrame;	// イージング用フレームカウント用変数
	float m_fMaxFrame;		// 最大フレーム変数
	bool m_bAfkButton;		// ボタン識別判定用変数
	bool m_bEasing;			// イージング用変数
	bool m_bDisplay;		// 表示するかの判定変数
};

