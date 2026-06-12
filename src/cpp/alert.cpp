//=========================================================
//
// 警告表示処理 [ alert.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "alert.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CAlert::CAlert(int nPriority):CAnimationObject2D(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAlert::~CAlert()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CAlert* CAlert::Create(const Alert& alert)
{
	// インスタンス生成
	CAlert* pAlert = new CAlert;

	// ヌルチェック
	if (pAlert == nullptr) return nullptr;

	// 各設定処理
	pAlert->SetPos(alert.pos);
	pAlert->SetCol(alert.col);
	pAlert->SetSize(alert.fWidth, alert.fHeight);
	pAlert->SetUV(alert.tex.x, alert.tex.y);
	pAlert->SetAnimFlag(alert.isLoop);
	pAlert->SetUse(alert.bUse);
	pAlert->SetTexture(Config::TEXNAME);

	// 初期化が失敗したとき
	if (FAILED(pAlert->Init())) return nullptr;

	return pAlert;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAlert::Init(void)
{
	// 親の初期化処理
	CAnimationObject2D::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CAlert::Uninit(void)
{
	// 親の終了処理
	CAnimationObject2D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CAlert::Update(void)
{
	// 使用していないなら
	if (GetUse() != true) return;
	
	// 1フレーム毎のテクスチャ移動量
	float fTexU = CEasing::SetEase(GetFreamCount(), Config::MAX_FREAM);

	float fLeftU = CEasing::EaseOutQuad(fTexU);
	float fRightU = CEasing::EaseOutQuad(fTexU) + GetTex().x;

	// テクスチャの横移動処理
	SetTexMoveU(Config::MAX_FREAM, fLeftU, fRightU);

	// 親の更新処理
	CAnimationObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CAlert::Draw(void)
{
	// 使用していないなら
	if (GetUse() != true) return;

	// 親の描画処理
	CAnimationObject2D::Draw();
}