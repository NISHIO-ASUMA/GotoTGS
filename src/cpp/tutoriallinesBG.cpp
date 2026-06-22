//=========================================================
//
// チュートリアルのセリフ背景処理 [ tutoriallinesBG.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutoriallinesBG.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutorialLinesBG::CTutorialLinesBG(int nPriority) :CObject2D(nPriority),
m_bUse(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialLinesBG::~CTutorialLinesBG()
{

}

//=========================================================
// 生成処理
//=========================================================
CTutorialLinesBG* CTutorialLinesBG::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const char* TEXNAME)
{
	// 生成
	CTutorialLinesBG* pTutorialLinesBG = new CTutorialLinesBG;

	// ヌルチェック
	if (pTutorialLinesBG == nullptr) return nullptr;

	// 各種値の設定
	pTutorialLinesBG->SetPos(pos);				// 位置
	pTutorialLinesBG->SetSize(size.x, size.y);	// サイズ
	pTutorialLinesBG->SetCol(COLOR_WHITE);		// カラー
	pTutorialLinesBG->SetTexture(TEXNAME);		// テクスチャ

	// 初期化が失敗した時
	if (FAILED(pTutorialLinesBG->Init())) return nullptr;

	return pTutorialLinesBG;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialLinesBG::Init(void)
{
	// 親の初期化処理
	CObject2D::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CTutorialLinesBG::Uninit(void)
{
	// 親の終了処理
	CObject2D::Uninit();

}

//=========================================================
// 更新処理
//=========================================================
void CTutorialLinesBG::Update(void)
{
	// 描画していない状態なら
	if (m_bUse != true) return;

	// 親の更新処理
	CObject2D::Update();

}

//=========================================================
// 描画処理
//=========================================================
void CTutorialLinesBG::Draw(void)
{
	// 描画していない状態なら
	if (m_bUse != true) return;

	// 親の描画処理
	CObject2D::Draw();

}