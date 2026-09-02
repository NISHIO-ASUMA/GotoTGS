//=========================================================
//
// 捕まった時のリザルトシーン処理 [ loselazyresult.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "loselazyresult.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "camera.h"
#include "meshfield.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "resultcastmanager.h"
#include "ui.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CLoseLazyResult::CLoseLazyResult() : CScene(CScene::MODE_LOSELAZY),
m_pBlock(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CLoseLazyResult::~CLoseLazyResult()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CLoseLazyResult::Init(void)
{
	// カメラの設定を変更する
	CManager::GetInstance()->GetCamera()->Init();

	// メッシュ生成
	CMeshField::Create(VECTOR3_NULL, 1000.0f, 1000.0f, 1, 1, "data/TEXTURE/field00.jpg");

	// ブロックマネージャー生成
	m_pBlock = std::make_unique<CBlockManager>();
	const auto& jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlock.get());

	// 初期化とポインタセット
	m_pBlock->SetLoadFileName();
	m_pBlock->Init();

	// キャラクター生成
	CResultCastManager::GetInstance()->Init();

	// ui生成
	CUi::Create({ 650.0f,140.0f,0.0f }, 0, 180.0f, 80.0f, "cacthword.png");

	// ui生成
	CUi::Create(CENTERPOS, 0, 640.0f, 360.0f, "effectscreenall.png");

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CLoseLazyResult::Uninit(void)
{
	// ポインタの破棄
	m_pBlock.reset();

	// キャラクター生成
	CResultCastManager::GetInstance()->Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CLoseLazyResult::Update(void)
{
	// 入力デバイスを取得
	CInputKeyboard* pInput = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJyoPad = CManager::GetInstance()->GetJoyPad();
	CInputMouse* pMouse = CManager::GetInstance()->GetMouse();

	// nullチェック
	if (pInput == nullptr) return;
	if (pJyoPad == nullptr) return;
	if (pMouse == nullptr) return;

	// 何らかの画面遷移キー
	if ((pInput->GetTrigger(DIK_RETURN) || pJyoPad->GetTrigger(pJyoPad->JOYKEY_A) || pJyoPad->GetTrigger(pJyoPad->JOYKEY_START))
		|| pMouse->GetTriggerDown(CInputMouse::MOUSE_LEFT))
	{
		// フェードクラスのポインタ取得
		CFade* pFade = CManager::GetInstance()->GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// ランキングシーン遷移
			pFade->SetFade(std::make_unique <CRanking>());
			return;
		}
	}
}
//=========================================================
// 描画処理
//=========================================================
void CLoseLazyResult::Draw(void)
{

}