//=========================================================
//
// AfkManager処理 [ afkmanager.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "afksmoke.h"
#include "afktv.h"
#include "afkmagazine.h"
#include "afkgamecenter.h"
#include "afkeating.h"
#include "afkbench.h"
#include "gamesceneobject.h"
#include "player.h"

//=================================================
// 静的メンバ変数
//=================================================
CAfkManager* CAfkManager::m_pInstance = nullptr; // インスタンス変数

//*********************************************************
// 名前空間
//*********************************************************
namespace AFKBENCH
{
	const D3DXVECTOR3 StationPos = { 792.4f, 14.0f, 1303.6f };		// 駅のベンチの位置
	const D3DXVECTOR3 GameCenterPos = { 1461.1f, 14.0f,317.0f };	// ゲームセンターのベンチの位置
	const D3DXVECTOR3 IzakayaPos = { 1527.5f, 14.0f, -962.4f };		// 居酒屋のベンチの位置
	const D3DXVECTOR3 OfficePos = { 734.4f, 14.0f, -468.0f };		// オフィス横のベンチの位置
	constexpr int MAX_BENCH = 4;									// ベンチの最大数
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkManager::CAfkManager() : m_pAfkSmoke(nullptr),
m_pAfkTV(nullptr),
m_pAfkMagazine(nullptr),
m_pAfkGameCenter(nullptr),
m_pAfkEating(nullptr)
{
	for (int nCnt = 0; nCnt < AFKBENCH::MAX_BENCH; nCnt++)
	{
		m_pAfkBench[nCnt] = NULL;
	}
}

//=========================================================
// デストラクタ
//=========================================================
CAfkManager::~CAfkManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkManager::Init(void)
{
	// たばこさぼりの初期化処理
	m_pAfkSmoke = new CAfksmoke;
	m_pAfkSmoke->Init();

	// テレビさぼりの初期化処理
	m_pAfkTV = new CAfkTV;
	m_pAfkTV->Init();

	// 漫画さぼりの初期化処理
	m_pAfkMagazine = new CAfkMagazine;
	m_pAfkMagazine->Init();

	// ゲームセンターさぼりの初期化処理
	m_pAfkGameCenter = new CAfkGameCenter;
	m_pAfkGameCenter->Init();

	// 飲食さぼりの初期化処理
	m_pAfkEating = new CAfkEating;
	m_pAfkEating->Init();

	// ベンチさぼりの初期化処理
	for (int nCnt = 0; nCnt < AFKBENCH::MAX_BENCH; nCnt++)
	{
		m_pAfkBench[nCnt] = new CAfkBench;
	}

	m_pAfkBench[0]->Init(AFKBENCH::StationPos);
	m_pAfkBench[1]->Init(AFKBENCH::GameCenterPos);
	m_pAfkBench[2]->Init(AFKBENCH::IzakayaPos);
	m_pAfkBench[3]->Init(AFKBENCH::OfficePos);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkManager::Uninit(void)
{
	// たばこさぼりの破棄処理
	if (m_pAfkSmoke)
	{
		m_pAfkSmoke->Uninit();
		delete m_pAfkSmoke;
		m_pAfkSmoke = nullptr;
	}

	// テレビさぼりの破棄処理
	if (m_pAfkTV)
	{
		m_pAfkTV->Uninit();
		delete m_pAfkTV;
		m_pAfkTV = nullptr;
	}

	// 漫画さぼりの破棄処理
	if (m_pAfkMagazine)
	{
		m_pAfkMagazine->Uninit();
		delete m_pAfkMagazine;
		m_pAfkMagazine = nullptr;
	}

	// ゲームセンターさぼりの破棄処理
	if (m_pAfkGameCenter)
	{
		m_pAfkGameCenter->Uninit();
		delete m_pAfkGameCenter;
		m_pAfkGameCenter = nullptr;
	}

	// 飲食さぼりの破棄処理
	if (m_pAfkEating)
	{
		m_pAfkEating->Uninit();
		delete m_pAfkEating;
		m_pAfkEating = nullptr;
	}

	for (int nCnt = 0; nCnt < AFKBENCH::MAX_BENCH; nCnt++)
	{
		// ベンチさぼりの破棄処理
		if (m_pAfkBench[nCnt])
		{
			m_pAfkBench[nCnt]->Uninit();
			delete m_pAfkBench[nCnt];
			m_pAfkBench[nCnt] = nullptr;
		}
	}

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
void CAfkManager::Update(void)
{
	// たばこさぼりの更新処理
	m_pAfkSmoke->Update();

	// テレビさぼりの更新処理
	m_pAfkTV->Update();

	// 漫画さぼりの更新処理
	m_pAfkMagazine->Update();

	// ゲームセンターさぼりの更新処理
	m_pAfkGameCenter->Update();

	// 飲食さぼりの更新処理
	m_pAfkEating->Update();

	bool bAfkBench[4];

	for (int nCnt = 0; nCnt < AFKBENCH::MAX_BENCH; nCnt++)
	{
		// 飲食さぼりの更新処理
		m_pAfkBench[nCnt]->Update();

		// プレイヤーがさぼっているか判定用の変数
		bAfkBench[nCnt] = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkBench(nCnt);

		if (bAfkBench[nCnt]) m_pAfkBench[nCnt]->SetDisplay(true);
		else m_pAfkBench[nCnt]->SetDisplay(false);
	}
}
//=========================================================
// インスタンス取得処理
//=========================================================
CAfkManager* CAfkManager::Instance(void)
{
	// nullチェック
	if (m_pInstance == nullptr)m_pInstance = new CAfkManager;

	// 生成されたインスタンスを返す
	return m_pInstance;
}