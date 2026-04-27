//=========================================================
//
// ゲームのタイマー処理 [ gametime.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gametime.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "number.h"

//=========================================================
// コンストラクタ
//=========================================================
CGametime::CGametime(int nPriority) : CObject(nPriority),
m_nAllTime(NULL),
m_nSeconds(NULL),
m_nMinutes(NULL),
m_nDecTime(NULL),
m_nCounter(NULL),
m_fWidth(NULL),
m_fHeight(NULL),
m_pos(VECTOR3_NULL)
{

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		m_pNumberMinutes[nDigit] = nullptr;
		m_pNumberSeconds[nDigit] = nullptr;
	}	
}

//=========================================================
// デストラクタ
//=========================================================
CGametime::~CGametime()
{
}

//=========================================================
// 生成処理処理
//=========================================================
CGametime* CGametime::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CGametime* pGametime = new CGametime;

	if (pGametime == nullptr)
	{// ヌルチェック
		return nullptr;
	}

	// 引数を設定
	pGametime->SetPos(pos);
	pGametime->SetWidth(fWidth);
	pGametime->SetHeight(fHeight);

	if (FAILED(pGametime->Init()))
	{// 初期化が失敗した場合
		return nullptr;
	}

	return pGametime;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGametime::Init(void)
{
	// 全体の時間を設定
	m_nAllTime = Config::NUMTIME;

	// 現在の分数を計算
	m_nMinutes = m_nAllTime / Config::CARVETIME;

	// 現在の秒数を計算
	m_nSeconds = m_nAllTime % Config::CARVETIME;

	// 一桁の横幅
	float fTexpos = m_fWidth / Config::DIGIT_TIME;

	// 分を生成
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		m_pNumberMinutes[nDigit] = new CNumber;

		// ナンバーの初期化処理
		m_pNumberMinutes[nDigit]->Init(D3DXVECTOR3(m_pos.x + (fTexpos * Config::VALUE_FLOAT * nDigit), m_pos.y, 0.0f),		// 位置
			fTexpos,																										// 一桁分の横幅
			m_fHeight);																										// 縦幅

		// サイズ設定
		m_pNumberMinutes[nDigit]->SetSize(fTexpos, m_fHeight);

		// カラー設定
		m_pNumberMinutes[nDigit]->SetCol(COLOR_WHITE);

		// テクスチャ設定
		m_pNumberMinutes[nDigit]->SetTexture(Config::TEXNAME);
	}

	// 位置を横にずらす
	m_pos.x += Config::VALUE_WIDTH;

	// 秒を生成
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		m_pNumberSeconds[nDigit] = new CNumber;

		// ナンバーの初期化処理
		m_pNumberSeconds[nDigit]->Init(D3DXVECTOR3(m_pos.x + (fTexpos * Config::VALUE_FLOAT * nDigit), m_pos.y, 0.0f),		// 位置
			fTexpos,																										// 一桁分の横幅
			m_fHeight);																										// 縦幅

		// サイズ設定
		m_pNumberSeconds[nDigit]->SetSize(fTexpos, m_fHeight);

		// カラー設定
		m_pNumberSeconds[nDigit]->SetCol(COLOR_WHITE);

		// テクスチャ設定
		m_pNumberSeconds[nDigit]->SetTexture(Config::TEXNAME);

	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CGametime::Uninit(void)
{
	// 桁数分破棄
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		if (m_pNumberMinutes[nDigit] != nullptr)
		{
			// 分数の終了処理
			m_pNumberMinutes[nDigit]->Uninit();
			delete m_pNumberMinutes[nDigit];
			m_pNumberMinutes[nDigit] = nullptr;
		}

		if (m_pNumberSeconds[nDigit] != nullptr)
		{
			// 秒数の終了処理
			m_pNumberSeconds[nDigit]->Uninit();
			delete m_pNumberSeconds[nDigit];
			m_pNumberSeconds[nDigit] = nullptr;
		}

	}

	// オブジェクト自身の破棄
	CObject::Release();
}

//=========================================================
// 更新処理
//=========================================================
void CGametime::Update(void)
{
	if (m_nCounter <= 0)
	{
		// カウンターを0にする
		m_nCounter = 0;

		return;
	}

	// カウンターを1つ増やす
	m_nCounter++;

	if (m_nCounter >= Config::CARVETIME)
	{// カウンターが1Fを超えたとき
		
		// カウンターを0にする
		m_nCounter = 0;

		// 全体の時間を減らす
		m_nAllTime--;

		// 現在の分数を計算
		m_nMinutes = m_nAllTime / Config::CARVETIME;

		// 現在の秒数を計算
		m_nSeconds = m_nAllTime % Config::CARVETIME;

		if (m_nAllTime <= 0)
		{// 現在の時間が0以下になった時
			// カウンターを0にする
			m_nCounter = 0;
		}

		// 経過時間を増やす
		m_nDecTime++;
	}

	// それぞれの数字の計算処理
	Seconds();
	Minutes();
}

//=========================================================
// 描画処理
//=========================================================
void CGametime::Draw(void)
{
	// 桁数分表示
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		// ナンバーの描画処理
		m_pNumberMinutes[nDigit]->Draw();
		m_pNumberSeconds[nDigit]->Draw();
	}
}

//=========================================================
// 秒数処理
//=========================================================
void CGametime::Seconds(void)
{
	// 桁数ごとの分割値
	int nTimeDate = Config::DIVIDE * Config::DIVIDE;
	int nTimeDateBase = Config::DIVIDE;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		int nPosTexU = m_nSeconds % nTimeDate / nTimeDateBase;
		nTimeDate /= Config::DIVIDE;
		nTimeDateBase /= Config::DIVIDE;

		if (m_pNumberSeconds[nDigit] != nullptr)
		{// 秒数の更新処理
			m_pNumberSeconds[nDigit]->Update();
			m_pNumberSeconds[nDigit]->SetDigit(nPosTexU);
		}
	}
}

//=========================================================
// 分数処理
//=========================================================
void CGametime::Minutes(void)
{
	// 桁数ごとの分割値
	int nTimeDate = Config::DIVIDE * Config::DIVIDE;
	int nTimeDateBase = Config::DIVIDE;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		int nPosTexU = m_nMinutes % nTimeDate / nTimeDateBase;
		nTimeDate /= Config::DIVIDE;
		nTimeDateBase /= Config::DIVIDE;

		if (m_pNumberMinutes[nDigit] != nullptr)
		{// 分数の更新処理
			m_pNumberMinutes[nDigit]->Update();
			m_pNumberMinutes[nDigit]->SetDigit(nPosTexU);
		}
	}

}