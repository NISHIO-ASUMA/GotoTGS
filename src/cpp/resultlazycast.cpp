//=========================================================
//
// 捕まった時のキャスト達 [ resultlazycast.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultlazycast.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "motion.h"

//=========================================================
// コンストラクタ
//=========================================================
CResultCast::CResultCast(int nPriority) : CNoMoveCharactor(nPriority),
m_nMotionType(NULL),
m_MotionPath{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CResultCast::~CResultCast()
{

}
//=========================================================
// 生成処理
//=========================================================
CResultCast* CResultCast::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const int& nMotionType,
	const std::string& Path
)
{
	// インスタンス生成
	CResultCast* pCast = new CResultCast;
	if (!pCast) return nullptr;

	// オブジェクト設定
	pCast->SetPos(pos);
	pCast->SetRot(rot);
	pCast->SetUseStencil(false);
	pCast->m_MotionPath = Path;
	pCast->m_nMotionType = nMotionType;

	// 初期化失敗時
	if (FAILED(pCast->Init())) return nullptr;

	return pCast;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultCast::Init(void)
{
	// 親クラスの初期化
	CNoMoveCharactor::Init();

	// モーション読み込み
	MotionLoad(m_MotionPath.c_str(), MOTION::MAX, true);

	// 開始モーション設定
	GetMotion()->SetMotion(m_nMotionType, true, 3);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultCast::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CResultCast::Update(void)
{
	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CResultCast::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}