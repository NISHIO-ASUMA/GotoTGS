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

	// サボりスコア生成
	m_pResultScore[info.IDX_LAZY] = CResultScore::Create(D3DXVECTOR3(1250.0f, 230.0f, 0.0f), 140.0f, 50.0f);

	// スコアをセットする
	m_pResultScore[info.IDX_LAZY]->SetAnimScore(m_nLazyScore);

	return S_OK;
}
//=======================================================
// 終了処理
//=======================================================
void CResultScoreManager::Uninit(void)
{
	// ロードクラスの破棄
	m_pLoad.reset();
}
//=======================================================
// 更新処理
//=======================================================
void CResultScoreManager::Update(void)
{
	// 無し
}
//=======================================================
// バイナリファイル読み込み処理
//=======================================================
void CResultScoreManager::Load(void)
{
	// スコアを読み込む
	m_nLazyScore = m_pLoad->LoadInt(Config::LAZYSCORE);
}