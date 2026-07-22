//=========================================================
//
// タイトルui複数管理処理 [ titleuimanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "titleuimanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "titleui.h"
#include "template.h"
#include "scene.h"
#include "titlemanager.h"
#include "ui.h"
#include "easing.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "tutorial.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace TITLEUIINFO
{
	// テクスチャパス群
	constexpr const char* FILE_NAME[3] =
	{
		"Select.png",
		"keymouse.png",
		"gamepad.png",
	};

	// 生成座標群
	const D3DXVECTOR3 CreatePos[3] =
	{
		{1600.0f,240.0f,0.0f},
		{1600.0f,480.0f,0.0f},
		{1600.0f,620.0f,0.0f}
	};

	// 目標値のx座標
	constexpr float DEST_POS_X = 640.0f;

	// 横幅
	const float CreateWidth[3] =
	{
		300.0f,
		140.0f,
		140.0f
	};

	// 高さ
	const float CreatHeight[3] =
	{
		90.0f,
		50.0f,
		50.0f
	};

	// 拡大サイズ
	constexpr float UPPER_WIDTH = 170.0f;
	constexpr float UPPER_HEIGHT = 80.0f;
};

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SLIDESTARTUI
{
	// テクスチャパス群
	constexpr const char* FILE_NAME[2] =
	{
		"TitleLogo.png",
		"gamestart.png",
	};

	// 生成座標群
	const D3DXVECTOR3 CreatePos[2] =
	{
		{640.0f,180.0f,0.0f},
		{640.0f,620,0.0f}
	};

	// 生成横幅サイズ
	const float CreateSizeWidth[2] =
	{
		{300.0f},
		{170.0f}
	};

	// 生成高さサイズ
	const float CreateSizeHeight[2] =
	{
		{160.0f},
		{80.0f}
	};

};

