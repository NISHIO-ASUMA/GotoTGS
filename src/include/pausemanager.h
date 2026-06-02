//=========================================================
//
// ポーズ管理処理 [ pausemanager.h ]
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
class CPause;

//*********************************************************
// ポーズメニュー管理クラスを定義 ( シングルトン )
//*********************************************************
class CPauseManager
{
public:

	~CPauseManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// ポーズの有効状態を切り替える
	/// </summary>
	/// <param name="isFlags">引数で設定する値</param>
	void SetEnablePause(const bool& isFlags) { m_isPause = isFlags; }

	/// <summary>
	/// ポーズの状態を判別する
	/// </summary>
	/// <param name=""></param>
	void SetEnablePause(void);

	/// <summary>
	/// 現在のフラグを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetPause(void) { return m_isPause; }

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name="">生成される1つのインスタンス</param>
	/// <returns></returns>
	static CPauseManager* GetInstance(void)
	{
		static CPauseManager Instance;
		return &Instance;
	};

private:

	CPauseManager(); // 格納コンストラクタ

private:

	static constexpr int SELECT_BEGIN = 1;	// メニュー開始番号
	static constexpr int SELECT_END = 3;	// メニュー終了番号
	static constexpr int SELECT_MAX = 4;	// 最大生成数

private:

	bool m_isPause;					// ポーズかどうか
	int m_nSelectIdx;				// セレクト番号
	CPause* m_pPause[SELECT_MAX];	// ポーズクラスのポインタ配列
};