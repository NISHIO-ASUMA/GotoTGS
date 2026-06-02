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

private:

	CTitleuiManager(); // 格納コンストラクタ

private:

	static constexpr int SELECT_BEGIN = 0;	// メニュー開始番号
	static constexpr int PAD_NUMBER = 1;	// パッドのインデックス
	static constexpr int SELECT_MAX = 2;	// 最大生成数

private:

	int m_nSelectIdx;				// セレクト番号
	CTitleUi* m_pUi[SELECT_MAX];	// タイトルuiクラスのポインタ配列
};