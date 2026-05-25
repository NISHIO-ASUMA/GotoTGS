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
#include "player.h"
#include "gametime.h"	// Misaki
#include "deskwork.h"	// Misaki
#include <enemy.h>
#include "friend.h"
#include "pcui.h"
#include "copyui.h"
#include "worldUIcollision.h" // 西尾追加


//*********************************************************
// 静的メンバ変数
//*********************************************************
CGameSceneObject* CGameSceneObject::m_pInstance = nullptr;				// シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace GAMEOBJECT
{
	const D3DXVECTOR3 TimerPos		= { 1020.0f,60.0f,0.0f };		// タイマーの座標
	const D3DXVECTOR3 PcUIPos		= { -45.0f, 50.0f, 170.0f };	// PCUIの座標
	const D3DXVECTOR3 CopyUIPos		= { 170.0f, 50.0f, 355.0f };	// コピー機UIの座標
	const D3DXVECTOR3 PlayerPos	    = { -100.0f, 0.0f, 0.0f };		// プレイヤーの座標
	constexpr const char* LoadName	= "data/JSON/Gameobject.json";	// 読み込みjsonファイル名
	constexpr const char* WallName	= "data/JSON/GameWall.json";	// 読み込みjsonファイル名
};

//=========================================================
// コンストラクタ
//=========================================================
CGameSceneObject::CGameSceneObject() : m_pBlocks(nullptr),
m_pTimer(nullptr),
m_pScore(nullptr),
m_pDeskwork(nullptr),
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
	// ゲームで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(GAMEOBJECT::LoadName);

	// タスクの判定を取る球形コライダー管理クラスを生成
	CWorldUICollision::GetInstance()->Init();

	// パソコン用チュートリアルUIの生成
	CPcUI::Create(GAMEOBJECT::PcUIPos, VECTOR3_NULL, "Fbutton.png");
	
	// コピー機用チュートリアルUIの生成
	CCopyUI::Create(GAMEOBJECT::CopyUIPos, VECTOR3_NULL, "Fbutton.png");

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(GAMEOBJECT::PlayerPos, VECTOR3_NULL);

	// 各種ポインタクラスの生成
	CreatePointer();

	// 敵生成
	CEnemy::Create(D3DXVECTOR3(390.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// スコア初期化
	m_pScore->DeleteScore();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
	//*************************************
	// ADD : 西尾
	// タスクの判定を取る球形コライダー管理クラスを破棄
	CWorldUICollision::GetInstance()->Uninit();

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
	//****************************************
	// ADD : 西尾
	// タスクの判定を取る球形コライダー管理クラスを更新
	CWorldUICollision::GetInstance()->Update();
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
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlocks.get());
	m_pBlocks->Init();

	// タイマー生成 Misaki
	m_pTimer = CGametime::Create(GAMEOBJECT::TimerPos, 60.0f, 40.0f);

	// スコア生成
	m_pScore = CScore::Create(D3DXVECTOR3(1250.0f, 650.0f, 0.0f), 200.f, 80.0f);

	// タスクの生成 Misaki
	m_pDeskwork = CDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));
}