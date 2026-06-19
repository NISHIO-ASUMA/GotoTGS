//=========================================================
//
// ゲームのオブジェクト管理処理 [ gamesceneobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gamesceneobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "blockmanager.h"
#include "score.h"
#include "gamemanager.h"
#include "jsonmanager.h"
#include "worldwallmanager.h"
#include "ui.h"

#include "meshfield.h"
#include "gametime.h"				// 髙橋追加
#include "deskwork.h"				// 髙橋追加
#include "progressgauge.h"			// 髙橋追加
#include "eventUI.h"				// 髙橋追加
#include "tutoriallines.h"			// 髙橋追加

#include "player.h"					// 近田追加
#include "friend.h"					// 近田追加
#include "pcui.h"					// 近田追加
#include "copyui.h"					// 近田追加
#include "smokeui.h"				// 近田追加
#include "afk2dui.h"				// 近田追加
#include "tutorialuimanager.h"		// 近田追加
#include "afkuimanager.h"			// 近田追加
#include "afkmanager.h"				// 近田追加

#include "enemy.h"					// 西尾追加
#include "worldUIcollision.h"		// 西尾追加
#include "camera.h"					// 西尾追加
#include "automaticdoormanager.h"	// 西尾追加
#include "autodoor_collision.h"		// 西尾追加

//*********************************************************
// 静的メンバ変数
//*********************************************************
CGameSceneObject* CGameSceneObject::m_pInstance = nullptr;		// シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace GAMEOBJECT
{
	const D3DXVECTOR3 TimerPos		= { 1020.0f,60.0f,0.0f };						// タイマーの座標
	const D3DXVECTOR3 PlayerPos	    = { -160.0f, 0.0f, 95.0f };						// プレイヤーの座標
	constexpr const char* LoadName	= "data/JSON/Gameobject.json";					// ゲーム内オブジェクトjsonファイル名
	constexpr const char* CharactorLoadName	= "data/JSON/GameCharactorData.json";	// キャラ読み込みjsonファイル名
};

//=========================================================
// コンストラクタ
//=========================================================
CGameSceneObject::CGameSceneObject() : m_pBlocks(nullptr),
m_pTimer(nullptr),
m_pScore(nullptr),
m_pDeskwork(nullptr),
m_pEventUI(nullptr),
m_pWorldWallManager(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CGameSceneObject::~CGameSceneObject()
{

}
//=========================================================
// インスタンス取得処理
//=========================================================
CGameSceneObject* CGameSceneObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CGameSceneObject;

	// インスタンスを返す
	return m_pInstance;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameSceneObject::Init(void)
{
#if 1
	// ゲームで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(GAMEOBJECT::LoadName);
	jsonmanager->Load(GAMEOBJECT::CharactorLoadName);

	// タスクの判定を取る球形コライダー管理クラスを生成
	CWorldUICollision::GetInstance()->Init();

	// チュートリアルUIマネージャーの初期化処理
	CTutorialUIManager::Instance()->Init();

	// AfkUIマネージャーの初期化処理
	CAfkUIManager::Instance()->Init();

	// さぼりをまとめたマネージャーの初期化処理
	CAfkManager::Instance()->Init();

	// さぼっているときのUIの生成
	CAfk2DUI::Create();

	// 各種ポインタクラスの生成
	CreatePointer();

	// 敵生成
	CEnemy::Create(D3DXVECTOR3(390.0f, 0.0f, 200.0f), VECTOR3_NULL);
	
	// 上司のデスクのかご
	CBlock::Create(D3DXVECTOR3(40.0f,36.0f,280.0f),VECTOR3_NULL,INITSCALE,"STAGEOBJ/basket.x");

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(GAMEOBJECT::PlayerPos, VECTOR3_NULL);

	// ブロックマネージャーに生成されたプレイヤーのポインタをセット
	m_pBlocks->SetPlayerPoint(m_pPlayer);

	// カメラのターゲット設定
	CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pPlayer->GetPos());

	// スコア初期化
	m_pScore->DeleteScore();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Init();

	// 自動ドアコライダー管理クラスの生成
	CAutoMaticDoorCollision::GetInstance()->Init();

#endif
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
	// タスクの判定を取る球形コライダー管理クラスを破棄
	CWorldUICollision::GetInstance()->Uninit();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Uninit();

	// 自動ドアコライダー管理クラスの終了
	CAutoMaticDoorCollision::GetInstance()->Uninit();

	// チュートリアルUIマネージャーの終了処理
	CTutorialUIManager::Instance()->Uninit();

	// AfkUIマネージャーの初期化処理
	CAfkUIManager::Instance()->Uninit();

	// さぼりをまとめたマネージャーの終了処理
	CAfkManager::Instance()->Uninit();

	// ブロック管理クラスの破棄
	m_pBlocks.reset();

	// 世界の壁の破棄
	m_pWorldWallManager.reset();

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
void CGameSceneObject::Update(void)
{
	// カメラの追従ターゲット設定
	CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pPlayer->GetPos());

	//  ADD : 西尾 タスクの判定を取る球形コライダー管理クラスを更新
	CWorldUICollision::GetInstance()->Update();

	// さぼりをまとめたマネージャーの更新処理
	CAfkManager::Instance()->Update();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Update();

	// 自動ドアコライダー管理クラスの更新
	CAutoMaticDoorCollision::GetInstance()->Update();

	// ブロック管理クラスの更新処理
	if (m_pBlocks) m_pBlocks->Update();

#ifdef _DEBUG
	// スコアの保存処理の検証
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2))
	{
		// 書き出し処理
		m_pScore->SaveScore("data/SCORE/LazyScore.bin");
	}
#endif // _DEBUG

}
//=========================================================
// 描画処理
//=========================================================
void CGameSceneObject::Draw(void)
{

}
//=========================================================
// ポインタの生成を行う関数
//=========================================================
void CGameSceneObject::CreatePointer(void)
{
	// ブロックマネージャー生成
	m_pBlocks = std::make_unique<CBlockManager>();
	const auto& jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlocks.get());

	// 初期化とポインタセット
	m_pBlocks->Init();

	// タスクの生成 Misaki
	m_pDeskwork = CDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT + 50.0f, 0.0f));

	// タイマー生成 Misaki
	m_pTimer = CGametime::Create(GAMEOBJECT::TimerPos, 60.0f, 40.0f);

	// スコア生成
	m_pScore = CScore::Create(D3DXVECTOR3(1250.0f, 650.0f, 0.0f), 200.f, 80.0f);

	// 進捗ゲージの生成 Misaki
	m_pProgressgauge = CProgressgauge::Create(D3DXVECTOR3(400.0f, 50.0f, 0.0f), 300.0f, 25.0f);

	// イベントUIの生成 Misaki
	m_pEventUI = CEventUI::Create();

	// チュートリアルのセリフの生成 Misaki
	m_pTutoriallines = CTutorialLines::Create(true);
}