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
#include "score.h"
#include "ui.h"
#include "progressgauge.h"
#include "gaugeneedle.h"
#include "titleuimanager.h"
#include "gamesceneobject.h"
#include "sound.h"
#include "tutorialobject.h"
#include "tutoriallines.h"

//=========================================================
// コンストラクタ
//=========================================================
CCOPYDeskwork::CCOPYDeskwork() :CDeskworkUIManager(),
m_pDeskUI{}
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
CCOPYDeskwork* CCOPYDeskwork::Create(const D3DXVECTOR3& pos, const bool& bUse)
{
	// PCタスクのポインタ
	static CCOPYDeskwork pPCDeskwork;

	// 設定処理
	pPCDeskwork.SetPos(pos);
	pPCDeskwork.SetUse(bUse);

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

	for (int nCount = 0; nCount < TEXTURE_MAX; nCount++)
	{
		// ポインタの初期化
		m_pDeskUI[nCount] = nullptr;
	}

	// 現在の操作方法を取得
	int nControl = CTitleuiManager::GetInstance()->GetSelectIdx();

	// UIの情報
	CDeskworkUI::UI ui;
	ui.pos = GetPos();
	ui.VTXtype = CDeskworkUI::VTXTYPE_CENTER;
	ui.fWidth = Config::UI_WIDTH;
	ui.fHeight = Config::UI_HEIGHT;
	ui.fDigit = Config::VALUE_TEXU;
	if (nControl == 1)
	{// キーボード操作の場合
		ui.nKeytype = CDeskworkUI::KEYTYPE_BOARD;
		// タスクをランダムに設定
		ui.nKey = (CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX);
	}
	else
	{
		ui.nKeytype = CDeskworkUI::KEYTYPE_PAD;
		// タスクをランダムに設定
		ui.nKey = (CDeskworkUI::KYAPAD)(rand() % CDeskworkUI::KYAPAD_MAX);
	}

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
	// 使えない状態なら
	if (GetCan() != true) return;

	// 親の更新処理
	CDeskworkUIManager::Update();

	// クリアUIのポインタ
	auto* pClear = CDeskworkUIManager::GetClearUI();

	if (pClear == nullptr)
	{// ヌルチェック
		return;
	}

	if (GetTime() != false)
	{// クールタイムが始まっているなら

		// クールタイム中の処理
		if (CoolTime(pClear) != true)
		{
			// クールタイムが終わったなら
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
void CCOPYDeskwork::SetAlphaUI(const bool& bUse)
{
	// クリアUIのポインタ
	auto* pCrear = CDeskworkUIManager::GetClearUI();

	// 使っていいるかどうかを設定する
	SetUse(bUse);

	if (GetPCTaskNum() <= NULL)
	{// PCのタスクを1つもこなしていない場合

		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->SetAlpha(NULL);
		}

		// UIを非表示にする
		pCrear->SetUse(false);

		return;
	}

	if (bUse != true)
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
	// 現在のカウント
	int nCountTime = GetCountTime();

	if (nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		SetCountTime(++nCountTime);

		return false;
	}

	if (m_pDeskUI[TEXTURE_KEY]->GetKeyType() == CDeskworkUI::KEYTYPE_BOARD)
	{// キーボード操作の場合
		// タスクをランダムに設定
		m_pDeskUI[TEXTURE_KEY]->SetKey((CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX));
	}
	else
	{
		// タスクをランダムに設定
		m_pDeskUI[TEXTURE_KEY]->SetKey((CDeskworkUI::KYAPAD)(rand() % CDeskworkUI::KYAPAD_MAX));
	}

	// 色を元に戻す(通常色)
	m_pDeskUI[TEXTURE_KEY]->SetAlpha(1.0f);

	// 横幅を初期化
	m_pDeskUI[TEXTURE_GAGE]->SetWidth(0.0f);

	// クールタイムが始まっていない状態にする
	SetTime(false);

	// クールタイムを初期化
	SetCountTime(NULL);

	// 点滅を止める
	pClear->SetUse(false);
	pClear->SetUseFlash(false);

	// チュートリアル以外なら処理しない
	if (CManager::GetInstance()->GetScene() == CScene::MODE::MODE_TUTORIAL)
	{
		// チュートリアルを進める
		CTutorialObject::GetInstance()->GetTutoriallines()->SetNextTutorial();

		// 使えない状態にする
		SetCan();

		return true;
	}

	return true;
}

//=========================================================
// タスク中の処理
//=========================================================
void CCOPYDeskwork::Task(const auto& pClear)
{
	// 現在のカウント
	int nCountTime = GetCountTime();

	// コントローラーを押した結果の処理
	ControlResult(nCountTime);

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクUIの更新処理
		m_pDeskUI[nCount]->Update();
	}

	if (nCountTime <= Config::TIME_PUSH)
	{
		return;
	}

	// カウントを初期化
	SetCountTime(NULL);

	// クールタイムを始める
	SetTime(true);

	// サウンド再生 ( 成功音 )
	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_TASKCLEAR_SE);

	// 点滅を始める
	pClear->SetUse(true);
	pClear->SetUseFlash(true);

	if (CManager::GetInstance()->GetScene() != CScene::MODE::MODE_TUTORIAL)
	{// チュートリアル以外での処理

		// スコアのポインタ
		auto* pScore = CGameSceneObject::GetInstance()->GetScore();
		// 指針のポインタ
		auto* pGaugeneedle = CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle();

		if (pScore == nullptr || pGaugeneedle == nullptr)
		{// ヌルチェック
			return;
		}

		// スコア加算
		pScore->AddScore(-100);

		// こなしたタスクの数を増やす
		pGaugeneedle->AddTask();

	}

	// こなしたコピー機タスクの数を一つ増やす
	AddCOPYTask();

	// こなしたPCタスクの数を1つ減らす
	MinusPCTask();
}

