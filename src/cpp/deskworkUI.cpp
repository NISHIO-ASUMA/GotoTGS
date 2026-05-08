//=========================================================
//
// タスクUI処理 [ deskworkUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "deskworkUI.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CDeskworkUI::CDeskworkUI(int nPriority) : CObject2D(nPriority),
m_pos(VECTOR3_NULL)
{

}

//=========================================================
// デストラクタ
//=========================================================
CDeskworkUI::~CDeskworkUI()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CDeskworkUI* CDeskworkUI::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight, const KEYTYPE& kyetype)
{
	// インスタンス生成
	CDeskworkUI* pDeskworkUI = new CDeskworkUI;

	// ヌルチェック
	if (pDeskworkUI == nullptr) return nullptr;

	// 各種値の設定
	pDeskworkUI->SetPos(pos);
	pDeskworkUI->SetSize(fWidth, fHeight);
	pDeskworkUI->SetKyeType(kyetype);

	// 初期化が失敗した場合
	if (FAILED(pDeskworkUI->Init())) return nullptr;

	return pDeskworkUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskworkUI::Init(void)
{
	// タスクUIの各種設定
	SetCol(COLOR_WHITE);			// カラー設定
	SetTexture(Config::TEXNAME);	// テクスチャ設定

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskworkUI::Uninit(void)
{
	// オブジェクト自身の破棄
	CObject::Release();
}

//=========================================================
// 更新処理
//=========================================================
void CDeskworkUI::Update(void)
{
	if (m_Kyetype == DRAWTYPE_W &&
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_W))
	{// Wを押した時

	}

	if (m_Kyetype == DRAWTYPE_A &&
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A))
	{// Aを押した時

	}

	if (m_Kyetype == DRAWTYPE_S &&
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_S))
	{// Sを押した時

	}

	if (m_Kyetype == DRAWTYPE_D &&
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D))
	{// Dを押した時

	}
}

//=========================================================
// 描画処理
//=========================================================
void CDeskworkUI::Draw(void)
{

}