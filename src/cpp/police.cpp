//========================================================
//
// 警察キャラクターの処理 [ police.cpp ]
// Author: Asuma Nishio
//
// NOTE : stateマシンとかでの制御も視野に入れる
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "police.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"
#include "collisionsphere.h"

//========================================================
// コンストラクタ
//========================================================
CPolice::CPolice(int nPriority) : CMoveCharactor(nPriority),
m_pSphereColiider(nullptr)
{

}
//========================================================
// デストラクタ
//========================================================
CPolice::~CPolice()
{

}
//========================================================
// 生成処理
//========================================================
CPolice* CPolice::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CPolice* pPolice = new CPolice;
	if (pPolice == nullptr) return nullptr;

	// オブジェクト設定
	pPolice->SetPos(pos);
	pPolice->SetRot(rot);
	pPolice->SetUseOutLine(true);

	// 初期化失敗時
	if (FAILED(pPolice->Init())) return nullptr;

	return pPolice;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CPolice::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーション読み込み
	MotionLoad("data/MOTION/police/PoliceMotion.txt", MOTION::MAX, false);

	// コライダー生成 ( 球 )
	m_pSphereColiider = CSphereCollider::Create(GetPos(), Config::SPHERE_RANGE);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CPolice::Uninit(void)
{
	// コライダーのリセット
	m_pSphereColiider.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();

}
//========================================================
// 更新処理
//========================================================
void CPolice::Update(void)
{
	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CPolice::Draw(void)
{
	// 親クラスの描画
	CMoveCharactor::Draw();
}
//========================================================
// 当たり判定処理 ( 球形 )
//========================================================
bool CPolice::Collision(CSphereCollider* pOther)
{
	if (m_pSphereColiider == nullptr) return false;

	return CCollisionSphere::Collision(m_pSphereColiider.get(), pOther);
}