//========================================================
//
// 敵の処理 [ enemy.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemy.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionbox.h"
#include "collisionsphere.h"
#include "blockmanager.h"
#include "block.h"
#include "jsonmanager.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//========================================================
// コンストラクタ
//========================================================
CEnemy::CEnemy(int nPriority) : CMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr)
{

}
//========================================================
// デストラクタ
//========================================================
CEnemy::~CEnemy()
{

}
//========================================================
// 生成処理
//========================================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) return nullptr;

	// オブジェクト設定
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetUseOutLine(true);

	// 初期化設定時
	if (FAILED(pEnemy->Init())) return nullptr;

	return pEnemy;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CEnemy::Init(void)
{
	// キャラクター初期化
	CMoveCharactor::Init();

	// モーションファイルロード
	MotionLoad("data/MOTION/Enemy/EnemyMotion.txt", MOTION::MAX, true);

	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot(); // オブジェクトの回転角度を取得

	// X, Y, Zの回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成
	m_pBoxColiider = CBoxCollider::Create(GetPos(), GetPos(), D3DXVECTOR3(Config::BOX_RANGE, Config::BOX_RANGE, Config::BOX_RANGE),matRot);

	// 球形コライダー生成
	m_pSphereColiider = CSphereCollider::Create(GetPos(),Config::SPHERE_RANGE);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CEnemy::Uninit(void)
{
	// ポインタの破棄
	m_pBoxColiider.reset();
	m_pSphereColiider.reset();

	// キャラクター終了
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CEnemy::Update(void)
{
#if 0
	// 現在座標取得
	auto NowPos = GetPos();
	auto OldPos = GetOldPos();

	// キャラクター座標更新
	CMoveCharactor::UpdatePosition();

	// 更新後の座標取得
	auto UpdatePos = GetPos();

	// 球形コライダー座標更新
	if (m_pSphereColiider)
	{
		m_pSphereColiider->SetPos(UpdatePos);
	}

	// 矩形コライダーの位置更新
	if (m_pBoxColiider)
	{
		m_pBoxColiider->SetPos(UpdatePos);
		m_pBoxColiider->SetPosOld(OldPos);
	}

	// jsonmanagerからブロックを取得
	const auto& BlockManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (BlockManager == nullptr) return;

	// 最大ブロックを取得する
	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		// 各ブロックを取得し判定を生成
		auto IdxBlock = BlockManager->GetBlock(nCnt);
		if (Collision(IdxBlock->GetCollider(), &UpdatePos))
		{
			// 当たった点の座標セット
			SetPos(UpdatePos);

			// コライダーと現在座標の更新をする
			m_pBoxColiider->SetPos(UpdatePos);
		}
	}
#endif
	// キャラクター全体更新
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CEnemy::Draw(void)
{
	// キャラクター描画
	CMoveCharactor::Draw();
}
//========================================================
// 矩形コリジョン判定
//========================================================
bool CEnemy::Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos)
{
	if (!m_pBoxColiider) return false;

	// 当たり判定処理
	return CCollisionBox::CollisionEx(m_pBoxColiider.get(), pOther, pOutPos);
}