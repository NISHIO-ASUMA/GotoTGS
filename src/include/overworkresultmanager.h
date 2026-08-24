//=========================================================
//
// 過労死シーンのリソース管理処理 [ overworkresultmanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 過労死シーン管理クラスを定義
//*********************************************************
class COverWorkResultManager
{
public:

	~COverWorkResultManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static COverWorkResultManager* GetInstance(void)
	{
		static COverWorkResultManager Instance;
		return &Instance;
	}

private:

	COverWorkResultManager();	// 格納コンストラクタ
};
