//=========================================================
//
// 外出タスク用UI処理 [ outsideUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "outsideUI.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace OutsideUI
{
	constexpr float fWidth = 15.0f;		// UIの横幅の値
	constexpr float fHeight = 15.0f;	// UIの縦幅の値
	constexpr float fRadius = 70.0f;	// UIの範囲表示の半径の値
};

//=========================================================
// コンストラクタ
//=========================================================
COutsideUI::COutsideUI(int nPriority) : CTutorialUI(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
COutsideUI::~COutsideUI()
{

}

//=========================================================
// 生成処理
//=========================================================
COutsideUI* COutsideUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pTexName)
{
	// インスタンス生成
	COutsideUI* pCpUI = new COutsideUI;
	if (pCpUI == nullptr) return nullptr;

	// オブジェクトセット
	pCpUI->SetPos(pos);
	pCpUI->SetSize(OutsideUI::fWidth, OutsideUI::fHeight);
	pCpUI->SetRot(rot);
	pCpUI->SetRadius(OutsideUI::fRadius);
	pCpUI->SetTexture(pTexName);
	pCpUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pCpUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pCpUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT COutsideUI::Init(void)
{
	// 親クラスの初期化処理
	CTutorialUI::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void COutsideUI::Uninit(void)
{
	// 親クラスの終了処理
	CTutorialUI::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void COutsideUI::Update(void)
{
	// 親クラスの更新処理
	CTutorialUI::Update();
}
//=========================================================
// 描画処理
//=========================================================
void COutsideUI::Draw(void)
{
	// 親クラスの描画処理
	CTutorialUI::Draw();
}