//========================================================
//
// 煙草吸っている同僚の処理 [ smokefriend.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "smokefriend.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "motion.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SMOKEFRIEND
{
	constexpr const char* SCRIPT = "data/MOTION/Smoke/SmokeMotion.txt"; // モーションファイル
};

//========================================================
// コンストラクタ
//========================================================
CSmokeFriend::CSmokeFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeCount(NULL)
{

}
//========================================================
// デストラクタ
//========================================================
CSmokeFriend::~CSmokeFriend()
{

}
//========================================================
// 生成処理
//========================================================
CSmokeFriend* CSmokeFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CSmokeFriend* pSmoke = new CSmokeFriend;
	if (pSmoke == nullptr) return nullptr;

	// オブジェクト設定
	pSmoke->SetPos(pos);
	pSmoke->SetRot(rot);
	pSmoke->SetUseOutLine(true);
	pSmoke->SetOutLineColor(D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f));

	// 初期化失敗時
	if (FAILED(pSmoke->Init())) return nullptr;

	return pSmoke;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CSmokeFriend::Init(void)
{
	// 親クラスの初期化
	CNoMoveCharactor::Init();

	// モーションロード
	MotionLoad(SMOKEFRIEND::SCRIPT, MOTION::MAX,false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CSmokeFriend::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CSmokeFriend::Update(void)
{
	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CSmokeFriend::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}