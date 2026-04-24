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
CGametime::CGametime(int nPriority) : CObject(nPriority)
{
	// 値を初期化
	m_pos = VECTOR3_NULL;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_NowTime = 0;

	for (int nDigit = 0; nDigit < Config::TIMEDIGIT; nDigit++)
	{
		m_pNumber[nDigit] = nullptr;
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

	// 引数をメンバ変数に代入
	pGametime->m_pos = pos;
	pGametime->m_fWidth = fWidth;
	pGametime->m_fHeight = fHeight;

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
	// 一桁の横幅
	float fTexpos = m_fWidth / Config::TIMEDIGIT;

	// 桁数分
	for (int nDigit = 0; nDigit < Config::TIMEDIGIT; nDigit++)
	{
		// インスタンス生成
		m_pNumber[nDigit] = new CNumber;

		// ナンバーの初期化処理
		m_pNumber[nDigit]->Init(D3DXVECTOR3(m_pos.x - (fTexpos * Config::POS_WIDTH_VALUE * nDigit), Config::POS_Y, 0.0f),		// 位置
			fTexpos,																											// 一桁分の横幅
			m_fHeight);																											// 縦幅

		// サイズ設定
		m_pNumber[nDigit]->SetSize(fTexpos, m_fHeight);

		// カラー設定
		m_pNumber[nDigit]->SetCol(COLOR_WHITE);

		// テクスチャ設定
		m_pNumber[nDigit]->SetTexture(Config::TEXNAME);
	}

	// 制限時間を設定
	m_NowTime = Config::TIMELIMIT;

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CGametime::Uninit(void)
{
	// 桁数分破棄
	for (int nDigit = 0; nDigit < Config::TIMEDIGIT; nDigit++)
	{
		if (m_pNumber[nDigit] != nullptr)
		{
			// ナンバーの終了処理
			m_pNumber[nDigit]->Uninit();
			delete m_pNumber[nDigit];
			m_pNumber[nDigit] = nullptr;
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
	if (m_nCountFream < Config::UNIT)
	{
		// フレームを1つ増やす
		m_nCountFream++;

		return;
	}

	// フレームを0にする
	m_nCountFream = 0;

	// 残り時間を減らす
	m_NowTime--;

	// 残り時間
	int nNowTime = ((m_NowTime / Config::UNIT) * Config::DIGITNUM * Config::DIGITNUM) + (m_NowTime % Config::UNIT);

	for (int nDigit = 0; nDigit < Config::TIMEDIGIT; nDigit++)
	{
		// 1桁を取り出す
		int nNum = 0;

		// 桁を更新する
		m_pNumber[nDigit]->SetDigit(nNum);
	}
}

//=========================================================
// 描画処理
//=========================================================
void CGametime::Draw(void)
{
	// 桁数分表示
	for (int nDigit = 0; nDigit < Config::TIMEDIGIT; nDigit++)
	{
		// ナンバーの描画処理
		m_pNumber[nDigit]->Draw();
	}
}