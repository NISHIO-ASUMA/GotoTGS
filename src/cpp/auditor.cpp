//========================================================
//
// 外の監査役の処理 [ auditor.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditor.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "spherecollider.h"
#include "manager.h"
#include "template.h"
#include "blockmanager.h"
#include "block.h"
#include "jsonmanager.h"
#include "billboard.h"
#include "statemachine.h"
#include "player.h"

#include "auditorstatebase.h"
#include "auditorstateneutral.h"
#include "auditorutility.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace AUDITOR_INFO
{
	constexpr const char* FILENAME = "data/MOTION/Auditor/AuditorMotion.txt"; // モーションファイル
	constexpr float RANGE = 2.0f;											  // 距離判定
};

//*********************************************************
// 使用する名前空間
//*********************************************************
using namespace AuditorUtility;					// ポイントデータの配列情報の格納先の名前空間

//========================================================
// コンストラクタ
//========================================================
CAuditor::CAuditor(int nPriority) : CMoveCharactor(nPriority),
m_nCoolTime(NULL),
m_nOfficeViewIdx(NULL),
m_nViewIdx(NULL),
m_nTargetIdx(NULL),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_pDestCharactor(nullptr),
m_pMachine(nullptr),
m_pChaseIcon(nullptr),
m_isStartChase(false),
m_isTargetChase(false),
m_MoveTypeData(MOVE_POINTTYPE::OFFICENEAR)
{
}
//========================================================
// デストラクタ
//========================================================
CAuditor::~CAuditor()
{

}
//========================================================
// 生成処理
//========================================================
CAuditor* CAuditor::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const MOVE_POINTTYPE& type)
{
	// インスタンス生成
	CAuditor* pAuditor = new CAuditor;
	if (pAuditor == nullptr) return nullptr;

	// オブジェクト設定
	pAuditor->SetPos(pos);
	pAuditor->SetRot(rot);
	pAuditor->SetUseOutLine(true);
	pAuditor->SetOutlineColor();
	pAuditor->m_MoveTypeData = type;

	// 初期化失敗時
	if (FAILED(pAuditor->Init())) return nullptr;

	return pAuditor;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CAuditor::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad(AUDITOR_INFO::FILENAME, MOTION::MAX, false);

	// ステート生成
	m_pMachine = new CStateMachine;
	if (m_pMachine)
		this->ChangeState(new CAuditorStateNeutral(),CAuditorStateBase::ID_NEUTRAL);

	// アイコン生成
	m_pChaseIcon = CBillboard::Create(GetPos(), VECTOR3_NULL, 20.0f, 20.0f, "ui_chaseicon.png");
	m_pChaseIcon->SetDrawFlags(false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CAuditor::Uninit(void)
{
	// コライダー破棄
	m_pBoxColiider.reset();
	m_pSphereColiider.reset();

	// ステートの破棄
	if (m_pMachine)
	{
		m_pMachine->OnExit();
		delete m_pMachine;
		m_pMachine = nullptr;
	}

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CAuditor::Update(void)
{
	// ステートの更新処理
	if (m_pMachine)
		m_pMachine->Update();

	// 座標の更新
	CMoveCharactor::UpdatePosition();

	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CAuditor::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();

	// 追跡状態がfalseなら
	if (!m_isStartChase)
		return;

	// 扇形の描画
	DrawEyeSight();
}
//========================================================
// ポイントごとの周回起点
//========================================================
void CAuditor::MovingTypeOutSide(void)
{
	switch (m_MoveTypeData)
	{
	case CAuditor::OFFICENEAR:	// オフィス近く
		UpdateOffice();
		break;
	case CAuditor::GAMECENTER:	// ゲーセン付近
		UpdateGameCenter();
		break;
	case CAuditor::SOBAANDBAR:	// 蕎麦屋
		UpdateSoba();
		break;
	case CAuditor::MAPLEFT:		// マップ左側
		UpdateMapLeft();
		break;
	default:
		break;
	}
}
//========================================================
// 追いかける移動処理
//========================================================
void CAuditor::ChaseMove(void)
{
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
		GetMotion()->SetMotion(MOTION::CATCH, true, 3);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 3.5f;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::CHASEDASH, true, 2);

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
// オフィス周りの更新
//========================================================
void CAuditor::UpdateOffice(void)
{
	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = OfficePoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= AUDITOR_INFO::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nCoolTime = 60;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, OFFICE_POINT - 1);

		// 目的地に到着したらモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL,true,5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.5f;
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
// 蕎麦屋付近の更新
//========================================================
void CAuditor::UpdateSoba(void)
{	
	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		// デクリメント
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = SobaPoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= AUDITOR_INFO::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nCoolTime = 60;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, SOBA_POINT - 1);

		// 目的地に到着したらモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.5f;
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
// ゲームセンターの更新
//========================================================
void CAuditor::UpdateGameCenter(void)
{
	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = GameCenterPoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= AUDITOR_INFO::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nCoolTime = 60;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, GAME_POINT - 1);

		// 目的地に到着したらモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.5f;
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
// 左サイドの動き更新
//========================================================
void CAuditor::UpdateMapLeft(void)
{
	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// 現在の座標とターゲットの座標を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 targetPos = LeftSidePoint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= AUDITOR_INFO::RANGE)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nCoolTime = 60;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, LEFTSIDE_POINT - 1);

		// 目的地に到着したらモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.5f;
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
// 扇形判定の描画
//========================================================
void CAuditor::DrawEyeSight(void)
{
	// 現在の設定を取得
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	float halfAngle = D3DXToRadian(CAuditor::Eyesight::EYE_ANGLE / 2.0f);

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
	vFan[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
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
// ステート切り替え設定
//========================================================
void CAuditor::ChangeState(CAuditorStateBase* pNewState, int nID)
{
	// 自分自身のポインタを設定
	pNewState->SetOwner(this);

	// IDの設定
	pNewState->SetID(nID);

	// 実際の切り替え
	m_pMachine->ChangeState(pNewState);
}
//========================================================
// 障害物判定
//========================================================
bool CAuditor::CheckObstacle(void)
{
	// ブロック管理クラスの取得
	CBlockManager* pManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (!pManager) return false;

	// 始点と終点
	D3DXVECTOR3 rayStart = GetPos();
	D3DXVECTOR3 rayEnd = m_pDestCharactor->GetPos();

	// 目線の高さ補正
	rayStart.y += 20.0f;
	rayEnd.y += 20.0f;

	// レイの方向ベクトルと距離を計算
	D3DXVECTOR3 rayDir = rayEnd - rayStart;
	float maxDistance = D3DXVec3Length(&rayDir);
	if (maxDistance <= 0.0001f) return false;

	// 正規化
	D3DXVec3Normalize(&rayDir, &rayDir);

	// 登録されている全ブロックとの衝突確認
	int blockCount = pManager->GetAll();
	for (int nCnt = 0; nCnt < blockCount; nCnt++)
	{
		// 単ブロックを取得
		CBlock* pBlock = pManager->GetBlock(nCnt);
		if (!pBlock) continue;

		// CBlockからコライダーを取得
		auto* pBoxCollider = pBlock->GetCollider();
		if (!pBoxCollider) continue;

		// ブロックのAABB座標範囲を取得
		D3DXVECTOR3 bPos = pBlock->GetPos();
		D3DXVECTOR3 bScale = pBlock->GetScale();

		D3DXVECTOR3 minBound = bPos - (bScale * 0.5f);
		D3DXVECTOR3 maxBound = bPos + (bScale * 0.5f);

		// 線分の交差判定
		float tMin = 0.0f;
		float tMax = maxDistance;

		bool hit = true;

		for (int axis = 0; axis < 3; ++axis)
		{
			float origin = ((float*)&rayStart)[axis];
			float dir = ((float*)&rayDir)[axis];
			float bMin = ((float*)&minBound)[axis];
			float bMax = ((float*)&maxBound)[axis];

			if (fabsf(dir) < 0.00001f)
			{
				// レイがこの軸と平行な場合、軸上の範囲外にあれば衝突しない
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

		// 障害物が存在する
		if (hit)
		{
			return true; // 障害物あり
		}
	}

	return false; // 障害物なし
}
//========================================================
// 実際の視界との距離判定
//========================================================
bool CAuditor::CheckRayToAngleRange(void)
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
	if (distance > 300.0f|| distance <= 0.0001f)
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
	float halfAngleRad = D3DXToRadian(70.0f * 0.5f);
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
// 視界との判定
//========================================================
bool CAuditor::CheckEyesight(void)
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
	float cosHalfAngle = cosf(D3DXToRadian(70.0f));

	// 扇形視界に入っている場合
	return (dot >= cosHalfAngle);
}