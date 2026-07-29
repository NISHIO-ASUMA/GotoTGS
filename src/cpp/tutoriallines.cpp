//=========================================================
//
// チュートリアルのセリフ処理 [ tutoriallines.cpp ]
// Author: Takahashi Misaki
//	
// 場所を示す矢印を配置 ( Asuma )
//CBlock::Create(D3DXVECTOR3(-55.0f, 80.0f, 180.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f), D3DXVECTOR3(HALF, HALF, HALF), "STAGEOBJ/yajirusi.x");
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
#include "deskworkUImanager.h"
#include "deskwork.h"
#include "tutorialobject.h"
#include "tutorialplayer.h"
#include "pointobject.h"
#include "billboard.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutorialLines::CTutorialLines(int nPriority) :CObject(nPriority),
m_bUse(false),
m_bAgain(false),
m_nNowIdx(NULL),
m_nCountSkip(NULL),
m_pLines(nullptr),
m_pBG(nullptr),
m_pTutoPlayer(nullptr),
m_pKeyUi(nullptr),
m_pArrow{}
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
	// 配列初期化
	m_pArrow.clear();

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

	// 矢印生成
	CreateArrow();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CTutorialLines::Uninit(void)
{
	// 配列消去
	m_pArrow.clear();

	// 各ポインタをヌルにする
	m_pBG = nullptr;
	m_pLines = nullptr;
}

//=========================================================
// 更新処理
//=========================================================
void CTutorialLines::Update(void)
{
	// 使用してない場合とチュートリアルを進めている場合は更新しない
	if (!m_bUse || CDeskworkUIManager::GetTutorial()) return;

	// 実践をやる場合
	if (m_nNowIdx == LINESTYPE_3 ||
		m_nNowIdx == LINESTYPE_5 ||
		m_nNowIdx == LINESTYPE_6)
	{
		// 実践をやっている状態にする
		CDeskworkUIManager::SetTutorial(true);
		return;
	}

	// 入力取得
	auto* Key = CManager::GetInstance()->GetInputKeyboard();
	auto* Mouse = CManager::GetInstance()->GetMouse();
	auto* Pad = CManager::GetInstance()->GetJoyPad();

	// スキップ機能
	if (Key->GetPress(DIK_SPACE) || Pad->GetPress(CJoyPad::JOYKEY_Y))
	{
		// カウントを進める
		m_nCountSkip++;

		// 最大カウントを超えていなかったら
		if (m_nCountSkip <= Config::MAX_COUNT) return;

		// 現在の番号を初期化
		m_nNowIdx = NULL;

		// 使用していない状態にする
		m_bUse = false;

		// 各ポインタを状況に合わせて状態を設定する
		m_pBG->SetUse(m_bUse);
		m_pLines->SetUse(m_bUse);

		return;
	}
	else
	{
		// カウントを初期化
		m_nCountSkip = NULL;
	}

	// キー入力したかどうか
	if (!Key->GetTrigger(DIK_RETURN) &&
		!Mouse->GetTriggerDown(CInputMouse::MOUSE_LEFT) &&
		!Pad->GetTrigger(CJoyPad::JOYKEY_A) &&
		!Pad->GetTrigger(CJoyPad::JOYKEY_START))
		return;

	// 現在の番号を一つ進める
	m_nNowIdx++;

	if (m_nNowIdx < LINESTYPE_MAX)
	{// 総数を超えていない場合

		// セリフを番号に合わせる
		m_pLines->SetTexture(m_LinesType[m_nNowIdx]);

		return;
	}

	// 使用していない状態にする
	m_bUse = false;

	// 現在の番号を初期化
	m_nNowIdx = NULL;

	// 各ポインタを状況に合わせて状態を設定する
	m_pBG->SetUse(m_bUse);
	m_pLines->SetUse(m_bUse);
}

//=========================================================
// 描画処理
//=========================================================
void CTutorialLines::Draw(void)
{

}

//=========================================================
// 次のチュートリアルに進む設定処理
//=========================================================
void CTutorialLines::SetNextTutorial(void)
{
	// 現在の番号を一つ進める
	m_nNowIdx++;

	// 対象と一致するとき( パソコンタスクが終わった時 )
	if (m_nNowIdx == LINESTYPE_4)
	{
		// 対象オブジェクトの状態を変更する
		if (m_pTutoPlayer)
			m_pTutoPlayer->SetDefaultState();

		// 2個目の矢印を起動
		m_pArrow[0]->SetIsDraw(false);
		m_pArrow[1]->SetIsDraw(true);

		// 起動したらビルボードの位置も移動する
		m_pKeyUi->SetPos(D3DXVECTOR3(150.0f, 65.0f, 360.0f));
	}

	if (m_nNowIdx == LINESTYPE_6)
	{
		// 3個目の矢印を起動
		m_pArrow[1]->SetIsDraw(false);
		m_pArrow[2]->SetIsDraw(true);

		// 起動したらビルボードの位置も移動する
		m_pKeyUi->SetPos(D3DXVECTOR3(40.0f, 50.0f, 280.0f));
	}

	if (m_nNowIdx == LINESTYPE_7)
	{
		// 矢印の描画をなくす
		m_pArrow[2]->SetIsDraw(false);

		// uiの消去
		m_pKeyUi->Uninit();
	}

	// セリフを番号に合わせる
	m_pLines->SetTexture(m_LinesType[m_nNowIdx]);

	// タスクの情報を取得
	auto* pDesk = CTutorialObject::GetInstance()->GetDesk();

	// タスクをしていない状態にする
	pDesk->SetTaskType(pDesk->GetTaskType());

	// 実践をやっていない状態にする
	CDeskworkUIManager::SetTutorial(false);
}

//=========================================================
// 矢印生成
//=========================================================
void CTutorialLines::CreateArrow(void)
{
	// ビルボード生成
	m_pKeyUi = CBillboard::Create(D3DXVECTOR3(-55.0f, 60.0f, 180.0f), VECTOR3_NULL, 10.0f, 10.0f, "Fbutton.png");

	// 矢印オブジェクトを生成 (PC)
	m_pArrow.push_back(CPointObject::Create
						(D3DXVECTOR3(-55.0f, 120.0f, 180.0f),
						 D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f),
						 D3DXVECTOR3(HALF, HALF, HALF),
						"STAGEOBJ/yajirusi.x")
	);

	// 矢印オブジェクトを生成 (コピー)
	m_pArrow.push_back(CPointObject::Create
					   (D3DXVECTOR3(150.0f, 120.0f, 370.0f),
						D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f),
						D3DXVECTOR3(HALF, HALF, HALF),
						"STAGEOBJ/yajirusi.x")
	);


	// 矢印オブジェクトを生成 (提出)
	m_pArrow.push_back(CPointObject::Create
					   (D3DXVECTOR3(40.0f, 110.0f, 280.0f),
						D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f),
						D3DXVECTOR3(HALF, HALF, HALF),
						"STAGEOBJ/yajirusi.x")
	);

	// 描画起動( 最初のオブジェクトのみ )
	m_pArrow[0]->SetIsDraw(true);
	m_pArrow[1]->SetIsDraw(false);
	m_pArrow[2]->SetIsDraw(false);
}