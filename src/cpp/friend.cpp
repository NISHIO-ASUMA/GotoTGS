//=========================================================
//
// 同僚処理 [ friend.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "friend.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionbox.h"

//=========================================================
// コンストラクタ
//=========================================================
CFriend::CFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CFriend::~CFriend()
{

}
//=========================================================
// 生成処理
//=========================================================
CFriend* CFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CFriend* pFriend = new CFriend;
	if (pFriend == nullptr)return nullptr;

	// オブジェクト設定
	pFriend->SetPos(pos);
	pFriend->SetRot(rot);
	pFriend->SetUseStencil(true);

	// 初期化失敗時
	if (FAILED(pFriend->Init()))return nullptr;

	return pFriend;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CFriend::Init(void)
{
	// 親クラスの初期化処理
	CNoMoveCharactor::Init();

	// モーション読み込み
	MotionLoad("data/MOTION/FriendMotion.txt", MAX, false);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	// スフィアコライダーの生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), 60.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CFriend::Uninit(void)
{
	// ボックスコライダーの破棄
	m_pBoxCollider.reset();

	// スフィアコライダーの破棄
	m_pSphereCollider.reset();

	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CFriend::Update(void)
{
	// 親クラスの更新処理
	CNoMoveCharactor::Update();

}
//=========================================================
// 描画処理
//=========================================================
void CFriend::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}
//=========================================================
// 当たり判定
//=========================================================
bool CFriend::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pBoxCollider.get(), pOther, OutPos);
}