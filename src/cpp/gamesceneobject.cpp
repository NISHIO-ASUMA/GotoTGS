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
#include "vigilanceUImanager.h"		// 髙橋追加

#include "player.h"					// 近田追加
#include "friend.h"					// 近田追加
#include "pcui.h"					// 近田追加
#include "copyui.h"					// 近田追加
#include "smokeui.h"				// 近田追加
#include "afk2dui.h"				// 近田追加
#include "tutorialuimanager.h"		// 近田追加
#include "afkuimanager.h"			// 近田追加
#include "afkmanager.h"				// 近田追加
#include "doorui.h"					// 近田追加
#include "afktvpolygon.h"			// 近田追加

#include "enemy.h"					// 西尾追加
#include "worldUIcollision.h"		// 西尾追加
#include "camera.h"					// 西尾追加
#include "automaticdoormanager.h"	// 西尾追加
#include "autodoor_collision.h"		// 西尾追加
#include "slideopendoormanager.h"	// 西尾追加
#include "sideopendoorcollision.h"	// 西尾追加
#include "enemymanager.h"			// 西尾追加
#include "mobcharactormanager.h"	// 西尾追加
#include "boss.h"

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
m_pScoreTask(nullptr),
m_pScoreDitch(nullptr),
m_pScoreAll(nullptr),
m_pDeskwork(nullptr),
m_pEventUI(nullptr),
m_pVigilanceUImanager(nullptr)
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
	static CGameSceneObject Instance;
	return &Instance;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameSceneObject::Init(void)
{
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

	// ドア用チュートリアルUIの生成
	CDoorUI::Create();

	// テレビ用ポリゴンの生成
	CAfkTVPolygon::Create();

	// 各種ポインタクラスの生成
	CreatePointer();

	// 敵管理クラス生成
	CEnemyManager::GetInstance()->Init();
	
	// 上司のデスクのかご
	CBlock::Create(D3DXVECTOR3(40.0f,36.0f,280.0f),VECTOR3_NULL,INITSCALE,"STAGEOBJ/basket.x");

	// ゲーセンの上のビル
	CBlock::Create(D3DXVECTOR3(1656.0f, 322.0f, 122.0f), VECTOR3_NULL, D3DXVECTOR3(0.85f, 0.25f, 1.85f), "STAGEOBJ/bill01.x");

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(GAMEOBJECT::PlayerPos, VECTOR3_NULL);

	// カメラに追従するキャラクターのポインタをセット
	CManager::GetInstance()->GetCamera()->SetAnyCharactorPointer(m_pPlayer);

	// カメラのターゲット設定
	CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pPlayer->GetPos());

	// スコア初期化
	m_pScoreTask->DeleteScore();
	m_pScoreDitch->DeleteScore();
	m_pScoreAll->DeleteScore();

	// サイドに開くドアの管理クラスを生成
	CSideOpenDoorManager::GetInstance()->Init();

	// サイドに開くドアのコライダークラスを生成
	CSideOpenDoorCollision::GetInstance()->Init();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Init();

	// 自動ドアコライダー管理クラスの生成
	CAutoMaticDoorCollision::GetInstance()->Init();

	// モブキャラクター管理クラスを追加
	CMobCharactorManager::GetInstance()->Init();

	// テスト生成
	CBoss::Create(D3DXVECTOR3(700.0f, 0.0f, 350.0f), VECTOR3_NULL);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
	// 破棄される前に書き出し実行
	if (m_pScoreDitch)
		m_pScoreDitch->SaveScore("data/SCORE/LazyScore.bin");

	if (m_pScoreTask)
	{
		m_pScoreTask->ScoreChangeMath(-1); // 値の反転をする
		m_pScoreTask->SaveScore("data/SCORE/TaskScore.bin");
	}

	// タスクの判定を取る球形コライダー管理クラスを破棄
	CWorldUICollision::GetInstance()->Uninit();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Uninit();

	// 自動ドアコライダー管理クラスの終了
	CAutoMaticDoorCollision::GetInstance()->Uninit();

	// サイドに開くドアの管理クラスの終了
	CSideOpenDoorManager::GetInstance()->Uninit();

	// サイドに開くドアのコライダークラスの終了処理
	CSideOpenDoorCollision::GetInstance()->Uninit();

	// チュートリアルUIマネージャーの終了処理
	CTutorialUIManager::Instance()->Uninit();

	// AfkUIマネージャーの初期化処理
	CAfkUIManager::Instance()->Uninit();

	// さぼりをまとめたマネージャーの終了処理
	CAfkManager::Instance()->Uninit();

	// 敵管理クラスの終了
	CEnemyManager::GetInstance()->Uninit();

	// モブキャラクター管理クラスの終了
	CMobCharactorManager::GetInstance()->Uninit();

	// ブロック管理クラスの破棄
	m_pBlocks.reset();
}

//=========================================================
// 更新処理
//=========================================================
void CGameSceneObject::Update(void)
{
	// カメラの追従ターゲット設定
	CManager::GetInstance()->GetCamera()->SetTargetPersonPos(m_pPlayer->GetPos());

	// 西尾 タスクの判定を取る球形コライダー管理クラスを更新
	CWorldUICollision::GetInstance()->Update();

	// さぼりをまとめたマネージャーの更新処理
	CAfkManager::Instance()->Update();

	// サイドに開くドアの管理クラスの更新
	CSideOpenDoorManager::GetInstance()->Update();

	// サイドに開くドアのコライダークラスの更新処理
	CSideOpenDoorCollision::GetInstance()->Update();

	// 自動ドア管理クラス
	CAutoMaticDoorManager::GetInstance()->Update();

	// 自動ドアコライダー管理クラスの更新
	CAutoMaticDoorCollision::GetInstance()->Update();

	// 全体スコアの更新
	if (m_pScoreAll)
	{
		// スコアの値をもらい、allに加算していく
		int ScoreDitch = m_pScoreDitch->GetScore();
		int ScoreTask = m_pScoreTask->GetScore();

		// セットする
		m_pScoreAll->SetScore(ScoreDitch + ScoreTask);
	}

	// ブロック管理クラスの更新処理
	if (m_pBlocks) m_pBlocks->Update();

#ifdef _DEBUG
	// スコアの保存処理の検証
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_N))
	{
		// 加算
		m_pScoreDitch->AddScore(1000);
	}

	// スコアの保存処理の検証
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_B))
	{
		// 書き出し処理
		m_pScoreTask->SaveScore("data/SCORE/LazyScore.bin");
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

//********************************************
 
	// スコア生成
	m_pScoreTask = CScore::Create(VECTOR3_NULL, 200.0f, 80.0f,false);
	m_pScoreDitch = CScore::Create(VECTOR3_NULL, 200.0f, 80.0f,false);
	m_pScoreAll = CScore::Create(D3DXVECTOR3(1250.0f, 650.0f, 0.0f), 200.0f, 80.0f);

//********************************************

	// 進捗ゲージの生成 Misaki
	m_pProgressgauge = CProgressgauge::Create(D3DXVECTOR3(200.0f, 70.0f, 0.0f), 100.0f, 70.0f);

	// イベントUIの生成 Misaki
	m_pEventUI = CEventUI::Create();

	// 警戒度UIマネージャーの生成 Misaki
	m_pVigilanceUImanager = CVigilanceUIManager::Create(true);
}