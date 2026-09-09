//========================================================
//
// 敵の処理 [ enemy.cpp ]
// Author: Asuma Nishio
// 
// NOTE : だめだ ブロック貫通するわ 避けれへん 判定作っても
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
#include "billboard.h"
#include "jsonmanager.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "template.h"
#include "particle.h"
#include "camera.h"
#include "enemydoubtgauge.h"
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
m_isStartChase(false),
m_pDestCharactor(nullptr),
m_pChaseIcon(nullptr),
m_pGauge(nullptr),
m_nStopTime(NULL),
m_nTargetIdx(NULL),
m_fLevelPoint(NULL),
m_fEyeAngle(NULL),
m_fMoveSpeed(NULL),
m_nCurrentWayPointIdx(-1),
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

	// アイコン生成
	m_pChaseIcon = CBillboard::Create(GetPos(),VECTOR3_NULL,20.0f,20.0f,"ui_chaseicon.png");
	m_pChaseIcon->SetDrawFlags(false);

	// ui生成 ( ?のゲージ )
	auto CreatePos = D3DXVECTOR3(GetPos().x,GetPos().y + Config::VALUE_HEIGHT,GetPos().z);
	m_pGauge = CEnemyDoubtGauge::Create(CreatePos, Config::SIZE, Config::SIZE,"hatena.png","gauge_enemyside.png");

	// 初期値を設定
	m_fMoveSpeed = EnemyInfo::SPEED;
	m_fEyeAngle = Eyesight::EYE_ANGLE;

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
	// アニメーション中なら
	if (CManager::GetInstance()->GetCamera()->GetIsAnimTime() ||
		CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_BOSS_SYSTEM)
	{
		UpdateMotionOnly();
		return;
	}

	// 頭上のゲージの位置の更新
	D3DXVECTOR3 headPos = this->GetPos();
	headPos.y += Config::VALUE_HEIGHT;

	// ゲージのポイント設定
	m_pGauge->SetTargetPos(headPos);

	// ステートマシンの更新
	m_pMachine->Update();

	// キャラクター座標更新
	CMoveCharactor::UpdatePosition();

	// 更新取得
	auto UpdatePos = GetPos();

	// 球形コライダー更新
	if (m_pSphereColiider) 
		m_pSphereColiider->SetPos(GetPos());

