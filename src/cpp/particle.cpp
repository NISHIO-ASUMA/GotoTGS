//=========================================================
//
// パーティクル処理[particle.cpp]
// Author : Kaiti Aoto
//
//=========================================================

//**********************************************************
// クラス定義ヘッダーファイル
//**********************************************************
#include "particle.h"

//**********************************************************
// インクルードファイル
//**********************************************************
#include "manager.h"
#include "renderer.h"
#include "effect.h"

//**********************************************************
// 静的メンバ変数
//**********************************************************
D3DXVECTOR3 CParticle::m_TargetPos = {};

namespace EFFECT
{
	constexpr int nMax = 200;		// ランダム最大値
	constexpr int nMin = 100;		// ランダム最小値
}

//=========================================================
// コンストラクタ
//=========================================================
CParticle::CParticle() : m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_col(V_COLOR_WHITE),
m_nLife(NULL),
m_nCreateTime(NULL),
m_fRadius(NULL),
m_bUse(false)
{
	m_TargetPos = VECTOR3_NULL;
}
//=========================================================
// デストラクタ
//=========================================================
CParticle::~CParticle()
{
}
//=========================================================
// 生成処理
//=========================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fRadius, TYPE type)
{
	CParticle* pParticle = new CParticle;

	if (pParticle == nullptr) return nullptr;

	//初期化
	pParticle->SetPos(pos);
	pParticle->SetRot(rot);
	pParticle->SetColor(col);
	pParticle->SetLife(nLife);
	pParticle->SetRadius(fRadius);
	pParticle->SetType(type);

	// 初期化失敗時
	if (FAILED(pParticle->Init())) return nullptr;

	return pParticle;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CParticle::Init(void)
{
	m_bUse = true;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CParticle::Uninit(void)
{
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CParticle::Update(void)
{
	if (!m_bUse)
	{
		Uninit();
		return;
	}
	
	switch (m_type)
	{
	case TYPE_NONE:
		None();
		break;
	case TYPE_SMOKE:
		Smoke();
		break;
	case TYPE_SPRAY:
		Spray();
		break;
	case TYPE_FLAME:
		Flame();
		break;
	case TYPE_THUNDER:
		Thunder();
		break;

	default:
		break;
	}

	m_nLife --;
	if (m_nLife <= 0)
	{
		m_bUse = false;
	}
}
//=========================================================
// 描画処理
//=========================================================
void CParticle::Draw(void)
{
}
//=========================================================
// 通常
//=========================================================
void CParticle::None(void)
{
	int nCntAppear;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;

	for (nCntAppear = 0; nCntAppear < 5; nCntAppear++)
	{
		// 今の位置を設定
		pos = m_pos;

		float theta = D3DXToRadian(rand() % 360);				// 方位角
		float phi = D3DXToRadian(rand() % 180);					// 仰角
		float fLength = (float)(rand() % 20) / 10.0f + 1.5f;

		move.x = sinf(phi) * cosf(theta) * fLength;
		move.y = cosf(phi) * fLength;
		move.z = sinf(phi) * sinf(theta) * fLength;

		// 色・サイズ・寿命の設定
		col = m_col;
		fRadius = m_fRadius;
		nLife = 30;

		// エフェクトの生成
		CEffect::Create(pos, m_rot, move, col, nLife, fRadius, CEffect::TYPE_NONE);
	}
}
//=========================================================
// 舞い上がる煙
//=========================================================
void CParticle::Smoke(void)
{
	// ローカル変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	
	// 今の位置を保存
	pos = m_pos;

	float xRand = ((float)(rand() % EFFECT::nMax) - (EFFECT::nMin + 75)) / 75.0f;
	float zRand = ((float)(rand() % EFFECT::nMax) - (EFFECT::nMin + 75)) / 75.0f;
	float ySpeed = ((float)(rand() % 50) / EFFECT::nMin) + 1.0f;

	move = D3DXVECTOR3(xRand, ySpeed, zRand);

	// 色・サイズ・寿命の設定
	col = m_col;
	float fRadius = m_fRadius;
	int nLife = 40 + rand() % 20;

	m_nCreateTime--;
	if (m_nCreateTime < 0)
	{
		// エフェクトの生成
		CEffect::Create(pos, m_rot, move, col, nLife, fRadius, CEffect::TYPE_NONE);
		m_nCreateTime = 30;
	}
}
//=========================================================
// スプレー
//=========================================================
void CParticle::Spray(void)
{
	// ローカル変数に位置と色を設定
	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col = m_col;

	// 角度の設定
	float angle = m_rot.y;

	for (int i = 0; i < 36; i++)
	{
		float spread = D3DXToRadian((rand() % EFFECT::nMax - EFFECT::nMin) / 10.0f);
		float dir = angle + spread;

		float speed = 3.0f + (rand() % 150) / 50.0f;

		move.x = -sinf(dir) * speed;
		move.y = ((rand() % 100) - 50) / 200.0f;
		move.z = -cosf(dir) * speed;

		// 粒子の半径
		float fRadius = m_fRadius * 0.8f + ((rand() % 10) / 30.0f);

		// 寿命短め
		int nLife = 12 + rand() % 5;

		// エフェクトの生成
		CEffect::Create(pos, m_rot, move, col, nLife, fRadius, CEffect::TYPE_NONE);
	}
}
//=========================================================
// 火炎放射
//=========================================================
void CParticle::Flame(void)
{
	// ローカル変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 位置を設定
	pos = m_pos;

	float xRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 55.0f;
	float zRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 55.0f;
	float yRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 55.0f;

	move = D3DXVECTOR3(xRand, yRand, zRand);
	
	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toTarget = m_TargetPos - GetPos();
	// 正規化
	D3DXVec3Normalize(&toTarget, &toTarget);

	//移動量
	move.x += toTarget.x * 15.0f;
	move.y += toTarget.y * 15.0f;
	move.z += toTarget.z * 15.0f;

	// 色・サイズ・寿命の設定
	col = m_col;
	float fRadius = m_fRadius;
	int nLife = 660 + rand() % 20;

	// エフェクトの生成
	CEffect::Create(pos, m_rot, move, col, nLife, fRadius, CEffect::TYPE_NONE);
}
//=========================================================
// 落雷
//=========================================================
void CParticle::Thunder(void)
{
	// ローカル変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 位置の設定
	pos = m_pos;

	float xRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 100.0f;
	float zRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 100.0f;
	float yRand = ((float)(rand() % EFFECT::nMax) - EFFECT::nMin) / 100.0f;

	move = D3DXVECTOR3(xRand, yRand, zRand);

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toTarget = m_TargetPos - GetPos();
	// 正規化
	D3DXVec3Normalize(&toTarget, &toTarget);

	const float fSpeed = 50.0f;

	//移動量
	move.x += toTarget.x * fSpeed;
	move.y += toTarget.y * fSpeed;
	move.z += toTarget.z * fSpeed;

	// 色・サイズ・寿命の設定
	col = m_col;
	float fRadius = m_fRadius;
	int nLife = 10 + rand() % 2;

	CEffect::Create(pos, m_rot, move, col, nLife, fRadius, CEffect::TYPE_THUNDER);

	for (int i = 0; i < 3; i++)
	{
		D3DXVECTOR3 MiniMove = toTarget;

		// 方向を少しずらす
		MiniMove.x += ((rand() % EFFECT::nMax) - EFFECT::nMin) / 500.0f;
		MiniMove.z += ((rand() % EFFECT::nMax) - EFFECT::nMin) / 500.0f;
		MiniMove.y += ((rand() % EFFECT::nMax) - EFFECT::nMin) / 500.0f;

		D3DXVec3Normalize(&move, &move);

		float br = m_fRadius * 0.3f;
		int nMiniLife = 5 + rand() % 2;

		 // エフェクトの生成
		CEffect::Create(pos, m_rot, MiniMove * 35.0f, col, nMiniLife, br, CEffect::TYPE_THUNDER);
	}
}