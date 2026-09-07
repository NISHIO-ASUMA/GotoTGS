//========================================================
//
// オフィスにもどれの警告表示ui [ returnofficeui.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "returnofficeui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "manager.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CReturnOfficeUi::CReturnOfficeUi(int nPriority) : CObject2D(nPriority),
m_nAlphaCnt(NULL),
m_nAlphaFrame(NULL),
m_nFlashFrame(NULL),
m_bUse(true)
{

}
//=========================================================
// デストラクタ
//=========================================================
CReturnOfficeUi::~CReturnOfficeUi()
{

}
//=========================================================
// 生成処理
//=========================================================
CReturnOfficeUi* CReturnOfficeUi::Create
(
	const D3DXVECTOR3& pos,
	const int& nFlashFrame,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	bool bUse
)
{
	// インスタンス生成
	CReturnOfficeUi* pUi = new CReturnOfficeUi;
	if (pUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init())) return nullptr;

	// オブジェクト設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);
	pUi->SetAnchor(CObject2D::ANCHORTYPE_CENTER);
	pUi->SetTexture(Filename);
	pUi->SetUse(bUse);
	pUi->m_nFlashFrame = nFlashFrame;

	// 生成されたポインタを返す
	return pUi;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CReturnOfficeUi::Init(void)
{
	// オブジェクトの初期化
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CReturnOfficeUi::Uninit(void)
{
	// オブジェクトの終了
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CReturnOfficeUi::Update(void)
{
	if (!m_bUse)
		return;

	// 点滅処理
	SetFlash(NULL, m_nFlashFrame, COLOR_WHITE);
	
	// 親クラス更新
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CReturnOfficeUi::Draw(void)
{
	if (!m_bUse)
		return;

	// 親クラスの描画
	CObject2D::Draw();
}