#if 0
	// 矩形コライダー更新
	if (m_pBoxColiider)
	{
		m_pBoxColiider->SetPosOld(GetOldPos());
		m_pBoxColiider->SetPos(UpdatePos);
	}

	// マップ内の当たり判定
	const auto& BlockManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (!BlockManager) return;

	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		auto IdxBlock = BlockManager->GetBlock(nCnt);
		if (!IdxBlock) continue;

		// コライダー取得
		CBoxCollider* Collider = IdxBlock->GetCollider();
		if (!Collider) continue;

		if (Collision(Collider, &UpdatePos))
		{
			// 現在座標をセット
			SetPos(UpdatePos);

			// コライダー更新
			m_pBoxColiider->SetPos(UpdatePos);
			m_pBoxColiider->SetPosOld(UpdatePos);
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

	// 追跡状態がfalseなら
	if (!m_isStartChase)
		return;

	// デスゾーンの描画
	DrawEyeSight();
}
//========================================================
// 対象を追いかける関数 
//========================================================
void CEnemy::ChaseMoving(void)
{
#if 0
	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = m_pDestCharactor->GetPos();

	// 描画フラグがoffなら
	if (!m_pChaseIcon->GetIsDrawFlags())
		m_pChaseIcon->SetDrawFlags(true);

	// uiアイコンの座標を更新
	m_pChaseIcon->SetPos(D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z));

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;
	vecToTarget.y = 0.0f;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (CheckEyesight())
	{
		// プレイヤーを捕まえる
		m_pDestCharactor->SetCatchEnemy(true);

		// 目的地に到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::CATCH, true,3);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= m_fMoveSpeed * 1.5f;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::CHASEDASH,true,2);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);

#else
	// アイコン表示
	if (!m_pChaseIcon->GetIsDrawFlags())
		m_pChaseIcon->SetDrawFlags(true);

	D3DXVECTOR3 pos = GetPos();
	m_pChaseIcon->SetPos(D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z));

	// 捕獲判定
	if (CheckEyesight())
	{
		m_pDestCharactor->SetCatchEnemy(true);
		GetMotion()->SetMotion(MOTION::CATCH, true, 3);
		return;
	}

	// 追跡用ウェイポイント配列
	const D3DXVECTOR3* pWayPoints = NavInfo::WayPoints;
	int pointMax = NavInfo::POINT_MAX;

	// 現在目標にしているポイントの座標
	D3DXVECTOR3 targetPos = pWayPoints[m_nTargetIdx];

	D3DXVECTOR3 vecToTarget = targetPos - pos;
	vecToTarget.y = 0.0f;
	float distance = D3DXVec3Length(&vecToTarget);

	//----------------------------------------------------
	// ポイントへの到着判定
	//----------------------------------------------------
	if (distance <= EnemyInfo::RANGE)
	{
		// 座標補正
		SetPos(D3DXVECTOR3(targetPos.x, pos.y, targetPos.z));

		// 最適ポイントを探してセット
		m_nTargetIdx = GetBestWayPointToPlayer(pWayPoints, pointMax, m_nTargetIdx);
		return;
	}

	//----------------------------------------------------
	// 目標ポイントへ向かって移動
	//----------------------------------------------------
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);
	moveVec *= (m_fMoveSpeed * 1.8f);
	SetMove(moveVec);

	// モーション設定
	GetMotion()->SetMotion(MOTION::CHASEDASH, true, 2);

	// 向きの回転計算
	float angleY = atan2(-moveVec.x, -moveVec.z);
	D3DXVECTOR3 rotDest = GetRotDest();
	rotDest.y = NormalAngle(angleY);
	SetRotDest(rotDest);
