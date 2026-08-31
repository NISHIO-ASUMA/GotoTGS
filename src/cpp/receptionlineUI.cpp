//=========================================================
//
// 受付人のセリフUI処理 [ receptionlineUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "receptionlineUI.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace ReceptionlineUI
{
	constexpr float fWidth = 15.0f;		// 横幅
	constexpr float fHeight = 15.0f;	// 縦幅
	constexpr float fRadius = 70.0f;	// 範囲表示の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CReceptionlineUI::CReceptionlineUI(int nPriority) : CTutorialUI(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CReceptionlineUI::~CReceptionlineUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CReceptionlineUI* CReceptionlineUI::Create(const D3DXVECTOR3& pos, const char* pTexName)
{
	// インスタンス生成
	CReceptionlineUI* pReceptionlineUI = new CReceptionlineUI;
	if (pReceptionlineUI == nullptr) return nullptr;

	// 各設定処理
	pReceptionlineUI->SetPos(pos);
	pReceptionlineUI->SetSize(ReceptionlineUI::fWidth, ReceptionlineUI::fHeight);
	pReceptionlineUI->SetRadius(ReceptionlineUI::fRadius);
	pReceptionlineUI->SetTexture(pTexName);
	pReceptionlineUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pReceptionlineUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pReceptionlineUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CReceptionlineUI::Init(void)
{
	// 親クラスの初期化処理
	CTutorialUI::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CReceptionlineUI::Uninit(void)
{
	// 親クラスの終了処理
	CTutorialUI::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CReceptionlineUI::Update(void)
{
	// 親クラスの更新処理
	CTutorialUI::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CReceptionlineUI::Draw(void)
{
	// 親クラスの描画処理
	CTutorialUI::Draw();
}