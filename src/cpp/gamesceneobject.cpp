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
	const D3DXVECTOR3 TopAntPos		= { 0.0f, 0.0f, -450.0f };		// 操作アリの座標
	const D3DXVECTOR3 QueenPos		= { 0.0f, 55.0f, 0.0f };		// 女王アリの座標
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
	// メッシュフィールド生成
	CMeshField::Create(VECTOR3_NULL, 4000, 4000, 1, 1);

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), VECTOR3_NULL);

	// 敵生成
	CEnemy::Create(D3DXVECTOR3(-800.0f,0.0f,250.0f),VECTOR3_NULL);

	// 同僚
	CFriend::Create(D3DXVECTOR3(40.0f, 15.0f, 162.5f), D3DXVECTOR3(0.0f,1.57f,0.0f));

	//// ゲームで使うオブジェクトの読み込み
	//auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	//jsonmanager->Load(GAMEOBJECT::LoadName);

	// 各種ポインタクラスの生成
	CreatePointer();

	//// スコア初期化
	//m_pScore->DeleteScore();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
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

	//// スコア生成
	//m_pScore = CScore::Create(VECTOR3_NULL);

	// タスクの生成 Misaki
	m_pDeskwork = CDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));

	//// 世界の壁管理クラスの生成
	//m_pWorldWallManager = std::make_unique<CWorldWallManager>();
	//jsonManager->SetWorldWallManager(m_pWorldWallManager.get());
	//m_pWorldWallManager->Init(GAMEOBJECT::WallName);

}