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

//*********************************************************
// 定数名前空間
//*********************************************************
namespace AUDITOR_INFO
{
	constexpr const char* FILENAME = "data/MOTION/Auditor/AuditorMotion.txt"; // モーションファイル
};

//*********************************************************
// ポイントを動く定数名前空間 ( 後々開地君次第で変更する )
//*********************************************************
namespace MOVE_VIEWPOINT
{
	constexpr int MAX_POINT = 1;

	// 巡回ポイント
	const D3DXVECTOR3 LocalPoint[MAX_POINT] =
	{
		{600.0f,0.0f,400.0f}
	};
};

//========================================================
// コンストラクタ
//========================================================
CAuditor::CAuditor(int nPriority) : CMoveCharactor(nPriority),
m_nCoolTime(NULL),
m_nOfficeViewIdx(NULL),
m_nViewIdx(NULL),
m_nTargetIdx(NULL),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr)
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
CAuditor* CAuditor::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CAuditor* pAuditor = new CAuditor;
	if (pAuditor == nullptr) return nullptr;

	// オブジェクト設定
	pAuditor->SetPos(pos);
	pAuditor->SetRot(rot);
	pAuditor->SetUseOutLine(true);
	pAuditor->SetOutlineColor();

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

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CAuditor::Update(void)
{
	// 座標取得
	auto pos = GetPos();

	// ポイント間座標の更新
	MovePointOutSide(pos);

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

	// 扇形の描画
	DrawEyeSight();
}
//========================================================
// ポイント間を動く処理 ( 巡回ポイントが決定次第 )
//========================================================
void CAuditor::MovePointOutSide(const D3DXVECTOR3& pos)
{
#if 0
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
	D3DXVECTOR3 targetPos = MOVE_VIEWPOINT::LocalPopint[m_nTargetIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= 2.0f)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// 停止時間を設定
		m_nStopTime = 60;

		// インデックス設定
		m_nTargetIdx = Wrap(m_nTargetIdx + 1, 0, MOVE_VIEWPOINT::MAX_POINT - 1);

		// 目的地に到着した瞬間にモーションを切り替える
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
#endif
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
// 視界との判定
//========================================================
bool CAuditor::CheckEyesight(const D3DXVECTOR3& TargetPos)
{
	// 現在座標を取得
	D3DXVECTOR3 MyPos = GetPos();

	// 高さの判定
	float heightDiff = fabsf(TargetPos.y - MyPos.y);

	if (heightDiff > Eyesight::EYE_HEIGHT / 2.0f)
	{
		return false; // 高さが範囲外
	}

	// 距離の判定
	D3DXVECTOR3 diff = TargetPos - MyPos;
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