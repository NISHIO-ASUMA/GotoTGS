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
CVigilancegauge::CVigilancegauge(int nPriority) :CObject2DMulti(nPriority)
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
CVigilancegauge* CVigilancegauge::Create(const Gauge& gauge)
{
	// インスタンス生成
	CVigilancegauge* pGauge = new CVigilancegauge;

	// ヌルチェック
	if (pGauge == nullptr) return nullptr;

	// 初期化が失敗したとき
	if (FAILED(pGauge->Init())) return nullptr;

	// 各設定処理
	pGauge->SetPos(gauge.pos);
	pGauge->SetCol(gauge.col);
	pGauge->SetSize(gauge.fWidth, gauge.fHeight);
	pGauge->SetTexture(Config::TEXNAME_MULTI, 0);
	pGauge->SetTexture(Config::TEXNAME_BASE, 1);

	return pGauge;
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
	// 親の更新処理
	CObject2DMulti::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	// 親の描画処理
	CObject2DMulti::Draw();
}