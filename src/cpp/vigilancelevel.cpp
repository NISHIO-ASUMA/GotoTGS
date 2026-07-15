//=========================================================
//
// 警戒度のレベル処理 [ vigilancelevel.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "vigilancelevel.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "number.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CNumber* CVigilancelevel::m_apNumber[Config::NUM_SCORE] = {};

//=========================================================
// コンストラクタ
//=========================================================
CVigilancelevel::CVigilancelevel(int nPriority) :CObject2D(nPriority)
{
	for (int nCount = 0; nCount < Config::NUM_SCORE; nCount++)
	{
		m_apNumber[nCount] = nullptr;
	}
}

//=========================================================
// デストラクタ
//=========================================================
CVigilancelevel::~CVigilancelevel()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CVigilancelevel* CVigilancelevel::Create(const Level& level)
{
	// インスタンス生成
	CVigilancelevel* pLevel = new CVigilancelevel;

	// ヌルチェック
	if (pLevel == nullptr) return nullptr;

	// 引数を代入
	pLevel->m_Level.pos = level.pos;
	pLevel->m_Level.fWidth = level.fWidth;
	pLevel->m_Level.fHeight = level.fHeight;

	// 初期化が失敗したとき
	if (FAILED(pLevel->Init())) return nullptr;

	return pLevel;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilancelevel::Init(void)
{
	// レベルの位置を代入
	D3DXVECTOR3 pos = m_Level.pos;

	// 親の初期化処理
	CObject2D::Init();

	// 横幅計算
	float fTexPos = m_Level.fWidth / Config::NUM_SCORE;

	// 横幅の分だけ右にずらす
	pos.x = m_Level.pos.x + m_Level.fWidth + Config::INTERVAL_VALUE * 0.5f;

	for (int nCount = 0; nCount < Config::NUM_SCORE; nCount++)
	{
		// ナンバーを生成
		m_apNumber[nCount] = new CNumber;

		// 初期化処理
		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (fTexPos * Config::DIGIT_VALUE * nCount), pos.y, 0.0f),
								 fTexPos,
								 m_Level.fHeight);

		// テクスチャセット
		m_apNumber[nCount]->SetTexture(Config::TEX_NUMBER);

	}

	// レベルの位置を左にずらす
	pos.x = m_Level.pos.x - m_Level.fWidth - Config::INTERVAL_VALUE * 0.5f;

	// レベルテクスチャの設定処理
	SetPos(pos);								// 位置
	SetSize(m_Level.fWidth, m_Level.fHeight);	// サイズ
	SetTexture(Config::TEX_LEVEL);				// テクスチャ名

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilancelevel::Uninit(void)
{
	// 配列ナンバーの破棄
	for (int nCount = 0; nCount < Config::NUM_SCORE; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// nullチェック
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			m_apNumber[nCount] = nullptr;
		}
	}

	// 親の終了処理
	CObject2D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CVigilancelevel::Update(void)
{
	// 配列ナンバーの更新処理
	for (int nCount = 0; nCount < Config::NUM_SCORE; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// nullチェック

			//// 現在の数値
			//int nDigit = 0;

			//// 桁更新
			//m_apNumber[nCount]->SetDigit(nDigit);

			// ナンバーの更新処理
			m_apNumber[nCount]->Update();

		}
	}

	// 親の更新処理
	CObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancelevel::Draw(void)
{
	// 配列ナンバーの描画処理
	for (int nCount = 0; nCount < Config::NUM_SCORE; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// nullチェック
			m_apNumber[nCount]->Draw();
		}
	}

	// 親の描画処理
	CObject2D::Draw();
}