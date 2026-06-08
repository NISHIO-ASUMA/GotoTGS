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
#include "easing.h"

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
m_nCountTime(NULL),
m_bTime(false),
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
	ui.nKeytype = CDeskworkUI::KEYTYPE_BOARD;

	for (ui.nIdx = 0; ui.nIdx < Config::UI_NUM; ui.nIdx++)
	{
		// タスクをランダムに設定
		ui.nKey = (CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX);

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

	if (m_bTime != false || m_bFalse != false)
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
			m_pDeskUI[nCount]->SetAlpha(NULL);
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
	if (m_nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		m_nCountTime++;

		if (m_bFalse != false)
		{// 失敗している場合
			// 1フレーム毎の割合
			float t = CEasing::SetEase(m_nCountTime, Config::TIME_COOL);
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

		// タスクをランダムに設定
		m_pDeskUI[nCount]->SetKey((CDeskworkUI::KEYBOARD)(rand() % CDeskworkUI::KEYBOARD_MAX));

		// 色を元に戻す(通常色)
		m_pDeskUI[nCount]->ChangeCol(COLOR_WHITE);
	}

	// 現在選択している番号を初期化
	m_nNowIdx = 0;

	// クールタイムを初期化
	m_nCountTime = 0;

	// クールタイムが始まっていない状態にする
	m_bTime = false;

	// 失敗していない状態にする
	m_bFalse = false;

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

	if ((pKeyboard->GetTrigger(DIK_W) == true && m_pDeskUI[m_nNowIdx]->GetKey() == CDeskworkUI::KEYBOARD_W) ||
		(pKeyboard->GetTrigger(DIK_A) == true && m_pDeskUI[m_nNowIdx]->GetKey() == CDeskworkUI::KEYBOARD_A) ||
		(pKeyboard->GetTrigger(DIK_S) == true && m_pDeskUI[m_nNowIdx]->GetKey() == CDeskworkUI::KEYBOARD_S) ||
		(pKeyboard->GetTrigger(DIK_D) == true && m_pDeskUI[m_nNowIdx]->GetKey() == CDeskworkUI::KEYBOARD_D))
	{// 正解を押した時

		// 色を半透明にする
		m_pDeskUI[m_nNowIdx]->SetAlpha(0.5f);

		// 次のタスクに移る
		m_nNowIdx++;
	}
	else if ((pKeyboard->GetTrigger(DIK_W) == true && m_pDeskUI[m_nNowIdx]->GetKey() != CDeskworkUI::KEYBOARD_W) ||
			(pKeyboard->GetTrigger(DIK_A) == true && m_pDeskUI[m_nNowIdx]->GetKey() != CDeskworkUI::KEYBOARD_A) ||
			(pKeyboard->GetTrigger(DIK_S) == true && m_pDeskUI[m_nNowIdx]->GetKey() != CDeskworkUI::KEYBOARD_S) ||
			(pKeyboard->GetTrigger(DIK_D) == true && m_pDeskUI[m_nNowIdx]->GetKey() != CDeskworkUI::KEYBOARD_D))
	{// 不正解を押した時

		// 色を赤にする
		m_pDeskUI[m_nNowIdx]->ChangeCol(COLOR_RED, true);

		// クールタイムを始める
		m_bTime = true;

		// 失敗した状態にする
		m_bFalse = true;

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