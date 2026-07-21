//=========================================================
//
// ランキングの横棒グラフ複数管理処理 [ rankingglaphmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankinggraphmanager.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <algorithm>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "rankinggraph.h"
#include "load.h"

//=========================================================
// コンストラクタ
//=========================================================
CRankingGraphManager::CRankingGraphManager() : m_pLoad(nullptr),
m_pScoreData{},
m_pGraphUi{nullptr}
{

}
//=========================================================
// デストラクタ
//=========================================================
CRankingGraphManager::~CRankingGraphManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingGraphManager::Init
(
	const D3DXVECTOR3& pos,
	float fHeight
)
{
	// 値の読み込み
	Load();

	// スコアの最大値を取得
	int maxScore = m_pScoreData[0];

	// 描画位置の基準設定
	D3DXVECTOR3 currentPos = pos;

	for (int i = 0; i < Config::DATA_MAX; ++i)
	{
		// スコアの値を配列から取得
		int score = m_pScoreData[i];

		float currentWidth = 0.0f;
		if (maxScore > 0 && score > 0)
		{
			// 最大幅から割合計算
			float ratio = static_cast<float>(score) / static_cast<float>(maxScore);
			currentWidth = UI_Config::MAX_WIDTH * ratio;
		}

		// ui生成
		m_pGraphUi[i] = CRankingGraph::Create(currentPos, currentWidth, fHeight);

		// Y座標をずらす
		currentPos.y += UI_Config::POS_Y_VALUE;
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingGraphManager::Uninit(void)
{
	// ポインタの破棄
	m_pLoad.reset();
}
//=========================================================
// 更新処理
//=========================================================
void CRankingGraphManager::Update(void)
{

}
//=========================================================
// 値の読み込み処理
//=========================================================
void CRankingGraphManager::Load(void)
{
	// ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	// 配列に数値を持たせる
	m_pScoreData = m_pLoad->LoadIntToFixedArray("data/SCORE/Ranking.bin");

	// ソートする
	std::sort(m_pScoreData.begin(), m_pScoreData.end(), std::greater<int>());
}