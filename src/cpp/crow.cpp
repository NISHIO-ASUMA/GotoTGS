//=========================================================
//
// カラス処理 [ crow.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "crow.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "motion.h"

//=========================================================
// コンストラクタ
//=========================================================
CCrowBird::CCrowBird(int nPriority) : CNoMoveCharactor(nPriority),
m_nMotionType(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CCrowBird::~CCrowBird()
{

}
//=========================================================
// 生成処理
//=========================================================
CCrowBird* CCrowBird::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const int &nMotionType)
{
	// インスタンス生成
	CCrowBird* pBird = new CCrowBird;
	if (!pBird) return nullptr;

	// オブジェクト設定
	pBird->SetPos(pos);
	pBird->SetRot(rot);
	pBird->SetUseStencil(false);
	pBird->m_nMotionType = nMotionType;

	// 初期化失敗時
	if (FAILED(pBird->Init())) return nullptr;

	return pBird;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CCrowBird::Init(void)
{
	// 親クラスの初期化
	CNoMoveCharactor::Init();

	// モーション読み込み
	MotionLoad("data/MOTION/Crow/CrowMotion.txt", MOTION::MAX, true);

	// 開始モーション設定
	GetMotion()->SetMotion(m_nMotionType,true,3);

	// サイズを下げる
	SetScale({ 0.5f, 0.5f, 0.5f });
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CCrowBird::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CCrowBird::Update(void)
{
	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CCrowBird::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}