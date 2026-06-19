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
#include "object2D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CTutorialLinesBG;
class CLines;

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
	static CTutorialLines* Create(const bool& bUse = false);

	// 情報取得処理
	CTutorialLinesBG* GetBG(void) { return m_pBG; }		// 背景

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const float POS_X = 790.0f;
		static constexpr const float POS_Y = SCREEN_HEIGHT * 0.87f;
		static constexpr const float WIDTH = 500.0f;						// 横幅
		static constexpr const float HEIGHT = 100.0f;						// 縦幅
		static constexpr const char* SERIHU_001 = "Tutoriallines001.png";	// セリフ1
		//static constexpr const char* SERIHU_002 = "Tutoriallines000.png";	// セリフ2
		//static constexpr const char* SERIHU_003 = "Tutoriallines000.png";	// セリフ3

	};

	// メンバ変数
	bool m_bUse;					// 使用しているかどうか
	CTutorialLinesBG* m_pBG;		// 背景のポインタ
	CLines* m_pLines;				// セリフのポインタ

};