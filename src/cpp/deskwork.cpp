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

//=========================================================
// コンストラクタ
//=========================================================
CDeskwork::CDeskwork(int nPriority): CObject2D(nPriority),
m_pDeskUIManager(nullptr)
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

	// 初期化が失敗した場合
	if (FAILED(pDeskwork->Init())) return nullptr;

	return pDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskwork::Init(void)
{
	// 背景の各種設定
	SetSize(Config::WIDTH, Config::HEIGHT);	// サイズ設定
	SetCol(COLOR_WHITE);					// カラー設定
	SetTexture(Config::TEXNAME);			// テクスチャ設定

	if (m_pDeskUIManager == nullptr)
	{
		// タスクUIマネージャーの生成 Misaki
		m_pDeskUIManager = CDeskworkUIManager::Create(D3DXVECTOR3(HALFWIDTH, HALFHEIGHT, 0.0f));
	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskwork::Uninit(void)
{
	// タスクUIマネージャーを破棄
	if (m_pDeskUIManager)
	{
		m_pDeskUIManager->Uninit();
		m_pDeskUIManager = nullptr;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CDeskwork::Update(void)
{
	if (m_pDeskUIManager != nullptr)
	{
		// タスクUIマネージャーの更新処理
		m_pDeskUIManager->Update();
	}
}

//=========================================================
// 描画処理
//=========================================================
void CDeskwork::Draw(void)
{

}