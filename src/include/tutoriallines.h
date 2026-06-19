//=========================================================
//
// チュートリアルのセリフ処理 [ tutoriallines.h ]
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
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CTutorialLinesBG;

//*********************************************************
// チュートリアルのセリフに対するクラスを定義
//*********************************************************
class CTutorialLines :public CObject
{
public:

	CTutorialLines(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTutorialLines();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CTutorialLines* Create(void);

	// 情報取得処理
	CTutorialLinesBG* GetBG(void) { return m_pBG; }		// 背景

private:

	// メンバ変数
	CTutorialLinesBG* m_pBG;
};