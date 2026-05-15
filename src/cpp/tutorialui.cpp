//=========================================================
//
// チュートリアルUI処理 [ tutorialui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutorialUI::CTutorialUI(int nPriority) : CBillboard(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialUI::~CTutorialUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CTutorialUI* CTutorialUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fWidth, float fHeight, const char* pTexName)
{
	// インスタンス生成
	CTutorialUI* pTutorialUI = new CTutorialUI;
	if (pTutorialUI == nullptr) return nullptr;

	// オブジェクトセット
	pTutorialUI->SetPos(pos);
	pTutorialUI->SetSize(fWidth, fHeight);
	pTutorialUI->SetRot(rot);
	pTutorialUI->SetTexture(pTexName);

	// 初期化失敗時
	if (FAILED(pTutorialUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pTutorialUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialUI::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialUI::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CTutorialUI::Update(void)
{
	// 親クラスの更新処理
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutorialUI::Draw(void)
{
	// 親クラスの描画処理
	CBillboard::Draw();
}