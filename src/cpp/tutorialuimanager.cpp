//=========================================================
//
// TutorialUIManager処理 [ tutorialuimanager.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialuimanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "pcui.h"
#include "copyui.h"
#include "titleuimanager.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace TutorialUIManager
{
	const D3DXVECTOR3 PcUIPos = { -45.0f, 75.0f, 170.0f };		// PCUIの座標
	const D3DXVECTOR3 CopyUIPos = { 150.0f, 75.0f, 355.0f };	// コピー機UIの座標
	constexpr const char* UI_FBUTTON = "Fbutton.png";			// (キーボード)uiのテクスチャ名
	constexpr const char* UI_STARTBUTTON = "startbutton.png";	// (パッド)uiのテクスチャ名
};

//=================================================
// 静的メンバ変数
//=================================================
CTutorialUIManager* CTutorialUIManager::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CTutorialUIManager::CTutorialUIManager()
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialUIManager::~CTutorialUIManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialUIManager::Init(void)
{
	// 操作の種類を設定する(パッドかキーマウかどうか)
	m_nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();

	// キーボード操作
	if (m_nControlTypes == CONTROLTYPE_KEY)
	{
		// コピー機用チュートリアルUIの生成
		CCopyUI::Create(TutorialUIManager::CopyUIPos, VECTOR3_NULL, TutorialUIManager::UI_FBUTTON);
		// パソコン用チュートリアルUIの生成
		CPcUI::Create(TutorialUIManager::PcUIPos, VECTOR3_NULL, TutorialUIManager::UI_FBUTTON);
	}
	// ジョイパッド操作
	else if (m_nControlTypes == CONTROLTYPE_PAD)
	{
		// コピー機用チュートリアルUIの生成
		CCopyUI::Create(TutorialUIManager::CopyUIPos, VECTOR3_NULL, TutorialUIManager::UI_STARTBUTTON);
		// パソコン用チュートリアルUIの生成
		CPcUI::Create(TutorialUIManager::PcUIPos, VECTOR3_NULL, TutorialUIManager::UI_STARTBUTTON);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialUIManager::Uninit(void)
{
	// シングルトンの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CTutorialUIManager::Update(void)
{
	
}
//=========================================================
// インスタンス取得処理
//=========================================================
CTutorialUIManager* CTutorialUIManager::Instance(void)
{
	// nullチェック
	if (m_pInstance == nullptr)m_pInstance = new CTutorialUIManager;

	// 生成されたインスタンスを返す
	return m_pInstance;
}