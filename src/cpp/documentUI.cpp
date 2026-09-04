//=========================================================
//
// 書類タスクUI処理 [ DocumentUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "Documentui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace DocumentUI
{
	constexpr float fWidth = 15.0f;		// UIの横幅の値
	constexpr float fHeight = 15.0f;	// UIの縦幅の値
	constexpr float fRadius = 70.0f;	// UIの範囲表示の半径の値
};

//=========================================================
// コンストラクタ
//=========================================================
CDocumentUI::CDocumentUI(int nPriority) : CTutorialUI(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CDocumentUI::~CDocumentUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CDocumentUI* CDocumentUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pTexName)
{
	// インスタンス生成
	CDocumentUI* pDocumentUI = new CDocumentUI;
	if (pDocumentUI == nullptr) return nullptr;

	// オブジェクトセット
	pDocumentUI->SetPos(pos);
	pDocumentUI->SetSize(DocumentUI::fWidth, DocumentUI::fHeight);
	pDocumentUI->SetRot(rot);
	pDocumentUI->SetRadius(DocumentUI::fRadius);
	pDocumentUI->SetTexture(pTexName);
	pDocumentUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pDocumentUI->Init())) return nullptr;

	// 書類タスクUIのポインタを返す
	return pDocumentUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDocumentUI::Init(void)
{
	// 親クラスの初期化処理
	CTutorialUI::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDocumentUI::Uninit(void)
{
	// 親クラスの終了処理
	CTutorialUI::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CDocumentUI::Update(void)
{
	// 親クラスの更新処理
	CTutorialUI::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CDocumentUI::Draw(void)
{
	// 親クラスの描画処理
	CTutorialUI::Draw();
}