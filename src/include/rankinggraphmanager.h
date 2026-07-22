//=========================================================
//
// ランキングの横棒グラフ複数管理処理 [ rankingglaphmanager.h ]
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
class CLoad;
class CRankingGraph;

//*********************************************************
// ランキングの横棒グラフ管理処理クラス定義
//*********************************************************
class CRankingGraphManager
{
public:

	~CRankingGraphManager();

	HRESULT Init(const D3DXVECTOR3& pos,float fHeight);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CRankingGraphManager* GetInstance(void)
	{
		static CRankingGraphManager Instance;
		return &Instance;
	}

private:
	CRankingGraphManager();

private:
	void Load(void);

private:

	//*************************************
	// 定数構造体宣言
	//*************************************
	struct Config
	{
		static constexpr int DATA_MAX = 5;			// 表示する横棒の数
	};

	//*************************************
	// UI定数構造体宣言
	//*************************************
	struct UI_Config
	{
		static constexpr float POS_Y_VALUE = 100.0f;		// ずらすY座標値
		static constexpr float MAX_WIDTH = 490.0f;			// 最大値の横幅
	};

private:
	std::unique_ptr<CLoad> m_pLoad;								// 読み込みクラス
	std::array<int, Config::DATA_MAX> m_pScoreData;				// ランキングから来たデータの配列
	std::array<CRankingGraph*, Config::DATA_MAX> m_pGraphUi;	// ui配列
};