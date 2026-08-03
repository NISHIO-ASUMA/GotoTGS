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
#include "overworkresultobject.h"
#include "input.h"
#include "fade.h"
#include "title.h"

//#include "overworkresultmanager.h"

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
	// オブジェクト生成
	COverWorkResultObject::GetInstance()->Init();

	return S_OK;
}
//==========================================================
// 終了処理
//==========================================================
void COverWorkResult::Uninit(void)
{
	// オブジェクト終了
	COverWorkResultObject::GetInstance()->Uninit();
}
//==========================================================
// 更新処理
//==========================================================
void COverWorkResult::Update(void)
{
	// キー遷移
	CFade* pFade = CManager::GetInstance()->GetFade();

	// トリガーで遷移
	if (pFade != nullptr && (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_SPACE) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_BACK)))
	{
		// 画面切り替え
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