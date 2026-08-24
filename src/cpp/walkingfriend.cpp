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
m_pBoxCollider(nullptr),
m_fMoveDistance(NULL),
m_MoveType(NULL),
m_nStopCount(NULL),
m_fSpeed(NULL),
m_nMotionNumber(NULL),
m_TargetPos(VECTOR3_NULL),
m_SavePos(VECTOR3_NULL),
m_nMoveTypeIndex(NULL),
m_isReturning(false)
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
CWalkFriend* CWalkFriend::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const char* MotionName, 
	const int& nMoveType, 
	const float& fMoveValue,
	const float& fSpeed,
	const int& nMotionNumber
)
{
	// インスタンス生成
	CWalkFriend* pWalk = new CWalkFriend;
	if (pWalk == nullptr) return nullptr;

	// オブジェクト設定
	pWalk->SetPos(pos);
	pWalk->SetSavePos(pos);
	pWalk->SetRot(rot);
	pWalk->SetFileName(MotionName);
	pWalk->SetMoveType(nMoveType);
	pWalk->SetMoveValue(fMoveValue);
	pWalk->SetMoveSpeed(fSpeed);
	pWalk->SetMathTargetPos(pos);
	pWalk->SetMotionNumber(nMotionNumber);
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

	// ファイル名が"マイケル"なら
	if (m_pFileName == "data/MOTION/Mob/MobMotionMj.txt")
	{
		m_nMoveTypeIndex = 1;
	}
	else
	{
		m_nMoveTypeIndex = 0;
	}

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
	// 動かないタイプなら処理を通さない
	if (this->m_MoveType == MOVING::NONE)
	{
		// 目的角を設定
		SetRotDest(GetRot());

		// 移動しない
		SetMove(VECTOR3_NULL);

		// もし"遊びモーション番号"なら
		if (this->m_nMotionNumber == MOTION::PLAY)
			GetMotion()->SetMotion(MOTION::PLAY,true,5);
		else
			GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);

		// 親クラスだけ更新
		CMoveCharactor::Update();
		return;
	}

	// 各移動方向に応じた処理
	UpdateMovingType();

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
//========================================================
// 目的地座標の計算関数
//========================================================
void CWalkFriend::SetMathTargetPos(const D3DXVECTOR3& startpos)
{
	// 初期化
	m_TargetPos = VECTOR3_NULL;

	// 代入
	m_TargetPos = startpos;

	// 種類によって変更する
	switch (m_MoveType)
	{
	case MOVING::MOVE_FRONT_Z: m_TargetPos.z -= m_fMoveDistance; break; // 手前(-Z)
	case MOVING::MOVE_BACK_Z:  m_TargetPos.z += m_fMoveDistance; break; // 奥行き(+Z)
	case MOVING::MOVE_LEFT:    m_TargetPos.x -= m_fMoveDistance; break; // 左(-X)
	case MOVING::MOVE_RIGHT:   m_TargetPos.x += m_fMoveDistance; break; // 右(+X)
	default: break;
	}
}
//========================================================
// 種類に応じた移動処理
//========================================================
void CWalkFriend::UpdateMovingType(void)
{
	// インデックスに応じて通す更新関数を選択する
	switch (m_nMoveTypeIndex)
	{
	case 0:
		UpdateNormal(); // 通常移動
		break;

	case 1:
		UpdateMj();		// マイケル移動
		break;

	default:
		break;
	}
}
//========================================================
// 通常モード
//========================================================
void CWalkFriend::UpdateNormal(void)
{
	// 待機中の処理
	if (m_isSet)
	{
		m_nStopCount++;

		if (m_nStopCount >= MaxStop)
		{
			m_isSet = false;
			m_nStopCount = 0;
			m_isReturning = !m_isReturning;
		}

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 現在の目的地となる座標を決定
	D3DXVECTOR3 targetPos = (!m_isReturning) ? m_TargetPos : m_SavePos;

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float fDistanceToGoal = D3DXVec3Length(&vecToTarget);

	// 目標に近づいた
	if (fDistanceToGoal <= 2.0f)
	{
		m_isSet = true;
		SetPos(targetPos);		// 座標を目的地にぴったり合わせる
		SetMove(VECTOR3_NULL);	// 移動を止める

		// 到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化して移動方向を決定
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 変数初期化
	float fTargetRot = NULL;

	// X方向の時は変更する
	if (m_MoveType == MOVING::MOVE_LEFT || m_MoveType == MOVING::MOVE_RIGHT)
	{
		fTargetRot = atan2f(moveVec.x, -moveVec.z);
	}
	else
	{
		// 既存の計算式
		fTargetRot = atan2f(-moveVec.x, -moveVec.z);
	}

	// 目的角を設定
	SetRotDest(D3DXVECTOR3(GetRotDest().x, -fTargetRot, GetRotDest().z));

	// スピードを適用して移動量を設定
	moveVec *= m_fSpeed;
	SetMove(moveVec);

	// 移動モーション
	GetMotion()->SetMotion(MOTION::WALK, true, 5);
}
//========================================================
// マイケルモードの移動処理
//========================================================
void CWalkFriend::UpdateMj(void)
{
	// 待機中の処理
	if (m_isSet)
	{
		m_nStopCount++;

		if (m_nStopCount >= MaxStop)
		{
			m_isSet = false;
			m_nStopCount = 0;
			m_isReturning = !m_isReturning;
		}

		// ニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// ゴールへの距離
	float fDistanceToGoal = 0.0f;

	// フラグがfalse
	if (!m_isReturning)
	{
		// 目的地に近いか
		D3DXVECTOR3 diff = pos - m_TargetPos;
		fDistanceToGoal = D3DXVec3Length(&diff);
	}
	else
	{
		// 元居た座標にもどってくる
		D3DXVECTOR3 diff = pos - m_SavePos;
		fDistanceToGoal = D3DXVec3Length(&diff);
	}

	// 目標に近づいたら
	if (fDistanceToGoal <= 2.0f)
	{
		m_isSet = true;
		SetMove(VECTOR3_NULL);
		return;
	}

	// 目的角の計算
	float fTargetRot = 0.0f;
	int MoveType = GetMoveType();

	switch (MoveType)
	{
	case MOVING::MOVE_FRONT_Z: fTargetRot = -D3DX_PI;		break;	// 手前(-Z)
	case MOVING::MOVE_BACK_Z:  fTargetRot = 0.0f;			break;	// 奥行き(+Z)
	case MOVING::MOVE_LEFT:    fTargetRot = -D3DX_PI / 2.0f; break; // 左(-X)
	case MOVING::MOVE_RIGHT:   fTargetRot = D3DX_PI / 2.0f;  break; // 右(+X)
	default: break;
	}

	// 向きの反転
	if (m_isReturning)
	{
		fTargetRot += D3DX_PI;
	}

	// 移動方向の計算
	D3DXVECTOR3 forward;
	forward.x = sinf(fTargetRot) * m_fSpeed;
	forward.y = 0.0f;
	forward.z = cosf(fTargetRot) * m_fSpeed;

	// 目的角を設定
	SetRotDest(D3DXVECTOR3(GetRotDest().x, -fTargetRot, GetRotDest().z));

	// 移動量の設定
	SetMove(forward);

	// 移動モーション
	GetMotion()->SetMotion(MOTION::WALK, true, 5);
}