//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "player.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "xfilemanager.h"
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionbox.h"
#include "playerstatebase.h"
#include "playerstateneutral.h"
#include "statemachine.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pMachine(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CPlayer::~CPlayer()
{

}
//=========================================================
// 生成処理
//=========================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CPlayer* pPlayer = new CPlayer;
	if(pPlayer == nullptr)return nullptr;

	// オブジェクト設定
	pPlayer->SetPos(pos);
	pPlayer->SetRot(rot);
	pPlayer->SetUseStencil(true);

	// 初期化失敗時
	if (FAILED(pPlayer->Init()))return nullptr;

	return pPlayer;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPlayer::Init(void)
{
	// 親クラスの初期化処理
	CMoveCharactor::Init();

	// モーション読み込み
	MotionLoad("data/MOTION/PlayerMotion.txt",MAX,false);

	// インスタンス生成
	m_pMachine = new CStateMachine;

	// 初期ステートをセット
	ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), D3DXVECTOR3(50.0f,50.0f,50.0f));

	// スフィアコライダーの生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), 60.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPlayer::Uninit(void)
{
	// ステートマシンの破棄
	if (m_pMachine)
	{
		m_pMachine->OnExit();
		delete m_pMachine;
		m_pMachine = nullptr;
	}
	
	// ボックスコライダーの破棄
	m_pBoxCollider.reset();

	// スフィアコライダーの破棄
	m_pSphereCollider.reset();	

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CPlayer::Update(void)
{
	// ステートマシンの更新処理
	m_pMachine->Update();

	// 座標の更新処理
	CMoveCharactor::UpdatePosition();

	// 親クラスの更新処理
	CMoveCharactor::Update();

}
//=========================================================
// 描画処理
//=========================================================
void CPlayer::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();
}
//=========================================================
// 当たり判定
//=========================================================
bool CPlayer::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pBoxCollider.get(), pOther, OutPos);
}
//=========================================================
// ステート変更処理
//=========================================================
void CPlayer::ChangeState(CPlayerStateBase* pState, int nID)
{
	// 自分自身のポインタを設定
	pState->SetOwner(this);

	// IDの設定
	pState->SetID(nID);

	// ステート変更
	m_pMachine->ChangeState(pState);
}
