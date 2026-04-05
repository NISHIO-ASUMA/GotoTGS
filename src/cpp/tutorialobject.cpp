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
#include "queen.h"
#include "tutoarrayant.h"
#include "tutorialtopant.h"
#include "feed.h"
#include "jsonmanager.h"
#include "tutorialuimanager.h"
#include "worldwallmanager.h"
#include "basemapfeed.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CTutorialObject* CTutorialObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace TUTORIALOBJECT
{
	const D3DXVECTOR3 TopAntPos		= { 0.0f, 0.0f, -450.0f };	// 操作アリ座標
	const D3DXVECTOR3 ArrayAntPos	= { 350.0f, 0.0f, 0.0f };	// 仲間アリ座標
	const D3DXVECTOR3 FeedPos		= { -500.0f, 0.0f, 0.0f };	// 餌座標
	constexpr const char* LoadName	= "data/JSON/Tutorialobject.json"; // 読み込みjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CTutorialObject::CTutorialObject() : m_pBlockManager(nullptr),
m_pTopAnt(nullptr),
m_pWorldWall(nullptr),
m_pArrayAnt(nullptr),
m_pFeed(nullptr)
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
	// チュートリアルで使うオブジェクトの読み込み
	auto JsonManager = CManager::GetInstance()->GetJsonManager();
	JsonManager->Load(TUTORIALOBJECT::LoadName);

	// ステージマップ読み込み
	m_pBlockManager = std::make_unique<CBlockManager>();
	JsonManager->SetBlockManager(m_pBlockManager.get());
	m_pBlockManager->Init();

	// チュートリアルuiマネージャー
	CTutorialUiManager::GetInstance()->Init();

	// 基準の餌を生成する
	CBaseMapFeed::GetInstance()->Init();

	// チュートリアルトップアリの生成
	m_pTopAnt = CTutoTopAnt::Create(TUTORIALOBJECT::TopAntPos);
	
	// 仲間の黒アリ生成
	m_pArrayAnt = CTutoArrayAnt::Create(TUTORIALOBJECT::ArrayAntPos);

	// 餌を生成
	m_pFeed = CFeed::Create(TUTORIALOBJECT::FeedPos, VECTOR3_NULL, INITSCALE);


	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialObject::Uninit(void)
{
	// ブロックマネージャーポインタの破棄
	m_pBlockManager.reset();

	// 世界の壁の破棄
	m_pWorldWall.reset();

	// uiマネージャーの終了
	CTutorialUiManager::GetInstance()->Uninit();

	// 基準の餌を破棄する
	//CBaseMapFeed::GetInstance()->Uninit();

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
	// uiマネージャーの更新
	CTutorialUiManager::GetInstance()->Update();
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