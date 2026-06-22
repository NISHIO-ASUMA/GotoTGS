//=========================================================
//
// チュートリアルのセリフ処理 [ tutoriallines.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutoriallines.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "lines.h"
#include "tutoriallinesBG.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutorialLines::CTutorialLines(int nPriority) :CObject(nPriority),
m_bUse(false),
m_nNowIdx(NULL),
m_pLines(nullptr),
m_pBG(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialLines::~CTutorialLines()
{

}

//=========================================================
// 生成処理
//=========================================================
CTutorialLines* CTutorialLines::Create(const bool& bUse)
{
	// 生成
	CTutorialLines* pTutoriallines = new CTutorialLines;

	// ヌルチェック
	if (pTutoriallines == nullptr) return nullptr;

	// 引数の値を代入
	pTutoriallines->m_bUse = bUse;

	// 初期化が失敗した時
	if (FAILED(pTutoriallines->Init())) return nullptr;

	return pTutoriallines;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialLines::Init(void)
{
	// 現在の番号を初期化
	m_nNowIdx = LINESTYPE_1;

	// 背景の生成処理
	m_pBG = CTutorialLinesBG::Create(D3DXVECTOR3(Config::POS_X, Config::POS_Y, 0.0f),
		D3DXVECTOR2(Config::WIDTH, Config::HEIGHT),
		Config::LINES_BG);

	// セリフの生成処理
	m_pLines = CLines::Create(D3DXVECTOR3(Config::POS_X, Config::POS_Y, 0.0f),
		D3DXVECTOR2(Config::WIDTH, Config::HEIGHT),
		m_LinesType[m_nNowIdx]);

	// 現在の状態を代入する
	m_pBG->SetUse(m_bUse);
	m_pLines->SetUse(m_bUse);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CTutorialLines::Uninit(void)
{
	// 各ポインタをヌルにする
	m_pBG = nullptr;
	m_pLines = nullptr;
}

//=========================================================
// 更新処理
//=========================================================
void CTutorialLines::Update(void)
{
	// マウスのポインタ
	auto* pMouse = CManager::GetInstance()->GetMouse();

	if (pMouse->GetTriggerDown(CInputMouse::MOUSE_LEFT) != true || m_bUse != true)
	{// 左クリックをしていない場合
		return;
	}

	// 現在の番号を一つ進める
	m_nNowIdx++;

	if (m_nNowIdx < LINESTYPE_MAX)
	{// 総数を超えていない場合

		// セリフを一つ進める
		m_pLines->SetTexture(m_LinesType[m_nNowIdx]);

		return;
	}

	// 使用していない状態にする
	m_bUse = false;

	m_pBG->SetUse(m_bUse);
	m_pLines->SetUse(m_bUse);
}

//=========================================================
// 描画処理
//=========================================================
void CTutorialLines::Draw(void)
{

}