//=========================================================
// コントローラーを押した結果の処理
//=========================================================
void CCOPYDeskwork::ControlResult(int& nCount)
{
	// キーボードのポインタ
	auto* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	// パッドのポインタ
	auto* pJoypad = CManager::GetInstance()->GetJoyPad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{// ヌルチェック
		return;
	}

	//************************************************************
	// ADD : 西尾 現在の操作のタイプに応じて入力取得を変化させる
	//************************************************************
	int nControl = CTitleuiManager::GetInstance()->GetSelectIdx();

	// 入力制御フラグ
	bool isInputControl = false;

	switch (nControl)
	{
	case CONTROL::CONTROL_NONE:
		break;

	case CONTROL::CONTROL_KEY:
		if ((pKeyboard->GetPress(DIK_W) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_W) ||
			(pKeyboard->GetPress(DIK_A) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_A) ||
			(pKeyboard->GetPress(DIK_S) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_S) ||
			(pKeyboard->GetPress(DIK_D) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYBOARD_D))
		{
			isInputControl = true;
		}
		break;

	case CONTROL::CONTROL_PAD:
		if ((pJoypad->GetPress(CJoyPad::JOYKEY_A) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYPAD_A) ||
			(pJoypad->GetPress(CJoyPad::JOYKEY_B) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYPAD_B) ||
			(pJoypad->GetPress(CJoyPad::JOYKEY_X) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYPAD_X) ||
			(pJoypad->GetPress(CJoyPad::JOYKEY_Y) == true && m_pDeskUI[TEXTURE_KEY]->GetKey() == CDeskworkUI::KEYPAD_Y))
		{
			isInputControl = true;
		}
		break;

	default:
		break;
	}

	if (isInputControl)
	{// 正解を押した時
		// 色をグレーにする
		m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_GLAY);

		// カウントを一つ進める
		nCount++;
		SetCountTime(nCount);

		// 進行度に応じて横幅を計算
		float fWidth = 0.0f;
		fWidth = Config::GAGE_WIDTH * ((float)nCount / (float)Config::TIME_PUSH);

		// 横幅を設定
		m_pDeskUI[TEXTURE_GAGE]->SetWidth(fWidth);
	}
	else
	{
		// 色を元に戻す(通常色)
		m_pDeskUI[TEXTURE_KEY]->ChangeCol(COLOR_WHITE);
	}
}