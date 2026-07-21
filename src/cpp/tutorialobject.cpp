//=========================================================
//
// チュートリアルのオブジェクト管理 [ tutorialobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "jsonmanager.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "tutorialplayer.h"
#include "camera.h"
#include "tutoriallines.h"			// 髙橋追加

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CTutorialObject* CTutorialObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace TUTORIALOBJECT
{
	constexpr const char* LoadName	= "data/JSON/Tutorialobject.json"; // 読み込みjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CTutorialObject::CTutorialObject() : m_pBlockManager(nullptr),
m_pTutoPlayer(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTutorialObject::~CTutorialObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialObject::Init(void)
{
	//  jsonマネージャー取得
	CJsonManager* pManager = CManager::GetInstance()->GetJsonManager();
	pManager->Load(TUTORIALOBJECT::LoadName);

	// ステージマップ読み込み
	m_pBlockManager = std::make_unique<CBlockManager>();
	pManager->SetBlockManager(m_pBlockManager.get());
	m_pBlockManager->Init();

	// 操作キャラクター生成
	m_pTutoPlayer = CTutorialPlayer::Create(D3DXVECTOR3(-160.0f, 0.0f, 95.0f), VECTOR3_NULL);

	//// カメラに追従するキャラクターのポインタをセット
	//CManager::GetInstance()->GetCamera()->SetAnyCharactorPointer(m_pTutoPlayer);
	//CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pTutoPlayer->GetPos());

	// チュートリアルのセリフの生成 Misaki
	m_plines = CTutorialLines::Create(true);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialObject::Uninit(void)
{
	// ブロックマネージャーポインタの破棄
	m_pBlockManager.reset();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CTutorialObject::Update(void)
{
	//// カメラの追従ターゲット設定
	//CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pTutoPlayer->GetPos());

	// 管理クラス更新
	if (m_pBlockManager)
		m_pBlockManager->Update();

	// セリフがまだ出ているなら繰り返す
	if (m_plines->GetUse())
		return;

	//// フェード取得
	//CFade* pFade = CManager::GetInstance()->GetFade();

	//if (pFade != nullptr)
	//{
	//	// 画面切り替え
	//	pFade->SetFade(std::make_unique<CGame>());
	//	return;
	//}

}
//=========================================================
// インスタンス取得
//=========================================================
CTutorialObject* CTutorialObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CTutorialObject;
	}

	// インスタンスを返す
	return m_pInstance;
}