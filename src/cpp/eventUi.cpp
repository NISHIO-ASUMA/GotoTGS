//=========================================================
//
// イベントUI処理 [ eventUi.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "eventUi.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "eventcutin.h"
#include "alert.h"

//=========================================================
// コンストラクタ
//=========================================================
CEventUI::CEventUI(int nPriority):CObject(nPriority),
m_pEventcutin(nullptr),
m_pAlert(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CEventUI::~CEventUI()
{


}

//=========================================================
// 生成処理
//=========================================================
CEventUI* CEventUI::Create(void)
{
	// インスタンス生成
	CEventUI* pEventUI = new CEventUI;

	// ヌルチェック
	if (pEventUI == nullptr) return nullptr;

	// 初期化が失敗した時
	if (FAILED(pEventUI->Init())) return nullptr;

	return pEventUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CEventUI::Init(void)
{
	// カットインの構造体
	CEventcutin::EventCutin eventcutin;
	eventcutin.pos = D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f);
	eventcutin.col = COLOR_WHITE;
	eventcutin.tex = D3DXVECTOR2(1.0f, 1.0f);
	eventcutin.fWidth = SCREEN_WIDTH;
	eventcutin.fHeight = 300.0f;
	eventcutin.isLoop = false;
	eventcutin.bUse = false;

	// カットインの生成
	m_pEventcutin = CEventcutin::Create(eventcutin);

	// 警告表示の構造体
	CAlert::Alert alert;
	alert.pos = D3DXVECTOR3(HALFWIDTH, HALFHEIGHT + 50.0f, 0.0f);
	alert.col = COLOR_YERROW;
	alert.tex = D3DXVECTOR2(1.0f, 1.0f);
	alert.fWidth = SCREEN_WIDTH;
	alert.fHeight = 200.0f;
	alert.isLoop = false;
	alert.bUse = false;

	// 警告表示の生成
	m_pAlert = CAlert::Create(alert);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CEventUI::Uninit(void)
{
	// 各UIの終了処理
	m_pEventcutin->Uninit();	// カットイン
	m_pAlert->Uninit();			// 警告表示
}

//=========================================================
// 更新処理
//=========================================================
void CEventUI::Update(void)
{
	// 各UIの更新処理
	m_pEventcutin->Update();	// カットイン
	m_pAlert->Update();			// 警告表示
}

//=========================================================
// 描画処理
//=========================================================
void CEventUI::Draw(void)
{
	// 各UIの描画処理
	m_pEventcutin->Draw();		// カットイン
	m_pAlert->Draw();			// 警告表示

}