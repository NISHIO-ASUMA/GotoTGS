//=========================================================
//
// PCタスク処理 [ PCdeskwork.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "PCdeskwork.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "gamesceneobject.h"
#include "score.h"
#include "progressgauge.h"
#include "ui.h"

//=========================================================
// コンストラクタ
//=========================================================
CPCDeskwork::CPCDeskwork() :CDeskworkUIManager()
{

}

//=========================================================
// デストラクタ
//=========================================================
CPCDeskwork::~CPCDeskwork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CPCDeskwork* CPCDeskwork::Create(const D3DXVECTOR3& pos)
{
	// PCタスクのポインタ
	static CPCDeskwork pPCDeskwork;

	// 設定処理
	pPCDeskwork.SetPos(pos);

	// 初期化が失敗した場合
	if (FAILED(pPCDeskwork.Init())) return nullptr;

	return &pPCDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CPCDeskwork::Init(void)
{
	// クールタイムが始まっていない状態にする
	m_bTime = false;

	// 現在選択されているUIの番号の初期化
	m_nNowIdx = 0;

	// 位置
	D3DXVECTOR3 pos = GetPos();
	pos.x = GetPos().x - Config::VALUE_WIDTH;

	// UIの情報
	CDeskworkUI::UI ui;
	ui.pos = pos;
	ui.col = D3DXCOLOR(COLOR_NULL);
	ui.VTXtype = CDeskworkUI::VTXTYPE_CENTER;
	ui.fWidth = Config::UI_WIDTH;
	ui.fHeight = Config::UI_HEIGHT;
	ui.fDigit = Config::VALUE_TEXU;

	for (ui.nIdx = 0; ui.nIdx < Config::UI_NUM; ui.nIdx++)
	{
		// タスクをランダムに設定
		ui.nKeytype = (CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX);

		// UIの生成処理
		m_pDeskUI[ui.nIdx] = CDeskworkUI::Create(ui);

		// UIの位置をずらす
		ui.pos.x += Config::VALUE_WIDTH;
	}

	// 親の初期化処理
	CDeskworkUIManager::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CPCDeskwork::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CPCDeskwork::Update(void)
{
	// 親の更新処理
	CDeskworkUIManager::Update();

	// クリアUIのポインタ
	auto* pClear = CDeskworkUIManager::GetClearUI();

	if (m_bTime != false)
	{// クールタイムが始まったなら

		// クールタイム中の処理
		if (CoolTime(pClear) != true)
		{
			// クールタイム中なら
			return;
		}
	}

	// クールタイムが始まっていないなら
	// 現在のタスクUIの更新処理
	m_pDeskUI[m_nNowIdx]->Update();

	// タスク中の処理
	Task(pClear);
}

//=========================================================
// 描画処理
//=========================================================
void CPCDeskwork::Draw(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの描画処理
		m_pDeskUI[nCount]->Draw();
	}
}

//=========================================================
// 透明度の処理
//=========================================================
void CPCDeskwork::SetAlphaUI(void)
{
	// クリアUIのポインタ
	auto* pCrear = CDeskworkUIManager::GetClearUI();

	if (pCrear == nullptr)
	{// ヌルチェック
		return;
	}

	// 使っていいるかどうかを設定する
	SetUse(GetUse() ? false : true);

	if (GetUse() != true)
	{// 使っていない状態の場合

		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->ChangeCol(COLOR_NULL);
		}

		// UIを非表示にする
		pCrear->SetUse(false);

		return;
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		if (nCount < m_nNowIdx)
		{
			// 色を半透明にする
			m_pDeskUI[nCount]->ChangeCol(COLOR_HALFVALUE);

			continue;
		}

		// 色を不透明にする(通常色)
		m_pDeskUI[nCount]->ChangeCol(COLOR_WHITE);
	}
}

//=========================================================
// クールタイム中の処理
//=========================================================
bool CPCDeskwork::CoolTime(const auto& pClear)
{
	if (m_nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		m_nCountTime++;

		return false;
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクをランダムに設定
		m_pDeskUI[nCount]->SetKeyType((CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX));

		// 色を元に戻す(通常色)
		m_pDeskUI[nCount]->ChangeCol(COLOR_WHITE);
	}

	// 現在選択している番号を初期化
	m_nNowIdx = 0;

	// クールタイムを初期化
	m_nCountTime = 0;

	// クールタイムが始まっていない状態にする
	m_bTime = false;

	// 点滅を止める
	pClear->SetUse(false);
	pClear->SetUseFlash(false);

	return true;
}

//=========================================================
// タスク中の処理
//=========================================================
void CPCDeskwork::Task(const auto& pClear)
{
	// キーボードのポインタ
	auto* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	// スコアのポインタ
	auto* pScore = CGameSceneObject::GetInstance()->GetScore();
	// 進捗ゲージのポインタ
	auto* pProgressgauge = CGameSceneObject::GetInstance()->GetProgressgauge();

	if (pKeyboard == nullptr || pScore == nullptr || pProgressgauge == nullptr)
	{// ヌルチェック
		return;
	}

	if ((pKeyboard->GetTrigger(DIK_W) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() == CDeskworkUI::DRAWTYPE_W) ||
		(pKeyboard->GetTrigger(DIK_A) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() == CDeskworkUI::DRAWTYPE_A) ||
		(pKeyboard->GetTrigger(DIK_S) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() == CDeskworkUI::DRAWTYPE_S) ||
		(pKeyboard->GetTrigger(DIK_D) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() == CDeskworkUI::DRAWTYPE_D))
	{// 正解を押した時

		// 色を半透明にする
		m_pDeskUI[m_nNowIdx]->ChangeCol(COLOR_HALFVALUE);

		// 次のタスクに移る
		m_nNowIdx++;
	}
	else if ((pKeyboard->GetTrigger(DIK_W) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() != CDeskworkUI::DRAWTYPE_W) ||
			(pKeyboard->GetTrigger(DIK_A) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() != CDeskworkUI::DRAWTYPE_A) ||
			(pKeyboard->GetTrigger(DIK_S) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() != CDeskworkUI::DRAWTYPE_S) ||
			(pKeyboard->GetTrigger(DIK_D) == true && m_pDeskUI[m_nNowIdx]->GetKeyType() != CDeskworkUI::DRAWTYPE_D))
	{// 不正解を押した時

		// 色を赤にする
		m_pDeskUI[m_nNowIdx]->ChangeCol(COLOR_RED);

		// クールタイムを始める
		m_bTime = true;

		return;
	}

	// タスクが終わっていないなら
	if (m_nNowIdx < Config::UI_NUM)
	{
		return;
	}

	// クールタイムを始める
	m_bTime = true;

	// こなしたタスクの数を増やす
	pProgressgauge->AddTask();

	// スコア加算
	pScore->AddScore(100);

	// 点滅を始める
	pClear->SetUse(true);
	pClear->SetUseFlash(true);

	// こなしたPCタスクの数を一つ増やす
	AddPCTask();

}