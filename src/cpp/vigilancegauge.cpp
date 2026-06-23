//=========================================================
//
// 警戒度のゲージ処理 [ vigilancegauge.cpp ]
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

	//// 各設定処理
	//pVigilancegauge->SetPos(vigilancegauge.pos);
	//pVigilancegauge->SetCol(vigilancegauge.col);
	//pVigilancegauge->SetSize(vigilancegauge.fWidth, vigilancegauge.fHeight);
	//pVigilancegauge->SetUV(vigilancegauge.tex.x, vigilancegauge.tex.y);
	//pVigilancegauge->SetAnimFlag(vigilancegauge.isLoop);
	//pVigilancegauge->SetUse(vigilancegauge.bUse);
	//pVigilancegauge->SetTexture(Config::TEXNAME);

	// 初期化が失敗したとき
	if (FAILED(pVigilancegauge->Init())) return nullptr;

	return pVigilancegauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilancegauge::Init(void)
{
	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilancegauge::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CVigilancegauge::Update(void)
{
	//// 使用していないなら
	//if (GetUse() != true) return;

	//// 1フレーム毎のテクスチャ移動量
	//float fTexU = CEasing::SetEase(GetFreamCount(), Config::MAX_FREAM);

	//float fLeftU = CEasing::EaseOutQuad(fTexU);
	//float fRightU = CEasing::EaseOutQuad(fTexU) + GetTex().x;

	//// テクスチャの横移動処理
	//SetTexMoveU(Config::MAX_FREAM, fLeftU, fRightU);

	//// 親の更新処理
	//CAnimationObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	//// 使用していないなら
	//if (GetUse() != true) return;

	//// 親の描画処理
	//CAnimationObject2D::Draw();
}