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
#include "score.h"
#include "ui.h"
#include "easing.h"
#include "progressgauge.h"
#include "gaugeneedle.h"
#include "titleuimanager.h"
#include "gamesceneobject.h"
#include "sound.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace PC_DESKWORK
{
	constexpr float SHAKE_POW = 40.0f;		// 振動力
};

//=========================================================
// コンストラクタ
//=========================================================
CPCDeskwork::CPCDeskwork() :CDeskworkUIManager(),
m_nNowIdx(NULL),
m_bFalse(false),
m_BasePos(VECTOR3_NULL),
m_Offsetpos(VECTOR3_NULL)
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
CPCDeskwork* CPCDeskwork::Create(const D3DXVECTOR3& pos, const bool& bUse)
{
	// PCタスクのポインタ
	static CPCDeskwork pPCDeskwork;

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
HRESULT CPCDeskwork::Init(void)
{
	// 現在の操作方法を取得
	int nControl = CTitleuiManager::GetInstance()->GetSelectIdx();

	// 失敗していない状態にする
	m_bFalse = false;

	// 現在選択されているUIの番号の初期化
	m_nNowIdx = 0;

	// 位置
	m_Offsetpos = GetPos();
	m_Offsetpos.x = GetPos().x - Config::VALUE_WIDTH;

	// UIの情報
	CDeskworkUI::UI ui;
	ui.pos = m_Offsetpos;
	ui.VTXtype = CDeskworkUI::VTXTYPE_CENTER;
	ui.fWidth = Config::UI_WIDTH;
	ui.fHeight = Config::UI_HEIGHT;
	ui.fDigit = Config::VALUE_TEXU;

	if (nControl == 1)
	{// キーボード操作の場合
		ui.nKeytype = CDeskworkUI::KEYTYPE_BOARD;
	}
	else
	{
		ui.nKeytype = CDeskworkUI::KEYTYPE_PAD;
	}

	for (ui.nIdx = 0; ui.nIdx < Config::UI_NUM; ui.nIdx++)
	{
		// ポインタの初期化
		m_pDeskUI[ui.nIdx] = nullptr;

		if (nControl == 1)
		{// キーボード操作の場合
			// タスクをランダムに設定
			ui.nKey = (CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX);
		}
		else
		{
			// タスクをランダムに設定
			ui.nKey = (CDeskworkUI::KYAPAD)(rand() % CDeskworkUI::KYAPAD_MAX);
		}

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
	// 親の終了処理
	CDeskworkUIManager::Uninit();
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

	if (GetTime() != false || m_bFalse != false)
	{// 失敗したかクールタイムが始まったなら

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

	if (GetUse() != false)
	{
		// タスク中の処理
		Task(pClear);
	}
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
void CPCDeskwork::SetAlphaUI(const bool& bUse)
{
	// クリアUIのポインタ
	auto* pCrear = CDeskworkUIManager::GetClearUI();

	if (pCrear == nullptr)
	{// ヌルチェック
		return;
	}

	// 使っていいるかどうかを設定する
	SetUse(bUse);

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

	// 使っている場合
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		if (nCount < m_nNowIdx)
		{
			// 色を半透明にする
			m_pDeskUI[nCount]->SetAlpha(0.5f);

			continue;
		}

		// 色を不透明にする(通常色)
		m_pDeskUI[nCount]->SetAlpha(1.0f);
	}
}

//=========================================================
// クールタイム中の処理
//=========================================================
bool CPCDeskwork::CoolTime(const auto& pClear)
{
	// 現在のカウント
	int nCountTime = GetCountTime();

	if (nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		nCountTime++;
		SetCountTime(nCountTime);

		if (m_bFalse != false)
		{// 失敗している場合
			// 1フレーム毎の割合
			float t = CEasing::SetEase(nCountTime, Config::TIME_COOL);
			float EasingValue = CEasing::EaseOutElastic(t);

			// 失敗したものだけ動かす
			D3DXVECTOR3 targetPos = m_pDeskUI[m_nNowIdx]->GetPos();

			// イージングを適用
			targetPos.x += PC_DESKWORK::SHAKE_POW * (1.0f - EasingValue);

			// 対象UIに座標を設定
			m_pDeskUI[m_nNowIdx]->SetPos(targetPos);
		}

		return false;
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// 元の基準の位置に戻す
		D3DXVECTOR3 normalPos = GetPos();
		normalPos.x = (GetPos().x - Config::VALUE_WIDTH) + (Config::VALUE_WIDTH * nCount);
		m_pDeskUI[nCount]->SetPos(normalPos);

		if (m_pDeskUI[nCount]->GetKeyType() == CDeskworkUI::KEYTYPE_BOARD)
		{// キーボード操作の場合
			// タスクをランダムに設定
			m_pDeskUI[nCount]->SetKey((CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX));
		}
		else
		{
			// タスクをランダムに設定
			m_pDeskUI[nCount]->SetKey((CDeskworkUI::KYAPAD)(rand() % CDeskworkUI::KYAPAD_MAX));
		}


		if (GetUse() != true)
		{
			// 現在の透明度を取得
			float fAlpha = m_pDeskUI[nCount]->GetAlpha();

			// 色を元に戻す(通常色)
			m_pDeskUI[nCount]->ChangeCol(COLOR_WHITE);

			// 透明度だけ引き継ぐ
			m_pDeskUI[nCount]->SetAlpha(fAlpha);

		}
		else
		{
			// 色を元に戻す(通常色)
			m_pDeskUI[nCount]->ChangeCol(COLOR_WHITE);
		}
	}

	// 現在選択している番号を初期化
	m_nNowIdx = 0;

	// 失敗していない状態にする
	m_bFalse = false;

	// クールタイムが始まっていない状態にする
	SetTime(false);

	// クールタイムを初期化
	SetCountTime(NULL);

	// 点滅を止める
	pClear->SetUse(false);
	pClear->SetUseFlash(false);

	// チュートリアル以外なら処理しない
	if (CManager::GetInstance()->GetScene() != CScene::MODE::MODE_TUTORIAL)
		return true;

	// チュートリアルが終わった状態にする
	CDeskworkUIManager::SetTutorial();

	return true;
}

//=========================================================
// タスク中の処理
//=========================================================
void CPCDeskwork::Task(const auto& pClear)
{
	// サウンドクラスのポインタ
	auto* pSound = CManager::GetInstance()->GetSound();

	if (ControlResult() != true)
	{// 間違ったボタンを押した場合
		return;
	}

	if (m_nNowIdx < Config::UI_NUM)
	{// タスクが終わっていないなら
		return;
	}

	// クールタイムを始める
	SetTime(true);

	// サウンド再生 ( 成功音 )
	pSound->Play(CSound::SOUND_LABEL_TASKCLEAR_SE);

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
		pScore->AddScore(-1000);

		// こなしたタスクの数を増やす
		pGaugeneedle->AddTask();

	}


	// こなしたPCタスクの数を一つ増やす
	AddPCTask();

}

//=========================================================
// コントローラーを押した結果の処理
//=========================================================
bool CPCDeskwork::ControlResult(void)
{
	// キーボードのポインタ
	auto* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	// パッドのポインタ
	auto* pJoypad = CManager::GetInstance()->GetJoyPad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{// ヌルチェック
		return false;
	}

	//**************************************
	// ADD 西尾 : リファクタリングと判定ロジックの修正
	//**************************************
	
	// プレイヤーの操作キーインデックス取得
	int nControl = CTitleuiManager::GetInstance()->GetSelectIdx();

	// 現在のタスクのUIのポインタ取得
	auto* pCurrentUI = m_pDeskUI[m_nNowIdx];

	// 現在要求されている正解のキー情報を格納
	auto targetKey = pCurrentUI->GetKey();

	// 入力情報を保持するローカル構造体
	struct InputMapping
	{
		bool isTriggered;	// 入力されたフラグ
		int requiredKey;	// 必要なキー
	};

	// 現在キーの入力判定を持つ動的配列
	std::vector<InputMapping> activeInputs;

	// 現在の操作タイプに応じた入力だけを判定する
	if (nControl == CONTROL::CONTROL_KEY)
	{// キーボード
		activeInputs =
		{
			{ pKeyboard->GetTrigger(DIK_W), CDeskworkUI::KEYBOARD_W },
			{ pKeyboard->GetTrigger(DIK_A), CDeskworkUI::KEYBOARD_A },
			{ pKeyboard->GetTrigger(DIK_S), CDeskworkUI::KEYBOARD_S },
			{ pKeyboard->GetTrigger(DIK_D), CDeskworkUI::KEYBOARD_D }
		};
	}
	else if (nControl == CONTROL::CONTROL_PAD)
	{// ゲームパッド
		activeInputs =
		{
			{ pJoypad->GetTrigger(CJoyPad::JOYKEY_A), CDeskworkUI::KEYPAD_A },
			{ pJoypad->GetTrigger(CJoyPad::JOYKEY_B), CDeskworkUI::KEYPAD_B },
			{ pJoypad->GetTrigger(CJoyPad::JOYKEY_X), CDeskworkUI::KEYPAD_X },
			{ pJoypad->GetTrigger(CJoyPad::JOYKEY_Y), CDeskworkUI::KEYPAD_Y }
		};
	}

	// キー入力判定フラグ
	bool hasAnyInput = false;
	bool isCorrectInput = false;

	// 配列を見て、正解と一致の場合ループを終了する
	for (const auto& input : activeInputs)
	{
		if (input.isTriggered)
		{
			hasAnyInput = true;

			if (input.requiredKey == targetKey)
			{
				isCorrectInput = true;
				break; // 正解
			}
		}
	}

	// 入力結果のui変化
	if (isCorrectInput)
	{
		// 正解の処理
		pCurrentUI->SetAlpha(HALF);
		m_nNowIdx++;

		// サウンド再生 ( 成功音 )
		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_KEY_CLEAR_SE);
		return true;
	}
	else if (hasAnyInput)
	{
		// 不正解の処理
		pCurrentUI->ChangeCol(COLOR_RED, true);
		m_bFalse = true;
		SetTime(true);

		// サウンド再生 ( 失敗音 )
		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_TASKMISS_SE);
		return false;
	}

	return false;
}