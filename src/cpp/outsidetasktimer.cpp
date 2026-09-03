//=========================================================
//
// 外タスクのタイマー処理 [ outsidetasktimer.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "outsidetasktimer.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "number.h"
#include "template.h"
#include "camera.h"

//=========================================================
// コンストラクタ
//=========================================================
COutSideTaskTimer::COutSideTaskTimer(int nPriority) : CObject(nPriority),
m_nAllTime(NULL),
m_nDecTime(NULL),
m_nCounter(NULL),
m_nMaxTime(NULL),
m_fWidth(NULL),
m_fHeight(NULL),
m_pos(VECTOR3_NULL)
{
	// ポインタ初期化
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		m_pNumberMinutes[nDigit] = nullptr;
	}
}
//=========================================================
// デストラクタ
//=========================================================
COutSideTaskTimer::~COutSideTaskTimer()
{

}
//=========================================================
// 生成処理処理
//=========================================================
COutSideTaskTimer* COutSideTaskTimer::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// インスタンス生成
	COutSideTaskTimer* pOutSidetime = new COutSideTaskTimer;
	if (pOutSidetime == nullptr) return nullptr;

	// 引数を設定
	pOutSidetime->SetPos(pos);
	pOutSidetime->SetWidth(fWidth);
	pOutSidetime->SetHeight(fHeight);

	// 初期化が失敗した場合
	if (FAILED(pOutSidetime->Init())) return nullptr;

	return pOutSidetime;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT COutSideTaskTimer::Init(void)
{
	// 開始時の時間を格納
	m_nMaxTime = Config::NUMTIME;

	// 全体の時間を設定
	m_nAllTime = Config::NUMTIME;

	// 一桁の横幅
	float fTexpos = m_fWidth / Config::DIGIT_TIME;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		// 数値生成
		m_pNumberMinutes[nDigit] = new CNumber;

		// ナンバーの初期化処理
		m_pNumberMinutes[nDigit]->Init
		(D3DXVECTOR3(m_pos.x + (fTexpos * Config::VALUE_FLOAT * nDigit), m_pos.y, 0.0f),		// 位置
			fTexpos,																			// 一桁分の横幅
			m_fHeight);																			// 縦幅

		// サイズ設定
		m_pNumberMinutes[nDigit]->SetSize(fTexpos, m_fHeight);

		// カラー設定
		m_pNumberMinutes[nDigit]->SetCol(COLOR_WHITE);

		// テクスチャ設定
		m_pNumberMinutes[nDigit]->SetTexture(Config::TEXNAME);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void COutSideTaskTimer::Uninit(void)
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
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void COutSideTaskTimer::Update(void)
{
	// アニメーション中なら
	if (CManager::GetInstance()->GetCamera()->GetIsAnimTime() || 
		CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_BOSS_SYSTEM) 
		return;

	// 最大時間が0なら処理を通さない
	if (m_nAllTime <= 0)
	{
		m_nAllTime = 0;
		return;
	}

	if (m_nCounter < 0)
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

		if (m_nAllTime <= 0)
		{// 現在の時間が0以下になった時
			// カウンターを0にする
			m_nCounter = 0;
		}

		// 経過時間を増やす
		m_nDecTime++;
	}

	// 桁数更新処理
	UpdateDigitNumbers();
}
//=========================================================
// 描画処理
//=========================================================
void COutSideTaskTimer::Draw(void)
{
	// 桁数分表示
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		// ナンバーの描画処理
		m_pNumberMinutes[nDigit]->Draw();
	}
}
//=========================================================
// 桁数の更新関数
//=========================================================
void COutSideTaskTimer::UpdateDigitNumbers(void)
{
	// 桁数ごとの分割値
	int nTimeDate = Config::DIVIDE * Config::DIVIDE;
	int nTimeDateBase = Config::DIVIDE;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		int nPosTexU = m_nAllTime % nTimeDate / nTimeDateBase;
		nTimeDate /= Config::DIVIDE;
		nTimeDateBase /= Config::DIVIDE;

		if (m_pNumberMinutes[nDigit] != nullptr)
		{// 分数の更新処理
			m_pNumberMinutes[nDigit]->Update();
			m_pNumberMinutes[nDigit]->SetDigit(nPosTexU);
		}
	}
}