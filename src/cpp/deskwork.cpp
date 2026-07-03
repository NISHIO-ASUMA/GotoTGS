//=========================================================
//
// タスク処理 [ deskwork.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "deskwork.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "PCdeskwork.h"
#include "COPYdeskwork.h"
#include "DOCUMENTdeskwork.h"
#include "input.h"
#include "titleuimanager.h"

//=========================================================
// 静的メンバ変数宣言
//=========================================================
CWorldUICollision::TYPE CDeskwork::m_TaskType = CWorldUICollision::TYPE_NONE;

//=========================================================
// コンストラクタ
//=========================================================
CDeskwork::CDeskwork(int nPriority): CObject2D(nPriority),
m_pPCDeskUI(nullptr),
m_pCOPYDeskUI(nullptr),
m_pDOCUMENTDesk(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CDeskwork::~CDeskwork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CDeskwork* CDeskwork::Create(const D3DXVECTOR3& pos)
{
	// ポインタ生成
	CDeskwork* pDeskwork = new CDeskwork;

	// ヌルチェック
	if (pDeskwork == nullptr) return nullptr;

	// 各種値の設定
	pDeskwork->SetPos(pos);										// 位置設定
	pDeskwork->SetSize(Config::PC_WIDTH, Config::PC_HEIGHT);	// サイズ設定
	pDeskwork->SetCol(COLOR_WHITE);								// カラー設定
	pDeskwork->SetTexture(Config::PC_TEXNAME);					// テクスチャ設定

	// 初期化が失敗した場合
	if (FAILED(pDeskwork->Init())) return nullptr;

	return pDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskwork::Init(void)
{
	// 親の初期化処理
	CObject2D::Init();

	if (m_pPCDeskUI != nullptr || m_pCOPYDeskUI != nullptr || m_pDOCUMENTDesk != nullptr)
	{// どれかのポインタに中身が入っているなら
		// 全てのポインタを初期化
		m_pPCDeskUI = nullptr;
		m_pCOPYDeskUI = nullptr;
		m_pDOCUMENTDesk = nullptr;
	}

	// PCタスクUIの生成
	m_pPCDeskUI = CPCDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT - Config::PC_VALUE_Y, 0.0f));

	// コピー機タスクUIの生成
	m_pCOPYDeskUI = CCOPYDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT + Config::COPY_VALUE_Y, 0.0f));

	// 書類タスクの生成
	m_pDOCUMENTDesk = CDOCUMENTDeskwork::Create();

	// タスクをしていない状態にする
	m_TaskType = CWorldUICollision::TYPE_NONE;

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskwork::Uninit(void)
{
	// 親の終了処理
	CObject2D::Uninit();

	// PCタスクUIを破棄
	if (m_pPCDeskUI)
	{
		m_pPCDeskUI->Uninit();
		m_pPCDeskUI = nullptr;
	}

	// コピー機タスクUIを破棄
	if (m_pCOPYDeskUI)
	{
		m_pCOPYDeskUI->Uninit();
		m_pCOPYDeskUI = nullptr;
	}

	// 書類タスクを破棄
	if (m_pDOCUMENTDesk)
	{
		m_pDOCUMENTDesk->Uninit();
		m_pDOCUMENTDesk = nullptr;
	}

	// タスクをしていない状態にする
	m_TaskType = CWorldUICollision::TYPE_NONE;
}

//=========================================================
// 更新処理
//=========================================================
void CDeskwork::Update(void)
{
	// ポインタがヌルなら
	if (m_pPCDeskUI == nullptr || 
		m_pCOPYDeskUI == nullptr ||
		m_pDOCUMENTDesk == nullptr)
	{
		return;
	}

	// 親の更新処理
	CObject2D::Update();

	switch (m_TaskType)
	{
	// タスクをしていない状態の場合 
	case CWorldUICollision::TYPE_NONE:

		// 何も更新しない
		break;

	// PCタスクの場合
	case CWorldUICollision::TYPE_PC:

		// PCタスクUIの更新処理
		m_pPCDeskUI->Update();

		break;

	// コピー機タスクの場合
	case CWorldUICollision::TYPE_COPY:

		// コピー機タスクUIの更新処理
		m_pCOPYDeskUI->Update();

		if (m_pCOPYDeskUI->GetPCTaskNum() > NULL || m_pCOPYDeskUI->GetTime() != false)
		{// PCタスクが残っている場合
			return;
		}

		// 使用できないとき用のテクスチャに設定
		SetTexture(Config::CANCEL_TEXNAME);

		// コピー機タスクUIの透明に設定
		m_pCOPYDeskUI->SetAlphaUI(NULL);

		break;

	// 書類タスクの場合
	case CWorldUICollision::TYPE_DOCUMENT:

		// 書類タスクの更新処理
		m_pDOCUMENTDesk->Update();

		break;
	}
}

//=========================================================
// 描画処理
//=========================================================
void CDeskwork::Draw(void)
{
	// PCタスクとコピー機タスク以外なら
	if (m_TaskType != CWorldUICollision::TYPE_PC && m_TaskType != CWorldUICollision::TYPE_COPY)
	{
		return;
	}

	// 親の描画処理
	CObject2D::Draw();
}

//=========================================================
// タスクの種類設定処理
//=========================================================
void CDeskwork::SetTaskType(const CWorldUICollision::TYPE& TaskType, const bool& bUse)
{
	switch (TaskType)
	{
	// PCタスクの場合
	case CWorldUICollision::TYPE_PC:
	
		// PCタスクUIの透明度を設定
		m_pPCDeskUI->SetAlphaUI(bUse);

		// テクスチャをPC用に設定
		SetSize(Config::PC_WIDTH, Config::PC_HEIGHT);
		SetTexture(Config::PC_TEXNAME);

		break;

	// コピー機タスクの場合
	case CWorldUICollision::TYPE_COPY:

		// コピー機タスクUIの透明度を設定
		m_pCOPYDeskUI->SetAlphaUI(bUse);

		// コピー機用のテクスチャに設定
		SetSize(Config::COPY_WIDTH, Config::COPY_HEIGHT);
		SetTexture(Config::COPY_TEXNAME);

		if (m_pCOPYDeskUI->GetPCTaskNum() <= NULL)
		{// PCタスクが残ってない場合
			// 使用できないとき用のテクスチャに設定
			SetTexture(Config::CANCEL_TEXNAME);

			// コピー機タスクUIの透明に設定
			m_pCOPYDeskUI->SetAlphaUI(NULL);

		}

		break;
	}

	if (bUse != true)
	{// 使用しないなら

		// タスクしていない状態に変更
		m_TaskType = CWorldUICollision::TYPE_NONE;

		return;
	}

	// タスクの種類を設定
	m_TaskType = TaskType;
}