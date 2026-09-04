//=========================================================
//
// カメラアニメーション時に出すUI [ cameraui.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "cameraui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//========================================================
// コンストラクタ
//========================================================
CCameraUI::CCameraUI(int nPriority) : CObject2D(nPriority),
m_Movement(NULL),
m_nLifeInterval(NULL)
{

}
//========================================================
// デストラクタ
//========================================================
CCameraUI::~CCameraUI()
{

}
//========================================================
// 生成処理
//========================================================
CCameraUI* CCameraUI::Create
(
	const D3DXVECTOR3& pos,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	const int& nMaxInterval,
	const int& nMoveStartSetNumber
)
{
	// インスタンス生成
	CCameraUI* pUi = new CCameraUI;
	if (pUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init())) return nullptr;

	// オブジェクト設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);
	pUi->SetTexture(Filename);
	pUi->SetAnchor();
	pUi->m_Movement = nMoveStartSetNumber;
	pUi->m_nLifeInterval = nMaxInterval;

	return pUi;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CCameraUI::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CCameraUI::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CCameraUI::Update(void)
{
	// 親クラスの更新処理
	CObject2D::Update();
}
//========================================================
// 描画処理
//========================================================
void CCameraUI::Draw(void)
{
	// 親クラスの描画処理
	CObject2D::Draw();
}