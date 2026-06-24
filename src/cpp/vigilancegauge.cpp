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
CVigilancegauge::CVigilancegauge(int nPriority)
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

	// 各設定処理
	pVigilancegauge->SetPos(vigilancegauge.pos);
	pVigilancegauge->SetCol(vigilancegauge.col);
	pVigilancegauge->SetSize(vigilancegauge.fWidth, vigilancegauge.fHeight);
	pVigilancegauge->SetUV(vigilancegauge.tex.x, vigilancegauge.tex.y);
	pVigilancegauge->SetAnimFlag(vigilancegauge.isLoop);
	pVigilancegauge->SetUse(vigilancegauge.bUse);
	pVigilancegauge->SetTexture(Config::TEXNAME);

	// 初期化が失敗したとき
	if (FAILED(pVigilancegauge->Init())) return nullptr;

	return pVigilancegauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilancegauge::Init(void)
{
	// 親の初期化処理
	CAnimationObject2D::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilancegauge::Uninit(void)
{
	// 親の終了処理
	CAnimationObject2D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CVigilancegauge::Update(void)
{
	// 使用していないなら
	if (GetUse() != true) return;

	// 親の更新処理
	CAnimationObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	// 使用していないなら
	if (GetUse() != true) return;

	// 親の描画処理
	CAnimationObject2D::Draw();
}