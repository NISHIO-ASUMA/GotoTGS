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

//*********************************************************
// 定数名前空間
//*********************************************************
namespace TITLEUIINFO
{
	// テクスチャパス群
	constexpr const char* FILE_NAME[2] =
	{
		"KeyMenu.jpg",
		"GamePadMenu.jpg",
	};

	// 生成座標群
	const D3DXVECTOR3 CreatePos[2] =
	{
		{0.0f,0.0f,0.0f},
		{100.0f,0.0f,0.0f},
	};
};

//=========================================================
// コンストラクタ
//=========================================================
CTitleuiManager::CTitleuiManager() : m_nSelectIdx(NULL),
m_pUi{}
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
	// 初期UI生成
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_pUi[nCnt] = CTitleUi::Create
								(TITLEUIINFO::CreatePos[nCnt], 
								140.0f, 
								60.0f,
								TITLEUIINFO::FILE_NAME[nCnt], 
								false);
	};

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleuiManager::Uninit(void)
{

}
//=========================================================
// 更新処理 NOTE : ここでさっきinputに作った判別関数を使って選択数の幅の増減する ( パッドの選択肢の方だけ )
//=========================================================
void CTitleuiManager::Update(void)
{
	// キー入力を取得
	const auto& key = CManager::GetInstance()->GetInputKeyboard();
	const auto& pad = CManager::GetInstance()->GetJoyPad();

	// パッドの接続判定チェック
	bool isConnect = pad->GetConnectGamePad();
	if (!isConnect) m_pUi[PAD_NUMBER]->SetIsUse(false); // 透明にして選択させないようにする

	// 選択インデックス設定


}