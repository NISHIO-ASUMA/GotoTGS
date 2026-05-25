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
CDOCUMENTDeskwork* CDOCUMENTDeskwork::Create(const D3DXVECTOR3& pos)
{
	// 書類タスクのポインタ
	static CDOCUMENTDeskwork pDOCUMENTDeskwork;

	// 設定処理
	pDOCUMENTDeskwork.SetPos(pos);

	// 初期化が失敗した場合
	if (FAILED(pDOCUMENTDeskwork.Init())) return nullptr;

	return &pDOCUMENTDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDOCUMENTDeskwork::Init(void)
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
		ui.nKeytype = (CDeskworkUI::DOCUMENT)(rand() % CDeskworkUI::DOCUMENT_MAX);

		// UIの生成処理
		m_pDeskUI[ui.nIdx] = CDeskworkUI::Create(ui);

		// UIの位置をずらす
		ui.pos.x += Config::VALUE_WIDTH;
	}

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
	// キーボードのポインタ
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard == nullptr)
	{// ヌルチェック
		return;
	}

	if (m_bTime != false)
	{// クールタイムが始まったなら

		if (m_nCountTime <= Config::TIME_COOL)
		{// クールタイムを数える
			m_nCountTime++;

			return;
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

	}

	// クールタイムが始まっていないなら
	// 現在のタスクUIの更新処理
	m_pDeskUI[m_nNowIdx]->Update();

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

	// タスクが終わっていないなら
	if (m_nNowIdx < Config::UI_NUM)
	{
		return;
	}

	// クールタイムを始める
	m_bTime = true;
}

//=========================================================
// 描画処理
//=========================================================
void CDOCUMENTDeskwork::Draw(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの描画処理
		m_pDeskUI[nCount]->Draw();
	}
}

//=========================================================
// 透明度の処理
//=========================================================
void CDOCUMENTDeskwork::SetAlphaUI(void)
{
	// 使っていいるかどうかを設定する
	SetUse(GetUse() ? false : true);

	if (GetUse() != true)
	{// 使っていない状態の場合

		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->ChangeCol(COLOR_NULL);
		}

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