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
#include "gamesceneobject.h"
#include "score.h"
#include "progressgauge.h"
#include "ui.h"

//=========================================================
// コンストラクタ
//=========================================================
CCOPYDeskwork::CCOPYDeskwork() :CDeskworkUIManager(),
m_pDeskUI{},
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
	// 乱数の種
	srand((unsigned int)time(0));

	// メンバ変数の初期化
	m_nCountTime = 0;	// 現在のカウント
	m_bTime = false;	// クールタイム

	// UIの情報
	CDeskworkUI::UI ui;
	ui.pos = GetPos();
	ui.VTXtype = CDeskworkUI::VTXTYPE_CENTER;
	ui.fWidth = Config::UI_WIDTH;
	ui.fHeight = Config::UI_HEIGHT;
	ui.fDigit = Config::VALUE_TEXU;
	ui.nKeytype = CDeskworkUI::KEYTYPE_BOARD;
	ui.nKey = (CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX);
	ui.nIdx = TEXTURE_KEY;

	// UIの生成処理
	m_pDeskUI[TEXTURE_KEY] = CDeskworkUI::Create(ui);

	// ゲージ用に設定する
	ui.pos.x -= (Config::GAGE_WIDTH * 0.5f + Config::VALUE_X);
	ui.pos.y -= Config::VALUE_Y;
	ui.VTXtype = CDeskworkUI::VTXTYPE_LEFT;
	ui.fWidth = 0.0f;
	ui.fHeight = Config::GAGE_HEIGHT;
	ui.fDigit = 1.0f;
	ui.nKeytype = CDeskworkUI::KEYTYPE_MAX;
	ui.nKey = CDeskworkUI::KEYBOARD_NONE;
	ui.nIdx = TEXTURE_GAGE;

	// ゲージUIの生成処理
	m_pDeskUI[TEXTURE_GAGE] = CDeskworkUI::Create(ui);

	// 親の初期化処理
	CDeskworkUIManager::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CCOPYDeskwork::Uninit(void)
{
	// 親の終了処理
	CDeskworkUIManager::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CCOPYDeskwork::Update(void)
{
	// 親の更新処理
	CDeskworkUIManager::Update();

	// クリアUIのポインタ
	auto* pClear = CDeskworkUIManager::GetClearUI();

	if (pClear == nullptr)
	{// ヌルチェック
		return;
	}

	if (m_bTime != false)
	{// クールタイムが始まっているなら

		// クールタイム中の処理
		if (CoolTime(pClear) != true)
		{
			// クールタイム中なら
			return;
		}
	}

	if (GetPCTaskNum() <= 0)
	{// こなしたPCタスクが1つもないとき
		return;
	}

	// クールタイムが始まっていないなら
	// タスク中の処理
	Task(pClear);
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
	// クリアUIのポインタ
	auto* pCrear = CDeskworkUIManager::GetClearUI();

	// 使っていいるかどうかを設定する
	SetUse(GetUse() ? false : true);

	if (GetPCTaskNum() <= NULL)
	{// PCのタスクを1つもこなしていない場合

		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->SetAlpha(NULL);
		}

		return;
	}

	if (GetUse() != true)
	{// 使っていない状態の場合

		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->SetAlpha(NULL);
		}

		// UIを非表示にする
		pCrear->SetUse(false);

		return;
	}

	// 色を不透明にする(通常色)
	m_pDeskUI[TEXTURE_KEY]->SetAlpha(1.0f);

	// ゲージの色を赤にする
	m_pDeskUI[TEXTURE_GAGE]->ChangeCol(COLOR_RED);
}

//=========================================================
// クールタイム中の処理
//=========================================================
bool CCOPYDeskwork::CoolTime(const auto& pClear)
{
	if (m_nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		m_nCountTime++;

		return false;
	}

	// タスクをランダムに設定
	m_pDeskUI[TEXTURE_KEY]->SetKey((CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX));

	// 色を元に戻す(通常色)
	m_pDeskUI[TEXTURE_KEY]->SetAlpha(1.0f);

	// クールタイムを初期化
	m_nCountTime = 0;

	// 横幅を初期化
	m_pDeskUI[TEXTURE_GAGE]->SetWidth(0.0f);

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
void CCOPYDeskwork::Task(const auto& pClear)
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

	if ((pKeyboard->GetPress(DIK_W) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_W) ||
		(pKeyboard->GetPress(DIK_A) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_A) ||
		(pKeyboard->GetPress(DIK_S) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_S) ||
		(pKeyboard->GetPress(DIK_D) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_D))
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

	// スコア加算
	pScore->AddScore(100);

	// こなしたさぼりの数を増やす（仮）
	pProgressgauge->AddAFK();

	// 点滅を始める
	pClear->SetUse(true);
	pClear->SetUseFlash(true);

	// こなしたコピー機タスクの数を一つ増やす
	AddCOPYTask();

	// こなしたPCタスクの数を1つ減らす
	MinusPCTask();

}