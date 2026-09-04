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
#include "receptionUI.h"
#include "pointobject.h"
#include "outsidetasktimer.h"

//=========================================================
// コンストラクタ
//=========================================================
COutsideWork::COutsideWork() :CDeskworkUIManager(),
m_bGoOutside(false),
m_bTaskNow(false)
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
	m_bGoOutside = false;

	// タスク中ではない状態にする
	m_bTaskNow = false;

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

}

//=========================================================
// 描画処理
//=========================================================
void COutsideWork::Draw(void)
{

}

//=========================================================
// 受付人に話しかけた時の設定処理
//=========================================================
void COutsideWork::SetOutside(void)
{
	// 外出できる状態にする
	m_bGoOutside = true;

	// タスクしている状態にする
	m_bTaskNow = true;
}

//=========================================================
// 外出から帰ってきた時の処理
//=========================================================
void COutsideWork::EndOutside(void)
{
	// タスク中の状態なら
	if (m_bTaskNow != false) return;

	// スコアのポインタ
	auto* pScore = CGameSceneObject::GetInstance()->GetScore();

	// 指針のポインタ
	CGaugeneedle* pGaugeneedle = CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle();
	if (pScore == nullptr || pGaugeneedle == nullptr) return;

	// スコア加算
	pScore->AddScoreMinus(-1000);

	// こなしたタスクの数を増やす
	pGaugeneedle->AddTask();

	// 外出できない状態にする
	m_bGoOutside = false;

}

//=========================================================
// タスク中の処理
//=========================================================
void COutsideWork::TaskSystem(void)
{
	// タスクをしていない状態なら処理しない
	if (m_bTaskNow != true) return;

	// 外出タスクの数の加算処理
	AddOutsideTask();

	// タスクをしていない状態にする
	m_bTaskNow = false;

	// 受付人UIの情報を取得
	auto* pReceptionUI = CGameSceneObject::GetInstance()->GetReceptionUI();

	// 矢印を非表示する
	pReceptionUI->GetPointObject()->SetIsDraw(false);

}
