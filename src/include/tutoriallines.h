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
// システムインクルードファイル
//*********************************************************
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CTutorialLinesBG;
class CLines;
class CTutorialPlayer;
class CPointObject;
class CBillboard;

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
		LINESTYPE_4,		// セリフ4
		LINESTYPE_5,		// セリフ5
		LINESTYPE_6,		// セリフ6
		LINESTYPE_7,		// セリフ7
		LINESTYPE_8,		// セリフ8
		LINESTYPE_9,		// セリフ9
		LINESTYPE_10,		// セリフ10
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

	// 設定処理
	void SetNextTutorial(void);		// 次のチュートリアルに進む設定
	void SetPlayer(CTutorialPlayer* pPlayer = nullptr) { m_pTutoPlayer = pPlayer; }

	// 情報取得処理
	CTutorialLinesBG* GetBG(void) { return m_pBG; }		// 背景のポインタ
	CLines* GetLines(void) { return m_pLines; }			// セリフのポインタ
	bool GetUse(void) const { return m_bUse; }			// 使用しているかどうか
	bool GetIsComp(void) const { return m_isComplite; }
	int GetNowIdxNumber(void) const { return m_nNowIdx; }

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const int MAX_COUNT = 60;
		static constexpr const float POS_X = 790.0f;
		static constexpr const float POS_Y = SCREEN_HEIGHT * 0.87f;
		static constexpr const float WIDTH = 500.0f;						// 横幅
		static constexpr const float HEIGHT = 100.0f;						// 縦幅
		static constexpr const char* LINES_BG = "TutoriallinesBG.png";		// 背景のテクスチャ名
		static constexpr const char* LINES_001 = "Tutoriallines001.png";	// セリフ1
		static constexpr const char* LINES_002 = "Tutoriallines002.png";	// セリフ2
		static constexpr const char* LINES_003 = "Tutoriallines003.png";	// セリフ3
		static constexpr const char* LINES_004 = "Tutoriallines004.png";	// セリフ4
		static constexpr const char* LINES_005 = "Tutoriallines005.png";	// セリフ5
		static constexpr const char* LINES_006 = "Tutoriallines006.png";	// セリフ6
		static constexpr const char* LINES_007 = "Tutoriallines007.png";	// セリフ7
		static constexpr const char* LINES_008 = "Tutoriallines008.png";	// セリフ8
		static constexpr const char* LINES_009 = "Tutoriallines009.png";	// セリフ9
		static constexpr const char* LINES_010 = "Tutoriallines010.png";	// セリフ10

	};

	// メンバ変数
	// セリフの種類
	const char* m_LinesType[LINESTYPE_MAX] =
	{
		{Config::LINES_001},
		{Config::LINES_002},
		{Config::LINES_003},
		{Config::LINES_004},
		{Config::LINES_005},
		{Config::LINES_006},
		{Config::LINES_007},
		{Config::LINES_008},
		{Config::LINES_009},
		{Config::LINES_010},

	};
	
	bool m_bUse;				// 使用しているかどうか
	bool m_bAgain;				// もう一度するかどうか
	int m_nNowIdx;				// 現在使用しているテクスチャの番号
	int m_nCountSkip;			// スキップする為のカウント
	CTutorialLinesBG* m_pBG;	// 背景のポインタ
	CLines* m_pLines;			// セリフのポインタ
	CTutorialPlayer* m_pTutoPlayer; // チュートリアルのプレイヤーポインタの入れ物

/// <summary>
/// ADD : 西尾  オブジェクトの追加
/// </summary>
private:
	void CreateArrow(int& nIdx);
private:
	std::vector<CPointObject*> m_pArrow;	// 矢印オブジェクト
	CBillboard* m_pKeyUi;					// キー入力表示のui
	bool m_isComplite;						// 全部完了した
};