//=========================================================
// コンストラクタ
//=========================================================
CTitleuiManager::CTitleuiManager() : m_nSelectIdx(1),
m_pUi{},
m_pStartUiList{},
m_isFinishSlideUi(false),
m_isSelectFinish(false),
m_isInputSet(false),
m_fSlideRatio(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CTitleuiManager::~CTitleuiManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleuiManager::Init(void)
{	
	// 初期ui生成
	for (int nCntStart = 0; nCntStart < START_UI_NUM; nCntStart++)
	{
		m_pStartUiList[nCntStart] = CUi::Create(SLIDESTARTUI::CreatePos[nCntStart],
												0,
												SLIDESTARTUI::CreateSizeWidth[nCntStart],
												SLIDESTARTUI::CreateSizeHeight[nCntStart],
												SLIDESTARTUI::FILE_NAME[nCntStart]
												);
	}

	// 選択肢UI生成
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_pUi[nCnt] = CTitleUi::Create
								(TITLEUIINFO::CreatePos[nCnt], 
								TITLEUIINFO::CreateWidth[nCnt],
								TITLEUIINFO::CreatHeight[nCnt],
								TITLEUIINFO::FILE_NAME[nCnt], 
								false);
	};

	// 初期インデックスを設定 ( キーマウ入力用選択 )
	m_nSelectIdx = SELECT_BEGIN;

	// 再起動用の変数を初期化する
	m_isFinishSlideUi = false;
	m_isSelectFinish = false;
	m_fSlideRatio = NULL;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleuiManager::Uninit(void)
{

}
//=========================================================
// 更新処理 
//=========================================================
void CTitleuiManager::Update(void)
{
	// 入力検知関数
	InputCheck();

	// 選択終わったら処理を通させない
	if (m_isSelectFinish) return;

	// スライド中なら
	if (!m_isFinishSlideUi)
	{
		SlideStartUi();
		return;
	}

	// キー入力を取得
	const auto& key = CManager::GetInstance()->GetInputKeyboard();
	const auto& pad = CManager::GetInstance()->GetJoyPad();
	const auto& mouse = CManager::GetInstance()->GetMouse();

	// パッドの接続判定チェック
	bool isConnect = pad->GetConnectGamePad();
	if (!isConnect) m_pUi[PAD_NUMBER]->SetIsUse(false);

	// パッドが接続されていない場合は、パッド用UIを非表示
	if (!isConnect)
	{
		m_pUi[PAD_NUMBER]->SetIsUse(false);
	}
	else
	{
		m_pUi[PAD_NUMBER]->SetIsUse(true);
	}

	int maxSelectIdx = isConnect ? PAD_NUMBER : SELECT_BEGIN;
	int inputDir = 0;

	if (key->GetTrigger(DIK_DOWN) || key->GetTrigger(DIK_S))
	{// 下入力
		inputDir = 1;
	}
	if (key->GetTrigger(DIK_UP) || key->GetTrigger(DIK_W))
	{// 上入力
		inputDir = -1;
	}

	// パッド入力の検知
	if (isConnect)
	{
		if (pad->GetTrigger(CJoyPad::JOYKEY_DOWN))
		{
			inputDir = 1;
		}
		if (pad->GetTrigger(CJoyPad::JOYKEY_UP))
		{
			inputDir = -1;
		}
	}

	if (inputDir != 0)
	{
		// インデックス加算
		m_nSelectIdx += inputDir;

		// 範囲内でループ
		m_nSelectIdx = Wrap(m_nSelectIdx, SELECT_BEGIN, maxSelectIdx);
	}

	// 選択されている物の強調
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		// 使ってないものはスキップ
		if (!m_pUi[nCnt]) continue;

		if (nCnt == 0)
		{
			m_pUi[nCnt]->SetCol(COLOR_WHITE);		 // 色変更
			continue;
		}

		if (nCnt == m_nSelectIdx)
		{
			// 選択中のUIの見た目を変える
			m_pUi[nCnt]->SetSize(TITLEUIINFO::UPPER_WIDTH, TITLEUIINFO::UPPER_HEIGHT); // 少し大きくする
			m_pUi[nCnt]->SetCol(COLOR_WHITE);		 // 色変更
		}
		else
		{
			// 選択されていないときはそのまま
			m_pUi[nCnt]->SetSize(140.0f, 50.0f); // 元のサイズ
			m_pUi[nCnt]->SetCol(COLOR_GLAY);	 // 色変更
		}
	}

	// 選択したら,キー入力でシーン遷移
	if (key->GetTrigger(DIK_RETURN) || pad->GetTrigger(CJoyPad::JOYKEY_A) || pad->GetTrigger(CJoyPad::JOYKEY_START)
		|| mouse->GetTriggerDown(CInputMouse::MOUSE_LEFT))
	{
		// 判定フラグを起動
		m_isSelectFinish = true;

		// サウンド取得
		CSound* pSound = CManager::GetInstance()->GetSound();
		if (pSound == nullptr) return;

		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_TITLEENTER);
		
		if (m_isFinishSlideUi && m_isSelectFinish)
		{
			// チュートリアルシーンへ遷移
			CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CTutorial>());
			return;
		}
	}
}
//=========================================================
// スライドする処理
//=========================================================
void CTitleuiManager::SlideStartUi(void)
{
	// フラグが未使用なら
	if (!m_isInputSet) return;

	// スライドの速度
	constexpr float SLIDE_SPEED = 0.05f;
	m_fSlideRatio += SLIDE_SPEED;

	if (m_fSlideRatio > 1.0f) m_fSlideRatio = 1.0f;

	// 初期UIを移動
	constexpr float TARGET_OUT_X = -400.0f;

	for (int nCntStart = 0; nCntStart < START_UI_NUM; nCntStart++)
	{
		if (!m_pStartUiList[nCntStart]) continue;

		D3DXVECTOR3 currentPos = m_pStartUiList[nCntStart]->GetPos();

		// Lerp関数
		currentPos.x = Lerp(SLIDESTARTUI::CreatePos[nCntStart].x, TARGET_OUT_X, m_fSlideRatio);
		m_pStartUiList[nCntStart]->SetPos(currentPos);
	}

	// 選択肢UIを右側から中央へ
	float easeRatio = CEasing::EaseOutBack(m_fSlideRatio);

	// 全選択肢の数
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (!m_pUi[nCnt]) continue;

		// 現在座標
		D3DXVECTOR3 currentPos = m_pUi[nCnt]->GetPos();

		// 初期位置から目標位置へイージングをかけて
		currentPos.x = Lerp(TITLEUIINFO::CreatePos[nCnt].x, TITLEUIINFO::DEST_POS_X, easeRatio);
		m_pUi[nCnt]->SetPos(currentPos);
	}

	// 目標のところに到達したら終了処理
	if (m_fSlideRatio >= 0.5f)
	{
		// スライド終了フラグを有効化
		m_isFinishSlideUi = true;

		//  uiの破棄
		for (int nCntStart = 0; nCntStart < START_UI_NUM; nCntStart++)
		{
			m_pStartUiList[nCntStart]->Uninit();
		}
		return;
	}
}
//=========================================================
// スライドする処理
//=========================================================
void CTitleuiManager::InputCheck(void)
{
	// 入力デバイス取得
	auto* pKey = CManager::GetInstance()->GetInputKeyboard();
	auto* pJoyPad = CManager::GetInstance()->GetJoyPad();
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
		m_isInputSet = true;

		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_TITLEENTER);
		return;
	}
}