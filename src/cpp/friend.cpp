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
#include "motion.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace FRIENDINFO
{
	constexpr int CHANGETIME_MAX = 600; // モーション変更カウント 
	constexpr float OUTLINESIZE = 0.96f; // アウトラインサイズ
	constexpr float SPHERE_SIZE = 60.0f; // 球形サイズ
	constexpr const char* SCRIPT = "data/MOTION/Friend/FriendMotion.txt"; // モーションファイル

	const D3DXVECTOR3 BOX_SIZE = { 50.0f,50.0f,50.0f };  // 矩形サイズ

};

//=========================================================
// コンストラクタ
//=========================================================
CFriend::CFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_nChangeTime(NULL)
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
	pFriend->SetUseOutLine(true);
	pFriend->SetOutLineSize(FRIENDINFO::OUTLINESIZE);

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
	MotionLoad(FRIENDINFO::SCRIPT, MAX, false);

	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot(); // オブジェクトの回転角度を取得

	// X, Y, Zの回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), FRIENDINFO::BOX_SIZE,matRot);

	// スフィアコライダーの生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), FRIENDINFO::SPHERE_SIZE);

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
	// 一定間隔でモーションのランダム更新
	m_nChangeTime++;
	
	// 10秒たったら更新
	if (m_nChangeTime >= FRIENDINFO::CHANGETIME_MAX)
	{
		// ローカルランダム数
		int nRand = rand() % MOTION::MAX;

		// モーションセット
		GetMotion()->SetMotion(nRand,true,10);

		// 変更時間を初期化する
		m_nChangeTime = NULL;
	}

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
bool CFriend::CollisionBox(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pBoxCollider.get(), pOther, OutPos);
}