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

//=========================================================
// コンストラクタ
//=========================================================
CDeskworkUIManager::CDeskworkUIManager() :
m_pos(VECTOR3_NULL),
m_bUse(false)
{
	m_nPCTaskNum = NULL;
	m_nCOPYTaskNum = NULL;
	m_nDOCUMENTTaskNum = NULL;

	if (m_pClearUI != nullptr)
	{
		return;
	}
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
// 書類の数の加算処理
//=========================================================
void CDeskworkUIManager::AddDOCUMENTTask(void)
{
	// こなしたコピー機タスクの数を加算
	m_nDOCUMENTTaskNum += m_nCOPYTaskNum;

	//  コピー機タスクの数を初期化
	m_nCOPYTaskNum = NULL;
}