#endif
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
	moveVec *= m_fMoveSpeed;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE,true,3);

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
	moveVec *= m_fMoveSpeed;
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
	vecToTarget.y = 0.0f;

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
	moveVec *= m_fMoveSpeed;
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
// 視界の扇形の描画処理 ( デスゾーン )
//========================================================
void CEnemy::DrawEyeSight(void)
{
	// 現在の設定を取得
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	float halfAngle = D3DXToRadian(m_fEyeAngle / 2.0f);

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
		float currentAngle = rot.y - halfAngle + (D3DXToRadian(m_fEyeAngle) * t);

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
// 扇形の視界判定 ( 赤い扇型の物 )
//========================================================
bool CEnemy::CheckEyesight(void)
{
	// nullなら
	if (!m_pDestCharactor) return false;

	// 敵とプレイヤーの現在座標を取得
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 CharactorPos = m_pDestCharactor->GetPos();

	// 高さ判定
	float heightDiff = fabsf(CharactorPos.y - enemyPos.y);
	if (heightDiff > Eyesight::EYE_HEIGHT / 2.0f) return false;

	// 距離判定
	D3DXVECTOR3 diff = CharactorPos - enemyPos;
	diff.y = 0.0f;
	float sqrDistance = D3DXVec3LengthSq(&diff);

	if (sqrDistance > Eyesight::EYE_RADIUS * Eyesight::EYE_RADIUS) return false;
	if (sqrDistance < 0.0001f) return true;

	// 角度判定
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 enemyForward(-sinf(rot.y), 0.0f, -cosf(rot.y));
	D3DXVec3Normalize(&enemyForward, &enemyForward);

	// 差分ベクトル
	D3DXVECTOR3 diffDir;
	D3DXVec3Normalize(&diffDir, &diff);

	// 内積判定
	float dot = D3DXVec3Dot(&enemyForward, &diffDir);
	float cosHalfAngle = cosf(D3DXToRadian(m_fEyeAngle));

	// 扇形視界に入っている場合
	return (dot >= cosHalfAngle);
}
//========================================================
// 本来の敵から見えるプレイヤーとの視界判定
//========================================================
bool CEnemy::CheckRayToAngleRange(void)
{
	// nullなら
	if (!m_pDestCharactor) return false;

	// 初期のタスク時間なら
	if (!m_pDestCharactor->GetIsInitTasking())
		return false;

	// タスク中の時間だったら
	if (m_pDestCharactor->GetIsPcWorking() || m_pDestCharactor->GetIsCopyWorking())
		return false;

	// 自身の座標とターゲットへの座標
	D3DXVECTOR3 myPos = GetPos();
	D3DXVECTOR3 targetPos = m_pDestCharactor->GetPos();

	// 高低差判定
	float heightDiff = fabsf(targetPos.y - myPos.y);
	if (heightDiff > Eyesight::EYE_HEIGHT)
	{
		return false;
	}

	// 距離制限
	D3DXVECTOR3 diff = targetPos - myPos;
	diff.y = 0.0f;

	float distance = D3DXVec3Length(&diff);
	if (distance > Eyesight::EYE_RANGE || distance <= 0.0001f)
	{
		return false; // 視界距離外
	}

	// 敵正面ベクトルを算出
	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXVECTOR3 forward(-matRot._31, 0.0f, -matRot._33);
	D3DXVec3Normalize(&forward, &forward);

	// ターゲットへのベクトルを算出
	D3DXVECTOR3 dirToTarget;
	D3DXVec3Normalize(&dirToTarget, &diff);

	// 角度と内積計算
	float halfAngleRad = D3DXToRadian(m_fEyeAngle * 0.5f);
	float thresholdDot = cosf(halfAngleRad);

	// 内積を計算
	float dot = D3DXVec3Dot(&forward, &dirToTarget);

	// 内積がしきい値より小さい
	if (dot < thresholdDot)
	{
		return false; // 視野角の外
	}

	// 障害物判定
	if (CheckObstacle())
	{
		return false; // 障害物に遮られている
	}
	
	return true;
}
//========================================================
// オブジェクト間判定
//========================================================
bool CEnemy::CheckObstacleBetween(const D3DXVECTOR3& startPos, const D3DXVECTOR3& endPos)
{
	CBlockManager* pManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (!pManager) return false;

	// 目線の高さ補正
	D3DXVECTOR3 rayStart = startPos;
	D3DXVECTOR3 rayEnd = endPos;
	rayStart.y += 20.0f;
	rayEnd.y += 20.0f;

	// ワールド空間でのレイのベクトルと距離
	D3DXVECTOR3 rayVec = rayEnd - rayStart;
	float maxDistance = D3DXVec3Length(&rayVec);
	if (maxDistance <= 0.0001f) return false;

	int blockCount = pManager->GetAll();
	for (int nCnt = 0; nCnt < blockCount; nCnt++)
	{
		CBlock* pBlock = pManager->GetBlock(nCnt);
		if (!pBlock) continue;

		//----------------------------------------------------
		// ブロックの変換行列（位置・回転）を取得して逆行列を作成
		//----------------------------------------------------
		D3DXVECTOR3 bPos = pBlock->GetPos();
		D3DXVECTOR3 bRot = pBlock->GetRot();

		D3DXMATRIX matWorld, matInvWorld;
		D3DXMATRIX matTrans, matRot;

		D3DXMatrixTranslation(&matTrans, bPos.x, bPos.y, bPos.z);
		D3DXMatrixRotationYawPitchRoll(&matRot, bRot.y, bRot.x, bRot.z);
		matWorld = matRot * matTrans; 

		// 逆行列を作成
		D3DXMatrixInverse(&matInvWorld, nullptr, &matWorld);

		//----------------------------------------------------
		// レイをブロックのローカル空間に変換
		//----------------------------------------------------
		D3DXVECTOR3 localStart, localEnd;
		D3DXVec3TransformCoord(&localStart, &rayStart, &matInvWorld);
		D3DXVec3TransformCoord(&localEnd, &rayEnd, &matInvWorld);

		D3DXVECTOR3 localRayDir = localEnd - localStart;
		float localMaxDist = D3DXVec3Length(&localRayDir);
		if (localMaxDist <= 0.0001f) continue;

		D3DXVec3Normalize(&localRayDir, &localRayDir);

		// ローカル空間での箱のサイズ
		D3DXVECTOR3 bScale = pBlock->GetScale();
		D3DXVECTOR3 minBound = -bScale * 0.5f;
		D3DXVECTOR3 maxBound = bScale * 0.5f;

		//----------------------------------------------------
		// ローカル空間でスラブ法（レイ vs AABB）判定
		//----------------------------------------------------
		float startArr[3] = { localStart.x, localStart.y, localStart.z };
		float dirArr[3] = { localRayDir.x, localRayDir.y, localRayDir.z };
		float minArr[3] = { minBound.x, minBound.y, minBound.z };
		float maxArr[3] = { maxBound.x, maxBound.y, maxBound.z };

		float tMin = 0.0f;
		float tMax = localMaxDist;
		bool hit = true;

		for (int axis = 0; axis < 3; ++axis)
		{
			float origin = startArr[axis];
			float dir = dirArr[axis];
			float bMin = minArr[axis];
			float bMax = maxArr[axis];

			if (fabsf(dir) < 0.00001f)
			{
				if (origin < bMin || origin > bMax)
				{
					hit = false;
					break;
				}
			}
			else
			{
				float invD = 1.0f / dir;
				float t1 = (bMin - origin) * invD;
				float t2 = (bMax - origin) * invD;

				if (t1 > t2) std::swap(t1, t2);

				tMin = (t1 > tMin) ? t1 : tMin;
				tMax = (t2 < tMax) ? t2 : tMax;

				if (tMin > tMax)
				{
					hit = false;
					break;
				}
			}
		}

		if (hit)
		{
			return true; // 回転したブロックに衝突している
		}
	}

	return false; // 障害物なし
}
//========================================================
// 判別する際に障害物が存在しているかどうか
//========================================================
bool CEnemy::CheckObstacle(void)
{
	if (!m_pDestCharactor) return false;

	// 関数で判別結果を見る
	return CheckObstacleBetween(GetPos(), m_pDestCharactor->GetPos());
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
//========================================================
// 敵のレベル加算関数
//========================================================
void CEnemy::AddLevel(const float fValue)
{
	// 加算
	m_fLevelPoint += fValue;

	// 最大で格納できるポイントを計算(最大1000.0fまで)
	float maxPoints = LevelConfig::MAX_LEVEL * LevelConfig::LEVELUP_NEED_POINT;

	// 値の設定
	if (m_fLevelPoint < 0.0f)
	{
		m_fLevelPoint = 0.0f;
	}
	else if (m_fLevelPoint > maxPoints)
	{
		m_fLevelPoint = maxPoints;
	}

	// 新規のレベル計算
	int newLevel = static_cast<int>(m_fLevelPoint / LevelConfig::LEVELUP_NEED_POINT);

	// 最大レベル制限
	if (newLevel >= LevelConfig::MAX_LEVEL_POINT)
	{
		newLevel = LevelConfig::MAX_LEVEL_POINT;
	}

	// レベルが変わったらパラメータを更新
	if (m_nLevel != newLevel)
	{
		m_nLevel = newLevel;
		UpdateLevelParameters();
	}
}
//========================================================
// レベルダウン設定関数
//========================================================
void CEnemy::LevelDown(const float fValue)
{
	// 値の減算
	AddLevel(-fValue);
}
//========================================================
// レベルダウンイベント関数
//========================================================
void CEnemy::LevelDownEvent(void)
{
	// nullチェック
	if (!m_pGauge) return;

	// 4割くらいの減少
	m_pGauge->SetRatioTypeEvent(0.35f);
}
//========================================================
// パラメータ更新関数
//========================================================
void CEnemy::UpdateLevelParameters(void)
{
	// 速度と角度の設定
	SetMoveSpeed();
	SetEyeAngle();
}
//========================================================
// 敵のレベルによる速度設定
//========================================================
void CEnemy::SetMoveSpeed(void)
{
	// 割合を計算する
	float rate = static_cast<float>(m_nLevel) / static_cast<float>(LevelConfig::MAX_LEVEL_POINT);

	// 線形補完計算
	m_fMoveSpeed = Lerp(LevelConfig::MIN_SPEED, LevelConfig::MAX_SPEED, rate);
}
//========================================================
// 敵のレベルによる視界の範囲設定
//========================================================
void CEnemy::SetEyeAngle(void)
{
	// 割合計算
	float rate = static_cast<float>(m_nLevel) / static_cast<float>(LevelConfig::MAX_LEVEL_POINT);

	// 線形補完計算
	m_fEyeAngle = Lerp(LevelConfig::MIN_EYE_ANGLE, LevelConfig::MAX_EYE_ANGLE, rate);
}
//=========================================================
// 自身から最も近いウェイポイントのインデックスを取得 ( 変更案件 )
//=========================================================
int CEnemy::GetNearestWayPointIndex(const D3DXVECTOR3* pPoints, int maxCount, const D3DXVECTOR3& targetPos)
{
	if (!pPoints || maxCount <= 0) return 0;

	float minDistanceSq = 99999999.0f;
	int nearestIdx = 0;

	for (int i = 0; i < maxCount; ++i)
	{
		D3DXVECTOR3 diff = pPoints[i] - targetPos;
		diff.y = 0.0f;

		float distSq = D3DXVec3LengthSq(&diff);
		if (distSq < minDistanceSq)
		{
			minDistanceSq = distSq;
			nearestIdx = i;
		}
	}

	return nearestIdx;
}
//========================================================
// 一番近いインデックスを自動セットする
//========================================================
void CEnemy::ResetTargetIdxToNearestByMoveType(void)
{
	const D3DXVECTOR3* pPoints = nullptr;
	int pointMax = 0;

	// MoveTypeに応じて参照する配列と要素数を切り替える
	switch (GetMoveType())
	{
	case MOVETYPE_NORMAL:
		pPoints = EnemyInfo::ViewPoint;
		pointMax = EnemyInfo::VIEWPOINT;
		break;

	case MOVETYPE_SMOKE:
		pPoints = SMOKE_AND_MAGAZINE::ViewPoint;
		pointMax = SMOKE_AND_MAGAZINE::ALL_POINT;
		break;

	case MOVETYPE_TV:
		pPoints = TV_AND_EATING::ViewPoint;
		pointMax = TV_AND_EATING::ALL_POINT;
		break;

	default:
		pPoints = EnemyInfo::ViewPoint;
		pointMax = EnemyInfo::VIEWPOINT;
		break;
	}

	// 一番近いインデックスを取得してセット
	int nearestIdx = GetNearestWayPointIndex(pPoints, pointMax,GetPos());
	SetTargetIdx(nearestIdx);
}
//========================================================
// 障害物を回避しつつ、プレイヤーに最も近いポイントを探す ( 変更案件 )
//========================================================
int CEnemy::GetBestWayPointToPlayer(const D3DXVECTOR3* pPoints, int maxCount, int currentIdx)
{
	if (!pPoints || maxCount <= 0 || !m_pDestCharactor) return 0;

	D3DXVECTOR3 myPos = GetPos();
	D3DXVECTOR3 playerPos = m_pDestCharactor->GetPos();

	// 1. 【プレイヤーに一番近いポイント（最終目的地）を取得】
	int playerTargetIdx = GetNearestWayPointIndex(pPoints, maxCount, playerPos);
	D3DXVECTOR3 playerTargetPos = pPoints[playerTargetIdx];

	// 敵から「プレイヤーの最終ポイント」までの距離
	D3DXVECTOR3 diffToPlayerPt = playerTargetPos - myPos;
	diffToPlayerPt.y = 0.0f;
	float distToPlayerPtSq = D3DXVec3LengthSq(&diffToPlayerPt);

	// ★修正: 到達判定の閾値（ EnemyInfo::RANGE 等の実際の到着半径（例: 20.0f）に合わせる）
	float arrivalThresholdSq = EnemyInfo::RANGE * EnemyInfo::RANGE;

	// 敵がすでにプレイヤーの最終ポイントに到達しているなら、そのポイントに留まる
	if (distToPlayerPtSq <= arrivalThresholdSq)
	{
		return playerTargetIdx;
	}

	// 自身の球形コライダーの半径（周囲を探す範囲）
	float searchRadiusSq = 1000.0f * 1000.0f;

	float bestDistToFinalSq = 99999999.0f; // 最終地点までの残り距離
	float bestEnemyDistSq = 99999999.0f;   // 敵からそのポイントまでの距離
	int bestIdx = -1;

	// 2. 自分の周囲のポイントから、最終地点（playerTargetPos）に一番近づくポイントを選択
	for (int i = 0; i < maxCount; ++i)
	{
		// 今到着したポイント（現在地）は絶対除外
		if (i == currentIdx) continue;

		D3DXVECTOR3 ptPos = pPoints[i];

		// 敵からの距離チェック（コライダー範囲内か）
		D3DXVECTOR3 diffE = ptPos - myPos;
		diffE.y = 0.0f;
		float distToEnemySq = D3DXVec3LengthSq(&diffE);

		if (distToEnemySq > searchRadiusSq)
		{
			continue; // コライダー範囲外は無視
		}

		// そのポイントから「プレイヤーの最終ポイント」までの残り距離
		D3DXVECTOR3 diffFinal = playerTargetPos - ptPos;
		diffFinal.y = 0.0f;
		float distToFinalSq = D3DXVec3LengthSq(&diffFinal);

		// ★現在の自分の位置よりプレイヤーから遠ざかるポイントは除外する
		if (distToFinalSq >= distToPlayerPtSq)
		{
			continue;
		}

		// 最終地点に一番近づくポイントを採用
		if (distToFinalSq < bestDistToFinalSq - 0.01f)
		{
			bestDistToFinalSq = distToFinalSq;
			bestEnemyDistSq = distToEnemySq;
			bestIdx = i;
		}
		// 距離が同等なら、より敵から近いポイントを優先
		else if (fabsf(distToFinalSq - bestDistToFinalSq) <= 0.01f)
		{
			if (distToEnemySq < bestEnemyDistSq)
			{
				bestDistToFinalSq = distToFinalSq;
				bestEnemyDistSq = distToEnemySq;
				bestIdx = i;
			}
		}
	}

	// 3. 【フォールバック】前進できるポイントが見つからない場合
	if (bestIdx == -1)
	{
		// コライダー内で、とにかく一番プレイヤーの最終ポイントに近いポイントを選ぶ
		for (int i = 0; i < maxCount; ++i)
		{
			if (i == currentIdx) continue;

			D3DXVECTOR3 ptPos = pPoints[i];
			D3DXVECTOR3 diffFinal = playerTargetPos - ptPos;
			diffFinal.y = 0.0f;
			float distToFinalSq = D3DXVec3LengthSq(&diffFinal);

			if (distToFinalSq < bestDistToFinalSq)
			{
				bestDistToFinalSq = distToFinalSq;
				bestIdx = i;
			}
		}
	}

	return (bestIdx != -1) ? bestIdx : playerTargetIdx;
}