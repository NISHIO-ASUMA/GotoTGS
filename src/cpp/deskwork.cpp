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
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CDeskwork::CDeskwork(int nPriority): CObject2D(nPriority),
m_pPCDeskUI(nullptr),
m_pCOPYDeskUI(nullptr)
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
	pDeskwork->SetSize(Config::WIDTH, Config::HEIGHT);	// サイズ設定
	pDeskwork->SetCol(COLOR_WHITE);						// カラー設定
	pDeskwork->SetTexture(Config::TEXNAME);				// テクスチャ設定

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

	if (m_pPCDeskUI != nullptr || m_pCOPYDeskUI != nullptr)
	{// どれかのポインタに中身が入っているなら
		// ポインタを初期化
		m_pPCDeskUI = nullptr;
		m_pCOPYDeskUI = nullptr;
	}

	// PCタスクUIの生成
	m_pPCDeskUI = CPCDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));

	// コピー機タスクUIの生成
	m_pCOPYDeskUI = CCOPYDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));

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
}

//=========================================================
// 更新処理
//=========================================================
void CDeskwork::Update(void)
{
	// ポインタがヌルかつ有効状態ではないなら
	if (m_pPCDeskUI != nullptr && m_pCOPYDeskUI != nullptr &&
		m_pPCDeskUI->GetUse() != true && m_pCOPYDeskUI->GetUse() != true)
	{
		return;
	}

	// 親の更新処理
	CObject2D::Update();

	if (m_pCOPYDeskUI->GetUse() != true)
	{// 他のタスクが起動していないなら
		// PCタスクUIの更新処理
		m_pPCDeskUI->Update();
	}

	if (m_pPCDeskUI->GetUse() != true)
	{// 他のタスクが起動していないなら
		// コピー機タスクUIの更新処理
		m_pCOPYDeskUI->Update();
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
// 背景の色の処理
//=========================================================
void CDeskwork::SetAlpha(const bool &bUse)
{
	if (bUse != false)
	{
		// 不透明にする
		SetCol(COLOR_WHITE);

		return;
	}

	// 透明にする
	SetCol(COLOR_NULL);
}