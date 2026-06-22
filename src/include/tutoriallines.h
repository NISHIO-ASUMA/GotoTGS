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

	enum LINESTYPE
	{// セリフの種類
		LINESTYPE_1 = 0,	// セリフ1
		LINESTYPE_2,		// セリフ2
		LINESTYPE_3,		// セリフ3
		LINESTYPE_MAX		// セリフの総数
	};

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
	CLines* GetLines(void) { return m_pLines; }			// セリフ

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
		static constexpr const char* LINES_BG = "TutoriallinesBG.png";		// 背景のテクスチャ名
		static constexpr const char* LINES_001 = "Tutoriallines001.png";	// セリフ1
		static constexpr const char* LINES_002 = "Tutoriallines002.png";	// セリフ2
		static constexpr const char* LINES_003 = "Tutoriallines003.png";	// セリフ3

	};

	// メンバ変数
	// セリフの種類
	const char* m_LinesType[LINESTYPE_MAX] =
	{
		{Config:: LINES_001},
		{Config:: LINES_002},
		{Config:: LINES_003}
	};
	
	bool m_bUse;				// 使用しているかどうか
	int m_nNowIdx;				// 現在使用しているテクスチャの番号
	CTutorialLinesBG* m_pBG;	// 背景のポインタ
	CLines* m_pLines;			// セリフのポインタ
};