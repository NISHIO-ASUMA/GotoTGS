//=========================================================
//
// 警戒ゲージ処理 [ vigilancegauge.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "vigilancegauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CVigilancegauge::CVigilancegauge(int nPriority) :CObject2DMulti(nPriority),
m_bUse(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CVigilancegauge::~CVigilancegauge()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CVigilancegauge* CVigilancegauge::Create(const Vigilancegauge& vigilancegauge)
{
	// インスタンス生成
	CVigilancegauge* pVigilancegauge = new CVigilancegauge;

	// ヌルチェック
	if (pVigilancegauge == nullptr) return nullptr;

	// 初期化が失敗したとき
	if (FAILED(pVigilancegauge->Init())) return nullptr;

	// 各設定処理
	pVigilancegauge->SetPos(vigilancegauge.pos);
	pVigilancegauge->SetCol(vigilancegauge.col);
	pVigilancegauge->SetSize(vigilancegauge.fWidth, vigilancegauge.fHeight);
	pVigilancegauge->SetTexture(Config::TEXNAME_BASE, 0);
	pVigilancegauge->SetTexture(Config::TEXNAME_MULTI, 1);
	pVigilancegauge->m_bUse = vigilancegauge.bUse;

	return pVigilancegauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilancegauge::Init(void)
{
	// 親の初期化処理
	CObject2DMulti::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilancegauge::Uninit(void)
{
	// 親の終了処理
	CObject2DMulti::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CVigilancegauge::Update(void)
{
	// 使用していないなら
	if (m_bUse != true) return;

	// 親の更新処理
	CObject2DMulti::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	// 使用していないなら
	if (m_bUse != true) return;

	// 親の描画処理
	CObject2DMulti::Draw();
}