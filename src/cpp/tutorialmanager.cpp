//=========================================================
//
// チュートリアル管理処理 [ tutorialmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//********************************************************
// クラス定義ヘッダーファイル
//********************************************************
#include "tutorialmanager.h"

//********************************************************
// インクルードファイル
//********************************************************
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "ui.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "camera.h"

//=========================================================
// インスタンス取得
//=========================================================
CTutorialManager* CTutorialManager::GetInstance(void)
{
	// 生成されたインスタンス
	static CTutorialManager pTutoManager;
	return &pTutoManager;
}
//=========================================================
// コンストラクタ
//=========================================================
CTutorialManager::CTutorialManager()
{

}
//=========================================================
// デストラクタ
//=========================================================
CTutorialManager::~CTutorialManager()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	//pSound->Play(CSound::SOUND_LABEL_TUTORIALBGM);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CTutorialManager::Update(void)
{
	// 入力取得
	auto* Key = CManager::GetInstance()->GetInputKeyboard();
	auto* Mouse = CManager::GetInstance()->GetMouse();
	auto* Pad = CManager::GetInstance()->GetJoyPad();

	// キー入力で遷移 ( 仮 )
	if (Key->GetTrigger(DIK_RETURN) ||
		Mouse->GetTriggerDown(CInputMouse::MOUSE_LEFT) ||
		Pad->GetTrigger(CJoyPad::JOYKEY_A) ||
		Pad->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// フェード取得
		CFade* pFade = CManager::GetInstance()->GetFade();
		if (pFade != nullptr)
		{
			// 画面切り替え
			pFade->SetFade(std::make_unique<CGame>());
			return;
		}
	}
}