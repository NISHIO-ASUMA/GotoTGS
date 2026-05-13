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
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionbox.h"
#include "playerstatebase.h"
#include "playerstateneutral.h"
#include "statemachine.h"
#include "input.h"
#include "camera.h"
#include "blockmanager.h"
#include "jsonmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
namespace player
{
	constexpr float fSpeed = 5.0f;		// プレイヤーの移動スピード
	constexpr float fInput = 0.0001f;	// 移動処理に使うキーが入力されてるか比較する用の変数

};
//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pMachine(nullptr),
m_bMove(false)
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
	MotionLoad("data/MOTION/Player/PlayerMotion.txt",MAX,false);

	// インスタンス生成
	m_pMachine = new CStateMachine;

	// 初期ステートをセット
	ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetOldPos(), D3DXVECTOR3(50.0f,50.0f,50.0f));

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

	// カメラ基準の移動処理
	MoveBasedOnCamera(player::fSpeed);

	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー座標の更新
	if (m_pBoxCollider) m_pBoxCollider->SetPos(pos);

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

#ifdef _DEBUG
	// モーションのデバッグ表示
	GetMotion()->Debug();
#endif
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
//=================================================
// プレイヤー移動処理
//=================================================
void CPlayer::MoveBasedOnCamera(float speed)
{
	// キーボードのポインタ
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ジョイパッドのポインタ
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();

	// カメラのポインタ
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 向きの取得
	D3DXVECTOR3 rot = pCamera->GetRot();

	// ビューマトリックスの取得
	auto ViewMatrix = pCamera->GetView();

	// ビュー行列の逆行列を計算
	D3DXMATRIX invViewMat;
	D3DXMatrixInverse(&invViewMat, NULL, &ViewMatrix);

	// 逆行列からカメラの方向ベクトルを抽出
	D3DXVECTOR3 camForward = D3DXVECTOR3(invViewMat._31, invViewMat._32, invViewMat._33);
	D3DXVECTOR3 camRight = D3DXVECTOR3(invViewMat._11, invViewMat._12, invViewMat._13);

	// XZ平面の移動にするため、Y成分を0にする
	camForward.y = NULL;
	camRight.y = NULL;

	// 方向ベクトルの正規化
	D3DXVec3Normalize(&camForward, &camForward);
	D3DXVec3Normalize(&camRight, &camRight);

	// 移動方向の計算
	D3DXVECTOR3 moveDir = VECTOR3_NULL;

	// 目的の向き
	D3DXVECTOR3 RotDest = GetRotDest();

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		moveDir += camForward;
		RotDest.y = rot.y + D3DX_PI;
	}
	if (pKeyboard->GetPress(DIK_S) == true)
	{
		moveDir -= camForward;
		RotDest.y = rot.y;
	}
	if (pKeyboard->GetPress(DIK_D) == true)
	{
		moveDir += camRight;
		RotDest.y = rot.y - D3DX_PI * HALF;
	}
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		moveDir -= camRight;
		RotDest.y = rot.y + D3DX_PI * HALF;
	}

	// 移動入力がある場合
	if (D3DXVec3LengthSq(&moveDir) > player::fInput)
	{
		// 移動の正規化
		D3DXVec3Normalize(&moveDir, &moveDir);

		// 位置の更新
		SetMove(moveDir * speed);

		// 移動方向から向きを計算
		RotDest.y = atan2f(-moveDir.x, -moveDir.z);

		// 目的の向きを設定
		SetRotDest(RotDest);

		// 移動判定をtrueに
		m_bMove = true;
	}
	else
	{
		// 移動判定をfalseに
		m_bMove = false;
	}
}