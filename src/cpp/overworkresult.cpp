//==========================================================
//
// タスクしすぎで過労死するシーン処理 [ ovreworkresult.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "overworkresult.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "overworkresultobject.h"
#include "overworkresultmanager.h"

//==========================================================
// コンストラクタ
//==========================================================
COverWorkResult::COverWorkResult() : CScene(CScene::MODE_OVERWORK)
{

}
//==========================================================
// デストラクタ
//==========================================================
COverWorkResult::~COverWorkResult()
{

}
//==========================================================
// 初期化処理
//==========================================================
HRESULT COverWorkResult::Init(void)
{
	// 管理クラス生成
	COverWorkResultManager::GetInstance()->Init();

	// オブジェクト生成
	COverWorkResultObject::GetInstance()->Init();

	return S_OK;
}
//==========================================================
// 終了処理
//==========================================================
void COverWorkResult::Uninit(void)
{
	// 管理クラス終了
	COverWorkResultManager::GetInstance()->Uninit();

	// オブジェクト終了
	COverWorkResultObject::GetInstance()->Uninit();
}
//==========================================================
// 更新処理
//==========================================================
void COverWorkResult::Update(void)
{
	// フェード取得
	CFade* pFade = CManager::GetInstance()->GetFade();

	// トリガーで遷移
	if (pFade != nullptr && (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_A) || 
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START)))
	{
		// タイトル画面に遷移
		pFade->SetFade(std::make_unique<CTitle>());
		return;
	}
}
//==========================================================
// 描画処理
//==========================================================
void COverWorkResult::Draw(void)
{

}