//=========================================================
//
// 書類タスク処理 [ DOCUMENTdeskwork.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "DOCUMENTdeskwork.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "score.h"
#include "progressgauge.h"
#include "gaugeneedle.h"
#include "gamesceneobject.h"

//=========================================================
// コンストラクタ
//=========================================================
CDOCUMENTDeskwork::CDOCUMENTDeskwork() :CDeskworkUIManager()
{

}

//=========================================================
// デストラクタ
//=========================================================
CDOCUMENTDeskwork::~CDOCUMENTDeskwork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CDOCUMENTDeskwork* CDOCUMENTDeskwork::Create(const bool& bUse)
{
	// 書類タスクのポインタ
	static CDOCUMENTDeskwork pDOCUMENTDeskwork;

	// 設定処理
	pDOCUMENTDeskwork.SetUse(bUse);

	// 初期化が失敗した場合
	if (FAILED(pDOCUMENTDeskwork.Init())) return nullptr;

	return &pDOCUMENTDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDOCUMENTDeskwork::Init(void)
{


	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDOCUMENTDeskwork::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CDOCUMENTDeskwork::Update(void)
{

}

//=========================================================
// 描画処理
//=========================================================
void CDOCUMENTDeskwork::Draw(void)
{

}

//=========================================================
// こなした書類タスクの数の設定処理
//=========================================================
void CDOCUMENTDeskwork::SetDOCUMENTValue(void)
{
	if (GetCOPYTaskNum() <= 0)
	{// 1つもコピー機タスクをこなしていない場合
		return;
	}

	// スコアのポインタ
	auto* pScore = CGameSceneObject::GetInstance()->GetScore();
	// 指針のポインタ
	auto* pGaugeneedle = CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle();

	// スコア加算
	pScore->AddScore(1000);

	// こなしたタスクの数を増やす
	pGaugeneedle->AddTask();

	// 書類タスクの数の加算処理
	AddDOCUMENTTask();
}
