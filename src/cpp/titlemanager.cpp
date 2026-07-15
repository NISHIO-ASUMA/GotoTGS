//=========================================================
//
// タイトル管理処理 [ titlemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "titlemanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//=========================================================
// コンストラクタ
//=========================================================
CTitleManager::CTitleManager() : isKeyinputSet(false)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTitleManager::~CTitleManager()
{

}
//==========================================================
//インスタンス取得
//==========================================================
CTitleManager* CTitleManager::GetInstance(void)
{
	// インスタンスを返す
	static CTitleManager pTitleManager;
	return &pTitleManager;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	//pSound->Play(CSound::SOUND_LABEL_TITLE_BGM);

	// フラグ初期化
	isKeyinputSet = false;

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CTitleManager::Update(void)
{
	// フラグが有効なら下の処理をスキップ
	if (isKeyinputSet) return;

	// 入力デバイス取得
	auto * pKey = CManager::GetInstance()->GetInputKeyboard();
	auto * pJoyPad = CManager::GetInstance()->GetJoyPad();
	auto* pMouse = CManager::GetInstance()->GetMouse();

	// 取得失敗時
	if (pKey == nullptr) return;
	if (pJoyPad == nullptr) return;
	if (pMouse == nullptr) return;

	// キー入力時の判定
	if ((pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_START) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)) || 
		pMouse->GetTriggerDown(CInputMouse::MOUSE_LEFT))
	{
		// サウンド取得
		CSound* pSound = CManager::GetInstance()->GetSound();
		if (pSound == nullptr) return;

		// 入力判定を有効化
		isKeyinputSet = true;

		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_TITLEENTER);
		return;
	}
}