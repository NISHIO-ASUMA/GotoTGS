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
#include "manager.h"
#include "debugproc.h"
#include "gamesceneobject.h"
#include "player.h"
#include "template.h"
#include "statemachine.h"
#include "enemystatebase.h"
#include "enemystateneutral.h"
#include "enemyutility.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace EnemyInfo
{
	constexpr const char* SCRIPT = "data/MOTION/Enemy/EnemyMotion.txt"; // モーションファイル
	constexpr int VIEWPOINT = 4; // ポイント数

	constexpr float SPEED = 1.0f; // 移動速度
	constexpr float RANGE = 2.0f; // 判定範囲

	// ビューポイント配列 ( 最初に出てきている敵 )
	const D3DXVECTOR3 ViewPoint[VIEWPOINT] =
	{
		{-95.0f, 0.0f, 235.5f},
		{120.8f ,0.0f, 225.0f},
		{81.0f, 0.0f, -150.0f},
		{-110.0f,0.0f,-145.0f}
	};
};

//========================================================
// コンストラクタ
//========================================================
CEnemy::CEnemy(int nPriority) : CMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_pMachine(nullptr),
m_isCheckPoint(false),
m_isTargetChase(false),
m_pDestCharactor(nullptr),
m_nStopTime(NULL),
m_nTargetIdx(NULL),
m_playerTargetPos(VECTOR3_NULL)
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
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const MOVETYPE& Movetype)
{
	// インスタンス生成
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) return nullptr;

	// オブジェクト設定
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetMoveType(Movetype);
	pEnemy->SetUseOutLine(true);
	pEnemy->SetOutLineSize(0.35f);

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
	MotionLoad(EnemyInfo::SCRIPT, MOTION::MAX, true);

	// オブジェクトの回転角度を取得
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot();

	// 回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成
	m_pBoxColiider = CBoxCollider::Create(GetPos(), GetPos(), D3DXVECTOR3(Config::BOX_RANGE, Config::BOX_RANGE, Config::BOX_RANGE),matRot);

	// 球形コライダー生成
	m_pSphereColiider = CSphereCollider::Create(GetPos(),Config::SPHERE_RANGE);

	// ステートマシン生成
	m_pMachine = new CStateMachine;

	// nullじゃないなら
	if (m_pMachine)
	{
		// 初期状態を生成
		ChangeState(new CEnemyStateNeutral(), CEnemyStateBase::ID_NEUTRAL);
	}

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

	// ステートマシンの破棄
	if (m_pMachine)
	{
		m_pMachine->OnExit();
		delete m_pMachine;
		m_pMachine = nullptr;
	}

	// キャラクタークラス終了処理
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CEnemy::Update(void)
{
	// ステートの更新
	m_pMachine->Update();

	// キャラクター座標更新
	CMoveCharactor::UpdatePosition();

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

	// 扇形メッシュの描画
	DrawEyeSight();
}
//========================================================
// 通常ビューポイント追従処理
//========================================================
void CEnemy::UpdateMoveViewPoint(void)
{	
	// 停止カウント中の処理
	if (m_nStopTime > 0)
	{
		m_nStopTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = EnemyInfo::ViewPoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= EnemyInfo::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nStopTime = Config::COOL_TIME;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, EnemyInfo::VIEWPOINT - 1);

		// 目的地に到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= EnemyInfo::SPEED;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
}
//========================================================
// 煙草回りを周回する動き
//========================================================
void CEnemy::UpdateMovingSmoke(void)
{
	// 停止カウント中の処理
	if (m_nStopTime > 0)
	{
		// デクリメント
		m_nStopTime--;

		// ニュートラルにする
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = SMOKE_AND_MAGAZINE::ViewPoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= EnemyInfo::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nStopTime = Config::COOL_TIME_DOUBLE;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, SMOKE_AND_MAGAZINE::ALL_POINT - 1);

		// 目的地に到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= EnemyInfo::SPEED;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
}
//========================================================
// TVらへんを周回する動き
//========================================================
void CEnemy::UpdateMovingTV(void)
{
	// 停止カウント中の処理
	if (m_nStopTime > 0)
	{
		// デクリメント
		m_nStopTime--;

		// ニュートラルにする
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = TV_AND_EATING::ViewPoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= EnemyInfo::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nStopTime = Config::COOL_TIME_DOUBLE;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, TV_AND_EATING::ALL_POINT - 1);

		// 目的地に到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= EnemyInfo::SPEED;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
}
//========================================================
// ステート変更処理
//========================================================
void CEnemy::ChangeState(CEnemyStateBase* pState, int nID)
{
	// 自分自身のポインタを設定
	pState->SetOwner(this);

	// IDの設定
	pState->SetID(nID);

	// ステート変更
	m_pMachine->ChangeState(pState);
}
//========================================================
// 視界の扇形の描画処理
//========================================================
void CEnemy::DrawEyeSight(void)
{
	// 現在の設定を取得
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	float halfAngle = D3DXToRadian(Eyesight::EYE_ANGLE / 2.0f);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return;

	//==========================================
	// 現在のデバイス設定を保存
	//==========================================
	DWORD oldLighting, oldAlphaBlend, oldSrcBlend, oldDestBlend, oldCullMode;
	DWORD oldColorOp, oldColorArg2, oldAlphaOp, oldAlphaArg2;
	LPDIRECT3DBASETEXTURE9 pOldTexture = nullptr;
	D3DXMATRIX matOldWorld, matIdentity;

	// レンダー状態の取得
	pDevice->GetRenderState(D3DRS_LIGHTING, &oldLighting);
	pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaBlend);
	pDevice->GetRenderState(D3DRS_SRCBLEND, &oldSrcBlend);
	pDevice->GetRenderState(D3DRS_DESTBLEND, &oldDestBlend);
	pDevice->GetRenderState(D3DRS_CULLMODE, &oldCullMode);

	// テクスチャステージ状態の取得
	pDevice->GetTexture(0, &pOldTexture);
	pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &oldColorOp);
	pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &oldColorArg2);
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &oldAlphaOp);
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &oldAlphaArg2);

	// 行列の取得
	pDevice->GetTransform(D3DTS_WORLD, &matOldWorld);

	//==========================================
	// 視界描画用の設定を適用
	//==========================================
	pDevice->SetTexture(0, nullptr);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// マトリックスの設定
	D3DXMatrixIdentity(&matIdentity);
	pDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	//==========================================
	// 頂点バッファの構築と描画
	//==========================================
	std::vector<VERTEX_3D> vFan(1 + (Config::DIVIDE + 1));

	// 中心点
	vFan[0].pos = D3DXVECTOR3(enemyPos.x, enemyPos.y + 0.1f, enemyPos.z);
	vFan[0].nor = VECTOR3_NULL;
	vFan[0].col = D3DXCOLOR(1.0f,0.0f,0.0f,0.6f);
	vFan[0].tex = VECTOR2_NULL;

	// 外周点
	for (int i = 0; i <= Config::DIVIDE; ++i)
	{
		float t = (float)i / (float)Config::DIVIDE;
		float currentAngle = rot.y - halfAngle + (D3DXToRadian(Eyesight::EYE_ANGLE) * t);

		D3DXVECTOR3 dir(-sinf(currentAngle), 0.0f, -cosf(currentAngle));

		int idx = i + 1;
		vFan[idx].pos = vFan[0].pos + dir * Eyesight::EYE_RADIUS;
		vFan[idx].nor = VECTOR3_NULL;
		vFan[idx].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
		vFan[idx].tex = VECTOR2_NULL;
	}

	// 頂点フォーマットの設定	
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, Config::DIVIDE, &vFan[0], sizeof(VERTEX_3D));

	//==========================================
	// すべてのデバイス設定を完全に元に戻す
	//==========================================
	pDevice->SetTransform(D3DTS_WORLD, &matOldWorld);

	pDevice->SetRenderState(D3DRS_LIGHTING, oldLighting);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaBlend);
	pDevice->SetRenderState(D3DRS_SRCBLEND, oldSrcBlend);
	pDevice->SetRenderState(D3DRS_DESTBLEND, oldDestBlend);
	pDevice->SetRenderState(D3DRS_CULLMODE, oldCullMode);

	pDevice->SetTexture(0, pOldTexture);
	if (pOldTexture) pOldTexture->Release();

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, oldColorArg2);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, oldAlphaArg2);
}
//========================================================
// 扇形の視界判定
//========================================================
bool CEnemy::CheckEyesight(void)
{
	// もしnullなら
	if (!m_pDestCharactor) return false;

	// 敵の現在座標を取得
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 CharactorPos = m_pDestCharactor->GetPos();

	// 高さの判定
	float heightDiff = fabsf(CharactorPos.y - enemyPos.y);

	if (heightDiff > Eyesight::EYE_HEIGHT / 2.0f)
	{
		return false; // 高さが範囲外
	}

	// 距離の判定
	D3DXVECTOR3 diff = CharactorPos - enemyPos;
	diff.y = 0.0f;

	// 距離の2乗を計算
	float sqrDistance = D3DXVec3LengthSq(&diff);

	if (sqrDistance > Eyesight::EYE_RADIUS * Eyesight::EYE_RADIUS)
	{
		return false; // 判定外
	}

	// ゼロ除算
	if (sqrDistance < 0.0001f) return true;

	// 角度を取得
	D3DXVECTOR3 rot = GetRot();

	// 角度から方向ベクトルを生成
	D3DXVECTOR3 enemyForward(-sinf(rot.y), 0.0f, -cosf(rot.y));
	D3DXVec3Normalize(&enemyForward, &enemyForward);

	// 方向ベクトルを正規化
	D3DXVECTOR3 diffDir;
	D3DXVec3Normalize(&diffDir, &diff);

	// 内積を計算
	float dot = D3DXVec3Dot(&enemyForward, &diffDir);

	// 角度のコサイン値を計算
	float halfAngleRad = D3DXToRadian(Eyesight::EYE_ANGLE);
	float cosHalfAngle = cosf(halfAngleRad);

	// 内積判定
	if (dot >= cosHalfAngle)
	{
		return true; // 視界に入っている
	}

	return false;
}
//========================================================
// 矩形コリジョン判定
//========================================================
bool CEnemy::Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos)
{
	// もしコライダーがないなら
	if (!m_pBoxColiider) return false;

	// 当たり判定処理
	return CCollisionBox::CollisionEx(m_pBoxColiider.get(), pOther, pOutPos);
}