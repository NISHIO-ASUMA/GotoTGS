//=========================================================
//
// コピー機タスク処理 [ COPYdeskwork.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "COPYdeskwork.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CCOPYDeskwork::CCOPYDeskwork() :CDeskworkUIManager(),
m_nCountTime(0),
m_bTime(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CCOPYDeskwork::~CCOPYDeskwork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CCOPYDeskwork* CCOPYDeskwork::Create(const D3DXVECTOR3& pos)
{
	// PCタスクのポインタ
	static CCOPYDeskwork pPCDeskwork;

	// 設定処理
	pPCDeskwork.SetPos(pos);

	// 初期化が失敗した場合
	if (FAILED(pPCDeskwork.Init())) return nullptr;

	return &pPCDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CCOPYDeskwork::Init(void)
{
	// メンバ変数の初期化
	m_nCountTime = 0;	// 現在のカウント
	m_bTime = false;	// クールタイム

	// UIの情報
	CDeskworkUI::UI ui;
	ui.pos = D3DXVECTOR3(Config::POS_X, Config::POS_Y, 0.0f);
	ui.col = D3DXCOLOR(COLOR_NULL);
	ui.VTXtype = CDeskworkUI::VTXTYPE_CENTER;
	ui.fWidth = Config::UI_WIDTH;
	ui.fHeight = Config::UI_HEIGHT;
	ui.fDigit = Config::VALUE_TEXU;
	ui.nKeytype = (CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX);
	ui.nIdx = TEXTURE_KEY;

	// UIの生成処理
	m_pDeskUI[TEXTURE_KEY] = CDeskworkUI::Create(ui);

	// ゲージ用に設定する
	ui.pos.x -= Config::GAGE_WIDTH * 0.5f;
	ui.pos.y += Config::VALUE_Y;
	ui.VTXtype = CDeskworkUI::VTXTYPE_LEFT;
	ui.fWidth = 0.0f;
	ui.fHeight = Config::GAGE_HEIGHT;
	ui.fDigit = 1.0f;
	ui.nKeytype = CDeskworkUI::DRAWTYPE_NONE;
	ui.nIdx = TEXTURE_GAGE;

	// ゲージUIの生成処理
	m_pDeskUI[TEXTURE_GAGE] = CDeskworkUI::Create(ui);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CCOPYDeskwork::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CCOPYDeskwork::Update(void)
{
	// キーボードのポインタ
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard == nullptr)
	{// ヌルチェック
		return;
	}

	if (m_bTime != false)
	{// クールタイムが始まっているなら

		if (m_nCountTime <= Config::TIME_COOL)
		{// クールタイムを数える
			m_nCountTime++;

			return;
		}

		// タスクをランダムに設定
		m_pDeskUI[TEXTURE_KEY]->SetKeyType((CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX));

		// 色を元に戻す(通常色)
		m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_WHITE);

		// クールタイムを初期化
		m_nCountTime = 0;

		// 横幅を初期化
		m_pDeskUI[TEXTURE_GAGE]->SetWidth(0.0f);

		// クールタイムが始まっていない状態にする
		m_bTime = false;
	}

	// クールタイムが始まっていないなら

	if ((pKeyboard->GetPress(DIK_W) == true && m_pDeskUI[TEXTURE_KEY]->GetKeyType() == CDeskworkUI::DRAWTYPE_W) ||
		(pKeyboard->GetPress(DIK_A) == true && m_pDeskUI[TEXTURE_KEY]->GetKeyType() == CDeskworkUI::DRAWTYPE_A) ||
		(pKeyboard->GetPress(DIK_S) == true && m_pDeskUI[TEXTURE_KEY]->GetKeyType() == CDeskworkUI::DRAWTYPE_S) ||
		(pKeyboard->GetPress(DIK_D) == true && m_pDeskUI[TEXTURE_KEY]->GetKeyType() == CDeskworkUI::DRAWTYPE_D))
	{// 正解を押した時
		// 色をグレーにする
		m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_GLAY);

		// カウントを一つ進める
		m_nCountTime++;

		// 進行度に応じて横幅を計算
		float fWidth = 0.0f;
		fWidth = Config::GAGE_WIDTH * ((float)m_nCountTime / (float)Config::TIME_PUSH);

		// 横幅を設定
		m_pDeskUI[TEXTURE_GAGE]->SetWidth(fWidth);

	}
	else
	{
		// 色を元に戻す(通常色)
		m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_WHITE);
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクUIの更新処理
		m_pDeskUI[nCount]->Update();
	}

	if (m_nCountTime <= Config::TIME_PUSH)
	{
		return;
	}

	// カウントを初期化
	m_nCountTime = 0;

	// クールタイムを始める
	m_bTime = true;

}

//=========================================================
// 描画処理
//=========================================================
void CCOPYDeskwork::Draw(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクUIの描画処理
		m_pDeskUI[nCount]->Draw();
	}
}

//=========================================================
// 透明度の処理
//=========================================================
void CCOPYDeskwork::SetAlphaUI(void)
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

	// 色を不透明にする(通常色)
	m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_WHITE);

	// ゲージの色を赤にする
	m_pDeskUI[TEXTURE_GAGE]->ChangeCol(COLOR_RED);

}