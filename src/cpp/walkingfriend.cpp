//========================================================
//
// 歩いている同僚の処理 [ walkingfriend.cpp ]
// Author: Asuma Nishio
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "walkingfriend.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "motion.h"
#include "boxcollider.h"
#include "collisionbox.h"

//========================================================
// コンストラクタ
//========================================================
CWalkFriend::CWalkFriend(int nPriority) : CMoveCharactor(nPriority),
m_isSet(false),
m_pBoxCollider(nullptr)
{

}
//========================================================
// デストラクタ
//========================================================
CWalkFriend::~CWalkFriend()
{

}
//========================================================
// 生成処理
//========================================================
CWalkFriend* CWalkFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* MotionName)
{
	// インスタンス生成
	CWalkFriend* pWalk = new CWalkFriend;
	if (pWalk == nullptr) return nullptr;

	// オブジェクト設定
	pWalk->SetPos(pos);
	pWalk->SetRot(rot);
	pWalk->SetFileName(MotionName);
	pWalk->SetUseOutLine(true);
	pWalk->SetOutlineColor(D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f));

	// 初期化失敗時
	if (FAILED(pWalk->Init())) return nullptr;

	return pWalk;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CWalkFriend::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad(m_pFileName, MOTION::MAX, false);

	// 回転角を生成
	D3DXMATRIX mtxRot;
	D3DXVECTOR3 rot = GetRot();

	// 回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

	// コライダー生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), BoxSize, mtxRot);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CWalkFriend::Uninit(void)
{
	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CWalkFriend::Update(void)
{
	// 移動処理



	// 座標の更新
	CMoveCharactor::UpdatePosition();

	// 更新後の座標取得
	D3DXVECTOR3 UpdatePos = GetPos();

	// ボックスコライダー座標の更新
	if (m_pBoxCollider)
	{
		m_pBoxCollider->SetPos(UpdatePos);
		m_pBoxCollider->SetPosOld(GetOldPos());
	}

	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CWalkFriend::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();
}
//========================================================
// 矩形の当たり判定
//========================================================
bool CWalkFriend::Collision(CBoxCollider* pOther, D3DXVECTOR3* PushPos)
{
	if (!m_pBoxCollider) return false;

	// 矩形同士の関数
	return CCollisionBox::CollisionEx(m_pBoxCollider.get(),pOther,PushPos);
}