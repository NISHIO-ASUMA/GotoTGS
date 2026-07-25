//========================================================
//
// 外の監査役の処理 [ auditor.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditor.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "spherecollider.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace AUDITOR_INFO
{
	constexpr const char* FILENAME = "data/MOTION/AUDITOR/AuditorMotion.txt";
};

//========================================================
// コンストラクタ
//========================================================
CAuditor::CAuditor(int nPriority) : CMoveCharactor(nPriority),
m_nCoolTime(NULL),
m_nOfficeViewIdx(NULL),
m_nViewIdx(NULL),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr)
{
}
//========================================================
// デストラクタ
//========================================================
CAuditor::~CAuditor()
{

}
//========================================================
// 生成処理
//========================================================
CAuditor* CAuditor::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CAuditor* pAuditor = new CAuditor;
	if (pAuditor == nullptr) return nullptr;

	// オブジェクト設定
	pAuditor->SetPos(pos);
	pAuditor->SetRot(rot);
	pAuditor->SetUseOutLine(true);
	pAuditor->SetOutlineColor();

	// 初期化失敗時
	if (FAILED(pAuditor->Init())) return nullptr;

	return pAuditor;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CAuditor::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad(AUDITOR_INFO::FILENAME, MOTION::MAX, false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CAuditor::Uninit(void)
{
	// コライダー破棄
	m_pBoxColiider.reset();
	m_pSphereColiider.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CAuditor::Update(void)
{
	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CAuditor::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();
}
//========================================================
// ポイント間を動く処理
//========================================================
void CAuditor::MovePointOutSide(const D3DXVECTOR3& pos)
{

}