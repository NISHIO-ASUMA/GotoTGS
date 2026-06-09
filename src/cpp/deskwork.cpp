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
	pDeskwork->SetPos(pos);
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
		// ポインタを初期化
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

	// フラグを初期化
	m_pPCDeskUI->SetUse(false);
	m_pCOPYDeskUI->SetUse(false);

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
}

//=========================================================
// 更新処理
//=========================================================
void CDeskwork::Update(void)
{
	// ポインタがヌルではなく有効状態ではないなら
	if ((m_pPCDeskUI != nullptr && 
		m_pCOPYDeskUI != nullptr) &&
		m_pDOCUMENTDesk != nullptr &&
		(m_pPCDeskUI->GetUse() != true && 
		m_pCOPYDeskUI->GetUse() != true))
	{
		return;
	}

	// 親の更新処理
	CObject2D::Update();

	if (m_pCOPYDeskUI->GetUse() != true)
	{// 他のタスクが起動していないなら

		// PCタスクUIの更新処理
		m_pPCDeskUI->Update();

		return;
	}

	if (m_pPCDeskUI->GetUse() != true)
	{// 他のタスクが起動していないなら

		// コピー機タスクUIの更新処理
		m_pCOPYDeskUI->Update();
	
		if (m_pPCDeskUI->GetCOPYTaskNum() > NULL)
		{// コピー機タスクが残っている場合
			return;
		}

		// タスクが出来ない状態にする
		SetTexBG(CWorldUICollision::TYPE_COPY);
		m_pPCDeskUI->SetAlphaUI();
	}
	
}

//=========================================================
// 描画処理
//=========================================================
void CDeskwork::Draw(void)
{
	// 有効状態ではないなら
	if (m_pPCDeskUI->GetUse() != true && m_pCOPYDeskUI->GetUse() != true)
	{
		return;
	}

	// 親の描画処理
	CObject2D::Draw();
}

//=========================================================
// 背景のテクスチャ処理
//=========================================================
void CDeskwork::SetTexBG(const CWorldUICollision::TYPE& TaskType)
{
	switch (TaskType)
	{
	case CWorldUICollision::TYPE_PC:

		// テクスチャをPC用に設定
		SetSize(Config::PC_WIDTH, Config::PC_HEIGHT);
		SetTexture(Config::PC_TEXNAME);

		break;

	case CWorldUICollision::TYPE_COPY:

		// テクスチャをコピー機用に設定
		SetSize(Config::COPY_WIDTH, Config::COPY_HEIGHT);

		if (m_pCOPYDeskUI->GetPCTaskNum() <= NULL)
		{// PCのタスクをこなしていなかったら
			// 使用できないとき用のテクスチャに設定
			SetTexture(Config::CANCEL_TEXNAME);

			return;
		}

		// コピー機用のテクスチャに設定
		SetTexture(Config::COPY_TEXNAME);

		break;
	}
}