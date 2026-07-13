//=========================================================
//
// ゲージの指針の処理 [ gaugeneedle.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gaugeneedle.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CGaugeneedle::CGaugeneedle(int nPriority) :CObject2DRotation(nPriority),
m_nTask(NULL),	// タスクをこなした数
m_nAFK(NULL)	// さぼりをこなした数
{

}

//=========================================================
// デストラクタ
//=========================================================
CGaugeneedle::~CGaugeneedle()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CGaugeneedle* CGaugeneedle::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// ゲージの指針のポインタ
	CGaugeneedle* pGaugeneedle = new CGaugeneedle;

	// ヌルチェック
	if (pGaugeneedle == nullptr) return nullptr;

	// ゲージの指針の設定
	pGaugeneedle->SetPos(pos);														// 位置
	pGaugeneedle->SetPivot(D3DXVECTOR3(Config::PIVOT_X, Config::PIVOT_Y, 0.0f));	// 回転の基準点
	pGaugeneedle->SetAngle(0.0f);													// 角度
	pGaugeneedle->SetCol(COLOR_WHITE);												// 色
	pGaugeneedle->SetSize(D3DXVECTOR2(fWidth, fHeight));							// サイズ
	pGaugeneedle->SetTexture("needle000.png");										// テクスチャ

	// 初期化が失敗した場合
	if (FAILED(pGaugeneedle->Init())) return nullptr;

	return pGaugeneedle;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CGaugeneedle::Init(void)
{
	// 親の初期化処理
	CObject2DRotation::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CGaugeneedle::Uninit(void)
{
	// 親の終了処理
	CObject2DRotation::Uninit();

}

//=========================================================
// 更新処理
//=========================================================
void CGaugeneedle::Update(void)
{
	// 親の更新処理
	CObject2DRotation::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CGaugeneedle::Draw(void)
{
	// 親の描画処理
	CObject2DRotation::Draw();

}