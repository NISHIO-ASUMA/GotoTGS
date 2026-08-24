//=========================================================
//
// 外出タスク処理 [ outsidework.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "outsidework.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "score.h"
#include "progressgauge.h"
#include "gaugeneedle.h"
#include "gamesceneobject.h"
#include "titleuimanager.h"
#include "receptionist.h"

//=========================================================
// コンストラクタ
//=========================================================
COutsideWork::COutsideWork() :CDeskworkUIManager(),
bGoOut(false),
bTask(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
COutsideWork::~COutsideWork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
COutsideWork* COutsideWork::Create(const bool& bUse)
{
	// 外タスクのポインタ
	static COutsideWork pDOCUMENTDeskwork;

	// 設定処理
	pDOCUMENTDeskwork.SetUse(bUse);

	// 初期化が失敗した場合
	if (FAILED(pDOCUMENTDeskwork.Init())) return nullptr;

	return &pDOCUMENTDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT COutsideWork::Init(void)
{
	// 外出できない状態にする
	bGoOut = false;

	// タスク中ではない状態にする
	bTask = false;

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void COutsideWork::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void COutsideWork::Update(void)
{
	// 外出している時の処理
	GoOut();
}

//=========================================================
// 描画処理
//=========================================================
void COutsideWork::Draw(void)
{

}

//=========================================================
// こなした書類タスクの数の設定処理
//=========================================================
void COutsideWork::SetOutside(void)
{
	// 外出できる状態にする
	bGoOut = true;

	// タスク中の状態にする
	bTask = true;
}

//=========================================================
// 外出する時の処理
//=========================================================
void COutsideWork::GoOut(void)
{
	//// スコアのポインタ
	//auto* pScore = CGameSceneObject::GetInstance()->GetScore();

	//// 指針のポインタ
	//CGaugeneedle* pGaugeneedle = CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle();
	//if (pScore == nullptr || pGaugeneedle == nullptr) return;

	//// タスク中ではないなら
	//if (bTask != false)
	//{
	//	// 書類タスクの数の加算処理
	//	AddDOCUMENTTask();

	//	// 外出できない状態にする
	//	bGoOut = false;

	//	return;
	//}

	//// 外出できない状態なら
	//if (bGoOut != true) return;

	//// スコア加算
	//pScore->AddScoreMinus(-1000);

	//// こなしたタスクの数を増やす
	//pGaugeneedle->AddTask();

	////// 外出できない状態にする
	//bGoOut = false;
}
