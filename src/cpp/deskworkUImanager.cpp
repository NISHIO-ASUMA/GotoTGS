//=========================================================
//
// タスクUIマネージャー処理 [ deskworkUImanager.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "deskworkUImanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "ui.h"

// 静的メンバ変数宣言
int CDeskworkUIManager::m_nPCTaskNum = NULL;
int CDeskworkUIManager::m_nCOPYTaskNum = NULL;
int CDeskworkUIManager::m_nDOCUMENTTaskNum = NULL;
bool CDeskworkUIManager::m_bTutorial = NULL;

//=========================================================
// コンストラクタ
//=========================================================
CDeskworkUIManager::CDeskworkUIManager() :
m_pos(VECTOR3_NULL),
m_pClearUI(nullptr),
m_bUse(false),
m_bTime(false),
m_bCan(false),
m_nCountTime(NULL)
{
	m_nPCTaskNum = NULL;
	m_nCOPYTaskNum = NULL;
	m_nDOCUMENTTaskNum = NULL;
	m_bTutorial = false;
}

//=========================================================
// デストラクタ
//=========================================================
CDeskworkUIManager::~CDeskworkUIManager()
{

}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskworkUIManager::Init(void)
{
	// クリアUIの生成
	m_pClearUI = CUi::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT + 200.0f, 0.0f),
		60,
		190.0f,
		70.0f,
		"CLEAR000.png",
		false,
		false,
		5,
		false,
		false);

	// クールタイムではない状態にする
	m_bTime = false;

	// カウントを初期化
	m_nCountTime = NULL;

	// 使える状態にする
	m_bCan = true;

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskworkUIManager::Uninit(void)
{
	m_nPCTaskNum = NULL;
	m_nCOPYTaskNum = NULL;
	m_nDOCUMENTTaskNum = NULL;
	m_bTutorial = false;
}

//=========================================================
// 更新処理
//=========================================================
void CDeskworkUIManager::Update(void)
{	
	// 乱数の種
	srand((unsigned int)time(0));
}

//=========================================================
// 書類タスクの加算処理
//=========================================================
void CDeskworkUIManager::AddDOCUMENTTask(void)
{
	// こなしたコピー機タスクの分だけ加算
	m_nDOCUMENTTaskNum += m_nCOPYTaskNum;

	//  コピー機タスクの数を初期化
	m_nCOPYTaskNum = NULL;
}

//=========================================================
// 外出タスクの加算処理
//=========================================================
void CDeskworkUIManager::AddOutsideTask(void)
{
	// こなした書類タスクの分だけ加算
	m_nOutsideTaskNum += m_nDOCUMENTTaskNum;

	// 書類タスクの数を初期化
	m_nDOCUMENTTaskNum = NULL;
}
