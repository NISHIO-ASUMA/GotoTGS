//=========================================================
//
// リザルト用スコア管理処理 [ resultscoremanager.cpp ]
// Author: Asuma Nishio
// 
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultscoremanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <ostream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "resultscore.h"
#include "manager.h"
#include "input.h"
#include "ranking.h"
#include "load.h"
#include "template.h"
#include "resulticon.h"

//=========================================================
// コンストラクタ
//=========================================================
CResultScoreManager::CResultScoreManager() : m_pResultScore{},
m_nTaskScore(NULL),
m_nLazyScore(NULL),
m_nLastScore(NULL),
m_pLoad(nullptr)
{
}
//=======================================================
// デストラクタ
//=======================================================
CResultScoreManager::~CResultScoreManager()
{

}
//=========================================================
// インスタンス取得処理
//=========================================================
CResultScoreManager* CResultScoreManager::GetInstancce(void)
{
	static CResultScoreManager Instance;
	return &Instance;
}
//=======================================================
// 初期化処理
//=======================================================
HRESULT CResultScoreManager::Init(void)
{
	// ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	// 構造体宣言
	Config info = {};
	
	// スコアファイル読み込み
	Load();

	// スコアを生成する
	m_pResultScore[info.IDX_LAZY] = CResultScore::Create(D3DXVECTOR3(360.0f, 230.0f, 0.0f), 140.0f, 55.0f);	// サボりスコア
	m_pResultScore[info.IDX_TASK] = CResultScore::Create(D3DXVECTOR3(1045.0f,230.0f, 0.0f), 140.0f, 55.0f);	// タスクスコア
	m_pResultScore[info.IDX_ALL] =  CResultScore::Create(D3DXVECTOR3(680.0f, 595.0f, 0.0f), 160.0f, 80.0f);	// 最終スコア

	// 最終スコアを計算し、出力する
	m_nLastScore = MathScoreResult(m_nTaskScore, m_nLazyScore);

	// アニメーションするスコアをセットする
	m_pResultScore[info.IDX_LAZY]->SetAnimScore(m_nLazyScore);
	m_pResultScore[info.IDX_TASK]->SetAnimScore(m_nTaskScore);

	// 計算された最終スコアが0以下なら
	if (m_nLastScore <= 0)
	{
		m_nLastScore = NULL;
	}

	// 最終スコアをセット
	m_pResultScore[info.IDX_ALL]->SetAnimScore(m_nLastScore);

	// アイコン生成クラス
	CResultIcon::Create(D3DXVECTOR3(1020.0f, 570.0f, 0.0f), 130.0f, 130.0f,m_nLastScore);

	return S_OK;
}
//=======================================================
// 終了処理
//=======================================================
void CResultScoreManager::Uninit(void)
{
	// 最終スコアを書き出す(ランキング用に書き出し)
	m_pResultScore[Config::IDX_ALL]->Save();

	// ロードクラスの破棄
	m_pLoad.reset();
}
//=======================================================
// 更新処理
//=======================================================
void CResultScoreManager::Update(void)
{
	
}
//=======================================================
// バイナリファイル読み込み処理
//=======================================================
void CResultScoreManager::Load(void)
{
	// スコアを読み込む
	m_nLazyScore = m_pLoad->LoadInt(Config::LAZYSCORE);
	m_nTaskScore = m_pLoad->LoadInt(Config::TASKSCORE);
}
//=======================================================
// 引き算計算
//=======================================================
int CResultScoreManager::MathScoreResult(int& nMinScore, int& nMaxScore)
{
	// 格納用変数
	int nResultScore = 0;

	// 計算式
	nResultScore = nMaxScore + nMinScore;

	// 計算結果を返す
	return nResultScore;
}