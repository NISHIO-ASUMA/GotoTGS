//=========================================================
//
// リザルト用スコア管理処理 [ resultscoremanager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <array>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CResultScore;
class CLoad;

//*********************************************************
// リザルトスコア管理クラスを定義
//*********************************************************
class CResultScoreManager
{
public:

	~CResultScoreManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Load(void);

	/// <summary>
	/// 各番号のスコアを取得
	/// </summary>
	/// <param name="nidx">取得番号</param>
	/// <returns></returns>
	CResultScore* GetIdx(const int& nidx) { return m_pResultScore[nidx]; }

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CResultScoreManager* GetInstancce(void);

private:

	CResultScoreManager();

	/// <summary>
	/// 2つのint型を計算して引き算結果を返す関数
	/// </summary>
	/// <param name="nMinScore">小さいほうのスコア(タスク分)</param>
	/// <param name="nMaxScore">大きい方のスコア(サボり分)</param>
	/// <returns></returns>
	int MathScoreResult(int& nMinScore, int& nMaxScore);

private:

	//***************************
	// 定数格納構造体
	//***************************
	struct Config
	{
		static constexpr int IDX_LAZY			  = 0;								// サボりスコアインデックス
		static constexpr int IDX_TASK			  = 1;								// タスクスコアインデックス
		static constexpr int IDX_ALL			  = 2;								// 最終スコアインデックス
		static constexpr const char* LAZYSCORE	  = "data/SCORE/LazyScore.bin";		// サボりのスコア
		static constexpr const char* TASKSCORE	  = "data/SCORE/TaskScore.bin";		// タスクのスコア
		static constexpr const char* ALLSCORE	  = "data/SCORE/AllScore.bin";		// 最終スコア
	};

private:

	int m_nLazyScore;		// サボりスコアを格納
	int m_nTaskScore;		// タスクスコアを格納
	int m_nLastScore;		// 最終計算スコアを格納

	std::array<CResultScore*, Config::IDX_ALL + 1>m_pResultScore;	// リザルトスコアポインタ
	std::unique_ptr<CLoad>m_pLoad;									// ロードクラスのポインタ
};