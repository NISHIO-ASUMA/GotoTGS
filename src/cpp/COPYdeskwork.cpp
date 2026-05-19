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
CCOPYDeskwork::CCOPYDeskwork()
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
	// クールタイムが始まっていない状態にする
	m_bTime = false;

	// 位置
	D3DXVECTOR3 pos = D3DXVECTOR3(Config::POS_X, Config::POS_Y, 0.0f);

	// キーの種類
	CDeskworkUI::KEYTYPE keytype = CDeskworkUI::DRAWTYPE_A;

	// タスクをランダムに設定
	keytype = (CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX);

	// UIの生成処理
	m_pDeskUI = CDeskworkUI::Create(pos, Config::UI_WIDTH, Config::UI_HEIGHT, keytype, NULL);

	// 色を透明にする
	m_pDeskUI->ChangeCol(COLOR_NULL);

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
		m_pDeskUI->SetKyeType((CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX));

		// 色を元に戻す(通常色)
		m_pDeskUI->ChangeCol(COLOR_WHITE);

		// クールタイムを初期化
		m_nCountTime = 0;

		// クールタイムが始まっていない状態にする
		m_bTime = false;
	}

	// クールタイムが始まっていないなら
	// 現在のタスクUIの更新処理
	m_pDeskUI->Update();

	if ((pKeyboard->GetPress(DIK_W) == true && m_pDeskUI->GetKyeType() == CDeskworkUI::DRAWTYPE_W) ||
		(pKeyboard->GetPress(DIK_A) == true && m_pDeskUI->GetKyeType() == CDeskworkUI::DRAWTYPE_A) ||
		(pKeyboard->GetPress(DIK_S) == true && m_pDeskUI->GetKyeType() == CDeskworkUI::DRAWTYPE_S) ||
		(pKeyboard->GetPress(DIK_D) == true && m_pDeskUI->GetKyeType() == CDeskworkUI::DRAWTYPE_D))
	{// 正解を押した時
		// 色をグレーにする
		m_pDeskUI->ChangeCol(COLOR_GLAY);

		// カウントを一つ進める
		m_nCountTime++;
	}
	else
	{
		// 色を元に戻す(通常色)
		m_pDeskUI->ChangeCol(COLOR_WHITE);
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
	// タスクUIの描画処理
	m_pDeskUI->Draw();
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

		// 色を透明にする
		m_pDeskUI->ChangeCol(COLOR_NULL);

		return;
	}

	// 色を不透明にする(通常色)
	m_pDeskUI->ChangeCol(COLOR_WHITE);
}