//=========================================================
//
// 進捗ゲージの処理 [ progressgauge.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "progressgauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "gaugeneedle.h"

//=========================================================
// コンストラクタ
//=========================================================
CProgressgauge::CProgressgauge(int nPriority) :CObject2D(nPriority),
m_nTask(NULL),	// タスクをこなした数
m_nAFK(NULL)	// さぼりをこなした数
{

}

//=========================================================
// デストラクタ
//=========================================================
CProgressgauge::~CProgressgauge()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CProgressgauge* CProgressgauge::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// 進捗ゲージのポインタ
	CProgressgauge* pProgressgauge = new CProgressgauge;

	// ヌルチェック
	if (pProgressgauge == nullptr) return nullptr;

	// 進捗ゲージの位置設定
	pProgressgauge->SetPos(pos);
	pProgressgauge->SetWidth(fWidth);
	pProgressgauge->SetHeight(fHeight);
	pProgressgauge->SetCol(COLOR_WHITE);
	pProgressgauge->SetTexture("progressgauge000.png");

	// 初期化が失敗した場合
	if (FAILED(pProgressgauge->Init())) return nullptr;

	return pProgressgauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CProgressgauge::Init(void)
{
	// 親の初期化処理
	CObject2D::Init();

	// 進捗ゲージの情報を取得
	D3DXVECTOR3 pos = GetPos();

	// 高さをずらす
	pos.y += Config::POS_Y_VALUE;

	// 指針の生成処理
	m_gaugeneedle = CGaugeneedle::Create(pos, Config::NEEDLE_WIDTH, Config::NEEDLE_HEIGHT);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CProgressgauge::Uninit(void)
{
	// 親の終了処理
	CObject2D::Uninit();

	// ポインタの破棄
	if (m_gaugeneedle != nullptr)
	{
		// 終了処理
		m_gaugeneedle->Uninit();
		m_gaugeneedle = nullptr;
	}
	
}

//=========================================================
// 更新処理
//=========================================================
void CProgressgauge::Update(void)
{

	// 親の更新処理
	CObject2D::Update();

	// 指針の更新処理
	m_gaugeneedle->Update();
}

//=========================================================
// 描画処理
//=========================================================
void CProgressgauge::Draw(void)
{
	// 親の描画処理
	CObject2D::Draw();

	// 指針の描画処理
	m_gaugeneedle->Draw();
}
