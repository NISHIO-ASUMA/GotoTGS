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

//=================================================
// 静的メンバ変数
//=================================================
CAfkManager* CAfkManager::m_pInstance = nullptr; // インスタンス変数

//=========================================================
// コンストラクタ
//=========================================================
CAfkManager::CAfkManager() : m_pAfkSmoke(nullptr),
m_pAfkTV(nullptr),
m_pAfkMagazine(nullptr),
m_pAfkGameCenter(nullptr)
{

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