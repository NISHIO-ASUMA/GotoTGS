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
#include "deskworkUImanager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CDeskwork::CDeskwork(int nPriority): CObject2D(nPriority),
m_pPCDeskUI(nullptr)
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

	if (m_pPCDeskUI == nullptr)
	{
		// PCタスクUIの生成 Misaki
		m_pPCDeskUI = CPCDeskwork::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));
	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskwork::Uninit(void)
{
	// 親の終了処理
	CObject2D::Uninit();

	// タスクUIマネージャーを破棄
	if (m_pPCDeskUI)
	{
		m_pPCDeskUI->Uninit();
		m_pPCDeskUI = nullptr;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CDeskwork::Update(void)
{
	// キーボードのポインタ
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F) == true)
	{// Fキーを押したら
		// 使っていいるかどうかで透明度を設定
		m_pPCDeskUI->SetAlphaUI();
		SetAlpha(m_pPCDeskUI->GetUse());
	}

	if (m_pPCDeskUI->GetUse() != true && m_pPCDeskUI->GetUse() != true)
	{
		return;
	}

	// 親の更新処理
	CObject2D::Update();

	if (m_pPCDeskUI != nullptr)
	{
		// タスクUIマネージャーの更新処理
		m_pPCDeskUI->Update();
	}
}

//=========================================================
// 描画処理
//=========================================================
void CDeskwork::Draw(void)
{
	if (m_pPCDeskUI->GetUse() != true)
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
