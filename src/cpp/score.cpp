//=========================================================
//
// スコア処理 [ score.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "score.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "load.h"

//=========================================================
// コンストラクタ
//=========================================================
CScore::CScore(int nPriority) : CObject(nPriority),
m_nScore(NULL),
m_col(COLOR_WHITE),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_nMinusScoreKeep(NULL),
m_apNumber{},
m_pLoad{},
m_isDraw(true)
{
}
//=========================================================
// デストラクタ
//=========================================================
CScore::~CScore()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CScore* CScore::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight, const bool& isDraw)
{
	// スコアインスタンス生成
	CScore* pScore = new CScore;
	if (pScore == nullptr) return nullptr;

	// オブジェクト設定
	pScore->SetPos(pos);
	pScore->SetWidth(fWidth);
	pScore->SetHeight(fHeight);
	pScore->m_isDraw = isDraw;

	// 初期化失敗時
	if (FAILED(pScore->Init())) return nullptr;
	
	// 生成されたポインタを返す
	return pScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CScore::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / Config::NUM_SCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < Config::NUM_SCORE; nCnt++)
	{
		// インスタンス生成
		m_apNumber[nCnt] = new CNumber;

		// 初期化処理
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * Config::DIGIT_VALUE * nCnt), m_pos.y,0.0f), fTexPos, m_fHeight);

		// テクスチャセット
		m_apNumber[nCnt]->SetTexture(Config::TEX_NAME);
	}

	// 読み込み処理ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CScore::Uninit(void)
{
	// ナンバークラスの破棄
	for (auto number : m_apNumber)
	{
		if (number != nullptr)
		{
			number->Uninit();
			delete number;
			number = nullptr;
		}
	}

	// ロードクラスのポインタ破棄
	m_pLoad.reset();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CScore::Update(void)
{
	if (!m_isDraw) return;

	// スコア格納
	int nScore = m_nScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < Config::NUM_SCORE; nCntScore++)
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % Config::NUM_DIGIT;

		// ナンバー更新
		m_apNumber[nCntScore]->Update();

		// 桁更新
		m_apNumber[nCntScore]->SetDigit(nDigit);

		if (nScore > 0 || nCntScore == 0)
		{
			m_apNumber[nCntScore]->SetIsUse(true);  // 表示
		}
		else
		{
			m_apNumber[nCntScore]->SetIsUse(false); // 非表示
		}

		// 次の桁へ
		nScore /= Config::NUM_DIGIT;
	}
}
//=========================================================
// 描画処理
//=========================================================
void CScore::Draw(void)
{
	if (!m_isDraw) return;

	// 数字の描画
	for (auto number : m_apNumber)
	{
		// trueなら描画をする
		if (number->GetIsUse())
		{
			number->Draw();
		}
	}
}
//=========================================================
// スコア加算処理
//=========================================================
void CScore::AddScore(int nValue)
{
	// スコア加算
	m_nScore += nValue;
	
	// もし0以下になったら
	if (m_nScore <= 0)
		m_nScore = 0;
}
//=========================================================
// マイナス分を加算する
//=========================================================
void CScore::AddScoreMinus(const int nValue)
{
	m_nMinusScoreKeep += nValue;
}
//=========================================================
// スコア初期化
//=========================================================
void CScore::DeleteScore(void)
{
	m_nScore = NULL;
}
//=========================================================
// スコア書き出し処理
//=========================================================
void CScore::SaveScore(const char* SaveName)
{
	// 一個の数値を書き出す
	m_pLoad->SaveInt(SaveName, m_nScore);
}
//=========================================================
// スコア書き出し処理 ( マイナスされた分 )
//=========================================================
void CScore::SaveScoreMinus(const char* SaveName)
{
	// 一個の数値を書き出す
	m_pLoad->SaveInt(SaveName, m_nMinusScoreKeep);
}