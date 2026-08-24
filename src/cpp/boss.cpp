//========================================================
//
// 社長処理 [ boss.cpp ]
// Author: Asuma Nishio
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "boss.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "boxcollider.h"
#include "spherecollider.h"
#include "template.h"
#include "billboard.h"
#include "statemachine.h"
#include "player.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace BOSS_INFO
{
	constexpr int NUM_OUTSIDEPOINT = 6;

	// ローカルビューポイント
	const D3DXVECTOR3 LocalMovePoint[NUM_OUTSIDEPOINT] =
	{
		{677.5f,0.0f,313.0f},
		{663.0f,0.0f,115.0f},
		{420.5f,0.0f,115.0f},
		{400.0f,0.0f,190.0f},
		{134.0f,0.0f,190.0f},
		{130.0f,0.0f,310.0f},
	};
};

//*********************************************************
// 定数名前空間
//*********************************************************
namespace OFFICE_INFO
{
	constexpr int COOLTIME = 120;
	constexpr int NUM_OFFICEPOINT = 18;

	// ローカルビューポイント
	const D3DXVECTOR3 OfficeMovePoint[NUM_OFFICEPOINT] =
	{
		{-50.0f,0.0f,323.0f},
		{-83.0f,0.0f,240.0f},
		{-125.0f,0.0f,90.0f},
		{-227.0f,0.0f,69.0f},
		{-217.0f,0.0f,-140.0f},
		{85.0f,0.0f,-153.0f},
		{90.0f,0.0f,36.0f},
		{174.0f,0.0f,33.0f},
		{168.0f,0.0f,-187.0f},
		{174.0f,0.0f,33.0f},
		{405.0f,0.0f,23.0f},
		{413.0f,0.0f,183.0f},
		{264.0f,0.0f,200.0f},
		{260.0f,0.0f,358.0f},
		{264.0f,0.0f,200.0f},
		{169.0f,0.0f,191.0f},
		{-83.0f,0.0f,240.0f},
		{-50.0f,0.0f,323.0f},
	};
};

//========================================================
// コンストラクタ
//========================================================
CBoss::CBoss(int nPriority) : CMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_nViewIdx(NULL),
m_nOfficeViewIdx(NULL),
m_nCoolTime(NULL),
m_isOutSideIn(true),
m_isOfficeMove(false),
m_pChaseIcon(nullptr),
m_pDestCharactor(nullptr),
m_playerTargetPos(VECTOR3_NULL),
m_pMachine(nullptr)
{

}
//========================================================
// デストラクタ
//========================================================
CBoss::~CBoss()
{

}
//========================================================
// 生成処理
//========================================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CBoss* pBoss = new CBoss;
	if (pBoss == nullptr) return nullptr;

	// オブジェクト設定
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);
	pBoss->SetUseOutLine(true);
	pBoss->SetOutLineSize(0.4f);

	// 初期化設定時
	if (FAILED(pBoss->Init())) return nullptr;

	return pBoss;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CBoss::Init(void)
{
	// キャラクター初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad("data/MOTION/Boss/BossMotion.txt", MOTION::MAX, false);

	// アイコン生成
	m_pChaseIcon = CBillboard::Create(GetPos(), VECTOR3_NULL, 20.0f, 20.0f, "ui_chaseicon.png");
	m_pChaseIcon->SetDrawFlags(false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CBoss::Uninit(void)
{
	// キャラクター終了
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CBoss::Update(void)
{
	// 現在地の座標を取得
	auto pos = GetPos();

	if (m_isOutSideIn)
		MoveInOffice(pos); // オフィス内に来る

	if (m_isOfficeMove)
		MoveOfficePoint(pos); // オフィス内で巡回する処理

	// キャラクター座標更新
	CMoveCharactor::UpdatePosition();

	// 更新された座標を取得
	auto UpdatePos = GetPos();

	// 親キャラクター更新
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CBoss::Draw(void)
{
	// キャラクター描画
	CMoveCharactor::Draw();

	if (!m_isOfficeMove) return;

	// TODO : ボスに捕まるかどうかを決める
}
//========================================================
// 扇形の描画処理
//========================================================
void CBoss::DrawEyeSight(void)
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
	vFan[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	vFan[0].tex = VECTOR2_NULL;

	// 外周点
	for (int nCnt = 0; nCnt <= Config::DIVIDE; ++nCnt)
	{
		// 角度計算
		float t = (float)nCnt / (float)Config::DIVIDE;
		float currentAngle = rot.y - halfAngle + (D3DXToRadian(Eyesight::EYE_ANGLE) * t);

		D3DXVECTOR3 dir(-sinf(currentAngle), 0.0f, -cosf(currentAngle));

		int idx = nCnt + 1;
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

	// テクスチャを戻す
	pDevice->SetTexture(0, pOldTexture);
	if (pOldTexture) pOldTexture->Release();

	// テクスチャステージチャートの再設定
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, oldColorArg2);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, oldAlphaArg2);
}
//========================================================
// 視界との当たり判定処理
//========================================================
bool CBoss::CheckEyesight(const D3DXVECTOR3& TargetPos)
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
//========================================================
// オフィス内に侵入する処理
//========================================================
void CBoss::MoveInOffice(const D3DXVECTOR3& pos)
{
	// もし最後のインデックスだったら
	if (m_nViewIdx >= BOSS_INFO::NUM_OUTSIDEPOINT)
	{
		m_isOutSideIn = false;
		m_isOfficeMove = true;
		return;
	}

	// ターゲットの座標
	D3DXVECTOR3 targetPos = BOSS_INFO::LocalMovePoint[m_nViewIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= 2.0f)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// インデックス設定
		m_nViewIdx++;

		// モーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 2.0f;
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
// オフィス内で巡回をする処理
//========================================================
void CBoss::MoveOfficePoint(const D3DXVECTOR3& pos)
{
	// 外から移動の時はreturn
	if (!m_isOutSideIn) return;

	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		// 減算
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ターゲットの座標
	D3DXVECTOR3 targetPos = OFFICE_INFO::OfficeMovePoint[m_nOfficeViewIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= 2.0f)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// クールタイム開始
		m_nCoolTime = OFFICE_INFO::COOLTIME;

		// インデックス設定
		m_nOfficeViewIdx = Wrap(m_nOfficeViewIdx + 1, 0, OFFICE_INFO::NUM_OFFICEPOINT - 1);

		// モーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 2.0f;
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