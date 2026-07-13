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
CTutorialObject::CTutorialObject() : m_pBlockManager(nullptr)
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

	// ステージマップ読み込み
	m_pBlockManager = std::make_unique<CBlockManager>();
	pManager->SetBlockManager(m_pBlockManager.get());
	m_pBlockManager->Init();

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
}
//=========================================================
// インスタンス取得
//=========================================================
CTutorialObject* CTutorialObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CTutorialObject;

	// インスタンスを返す
	return m_pInstance;
}