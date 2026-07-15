//=========================================================
//
// ランキングスコア処理 [ rankingscore.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankingscore.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <algorithm>
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "number.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CRankingScore::CRankingScore(int nPriority) : CObject(nPriority),
m_fHeight(NULL),
m_fWidth(NULL),
m_pos(VECTOR3_NULL),
m_pLoad(nullptr)
{
	for (int nCntData = 0; nCntData < Config::RANKING_MAX; nCntData++)
	{
		// スコア初期化
		m_aRankData[nCntData] = NULL;

		for (int nCnt = 0; nCnt < Config::RANKSCOREDIGIT; nCnt++)
		{
			// ナンバーポインタ
			m_apNumber[nCntData][nCnt] = nullptr;
		}
	}
}
//=========================================================
// デストラクタ
//=========================================================
CRankingScore::~CRankingScore()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CRankingScore* CRankingScore::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CRankingScore* pRankScore = new CRankingScore;
	if (pRankScore == nullptr) return nullptr;

	// オブジェクト設定
	pRankScore->SetPos(pos);
	pRankScore->SetWidth(fWidth);
	pRankScore->SetHeight(fHeight);

	// 初期化失敗時
	if (FAILED(pRankScore->Init())) return nullptr;

	// 生成されたポインタを返す
	return pRankScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingScore::Init(void)
{
	// スコアをロード
	Load();

	// 横幅計算
	float fTexPos = m_fWidth / Config::RANKSCOREDIGIT;

	for (int nRank = 0; nRank < Config::RANKING_MAX; nRank++)
	{
		// 桁数分
		for (int nDigit = 0; nDigit < Config::RANKSCOREDIGIT; nDigit++)
		{
			// インスタンス生成
			m_apNumber[nRank][nDigit] = new CNumber;

			// Y座標をずらす
			float yOffset = m_pos.y + (m_fHeight + Config::POS_Y_VALUE) * nRank;

			// 初期化処理
			m_apNumber[nRank][nDigit]->Init
			(
				D3DXVECTOR3(m_pos.x - (fTexPos * Config::POS_WIDTH_VALUE * nDigit), yOffset, 0.0f),
				fTexPos,
				m_fHeight
			);

			// サイズ設定
			m_apNumber[nRank][nDigit]->SetSize(fTexPos, m_fHeight);

			// カラー設定
			m_apNumber[nRank][nDigit]->SetCol(COLOR_WHITE);

			// テクスチャ設定
			m_apNumber[nRank][nDigit]->SetTexture(Config::TEXNAME);
		}
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingScore::Uninit(void)
{
	// ポインタの破棄
	m_pLoad.reset();

	// 使っている桁数分の破棄
	for (int nRankData = 0; nRankData < Config::RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt < Config::RANKSCOREDIGIT; nCnt++)
		{
			if (m_apNumber[nRankData][nCnt] != nullptr)
			{
				// ナンバークラスの終了処理
				m_apNumber[nRankData][nCnt]->Uninit();
				delete m_apNumber[nRankData][nCnt];
				m_apNumber[nRankData][nCnt] = nullptr;
			}
		}
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CRankingScore::Update(void)
{
	// スコアの桁数更新
	for (int nRank = 0; nRank < Config::RANKING_MAX; nRank++)
	{
		// 現在のスコアを格納
		int nScore = m_aRankData[nRank];

		for (int nDigit = 0; nDigit < Config::RANKSCOREDIGIT; nDigit++)
		{
			// 1桁ずつ取り出す
			int nNum = nScore % Config::DIGITNUM;

			// ナンバー更新
			m_apNumber[nRank][nDigit]->Update();

			// 桁更新
			m_apNumber[nRank][nDigit]->SetDigit(nNum);

			if (nScore > 0 || nDigit == 0)
			{
				m_apNumber[nRank][nDigit]->SetIsUse(true);  // 表示
			}
			else
			{
				m_apNumber[nRank][nDigit]->SetIsUse(false); // 非表示
			}

			// 数値の割合計算処理
			nScore /= Config::DIGITNUM;
		}
	}
}
//=========================================================
// 描画処理
//=========================================================
void CRankingScore::Draw(void)
{
	// 使っている桁数分の描画
	for (int nRankData = 0; nRankData < Config::RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt <Config::RANKSCOREDIGIT; nCnt++)
		{
			// ナンバー描画
			m_apNumber[nRankData][nCnt]->Draw();
		}
	}
}
//=========================================================
// ランキングスコア読み込み
//=========================================================
void CRankingScore::Load(void)
{
	// クラス生成
	m_pLoad = std::make_unique<CLoad>();

	// リザルトから来た最新の1つのスコアを読み込む
	int LoadresultScore = m_pLoad->LoadInt("data/SCORE/ResultScore.bin");
	if (LoadresultScore == -1) 	return;

	// 既存のランキングデータがあれば読み込む
	m_aRankData = m_pLoad->LoadIntToFixedArray("data/SCORE/Ranking.bin");

	// 現在の値を取得
	int rank5thScore = m_aRankData[Config::RANKING_MAX - 1];

	// 5番目のランキングの値以下なら
	if (LoadresultScore <= rank5thScore) return;

	// 格納用の配列
	std::vector<int> ScoreVector;
	ScoreVector.reserve(Config::RANKING_MAX + 1);

	// データを配列に入れる
	for (int score : m_aRankData)
	{
		ScoreVector.push_back(score);
	}

	// 最新スコアを追加
	ScoreVector.push_back(LoadresultScore);

	// 降順ソート
	std::sort(ScoreVector.begin(), ScoreVector.end(), std::greater<int>());

	// ランキングの分だけ配列に保存する
	for (int nCnt = 0; nCnt < Config::RANKING_MAX; nCnt++)
	{
		m_aRankData[nCnt] = ScoreVector[nCnt];
	}

	// 新しいランキングデータを書き出す
	if (m_pLoad)
		m_pLoad->SaveIntToFixedArray("data/SCORE/Ranking.bin", m_aRankData);
}