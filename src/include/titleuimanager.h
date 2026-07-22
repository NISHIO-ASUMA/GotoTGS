//=========================================================
//
// タイトルui複数管理処理 [ titleuimanager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 前方宣言
//*********************************************************
class CTitleUi;
class CUi;

//*********************************************************
// タイトルui管理処理
//*********************************************************
class CTitleuiManager
{
public:

	~CTitleuiManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name="">生成される1つのインスタンス</param>
	/// <returns></returns>
	static CTitleuiManager* GetInstance(void)
	{
		static CTitleuiManager Instance;
		return &Instance;
	};

	/// <summary>
	/// タイトルで選択されたインデックス番号を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetSelectIdx(void) const { return m_nSelectIdx; }

private:

	CTitleuiManager(); // 格納コンストラクタ

	void SlideStartUi(void); // 初期uiのスライド処理関数
	void InputCheck(void);	// 入力判定

private:

	static constexpr int SELECT_BEGIN = 1;	// 開始番号
	static constexpr int PAD_NUMBER = 2;	// パッドのインデックス
	static constexpr int SELECT_MAX = 3;	// 最大生成数

private:
	static constexpr int START_UI_NUM = 2;  // 最初に置いてあるuiの生成数

private:

	int m_nSelectIdx;				// セレクト番号
	CTitleUi* m_pUi[SELECT_MAX];	// タイトルuiクラスのポインタ配列

private:

	bool m_isFinishSlideUi;				// スライド終了判定
	bool m_isSelectFinish;				// 選択終わり判定
	bool m_isInputSet;					// 入力判定
	float m_fSlideRatio;				// スライドの進行度
	CUi* m_pStartUiList[START_UI_NUM];	// 初期でおいてあるui群
};