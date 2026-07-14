//=========================================================
//
// リザルト用スコア管理処理 [ resultscoremanager.cpp ]
// Author: Asuma Nishio
// 
// NOTE : これから複数のスコアを読み込む形に変わる
//        今は単体のスコアを読み込む形に変更しているだけ
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

//=========================================================
// コンストラクタ
//=========================================================
CResultScoreManager::CResultScoreManager() : m_pResultScore{},
m_nTaskScore(NULL),
m_nLazyScore(NULL),
m_nLastScore(NULL)
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
	m_pResultScore[info.IDX_LAZY] = CResultScore::Create(D3DXVECTOR3(1250.0f, 230.0f, 0.0f), 140.0f, 50.0f);	// サボりスコア
	m_pResultScore[info.IDX_TASK] = CResultScore::Create(D3DXVECTOR3(1250.0f, 400.0f, 0.0f), 140.0f, 50.0f);	// タスクスコア
	m_pResultScore[info.IDX_ALL] = CResultScore::Create(D3DXVECTOR3(1255.0f, 630.0f, 0.0f), 160.0f, 60.0f);		// 最終スコア

	// 最終スコアを計算し、出力
	m_nLastScore = MathScoreResult(m_nLazyScore, m_nTaskScore);

	// アニメーションするスコアをセットする
	m_pResultScore[info.IDX_LAZY]->SetAnimScore(m_nLazyScore);
	m_pResultScore[info.IDX_TASK]->SetAnimScore(m_nTaskScore);
	m_pResultScore[info.IDX_ALL]->SetAnimScore(m_nLastScore);

	return S_OK;
}
//=======================================================
// 終了処理
//=======================================================
void CResultScoreManager::Uninit(void)
{
	// 最終スコアを書き出す(ランキング用に持ち込む)
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
// デストラクタ
//=======================================================
int CResultScoreManager::MathScoreResult(int& nScore1, int& nScore2)
{
	// 格納用変数
	int nResultScore = 0;
	nResultScore = nScore1 + nScore2;

	// 計算結果を返す
	return nResultScore;
}