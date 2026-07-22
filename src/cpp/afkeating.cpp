//=========================================================
//
// AFKEating処理 [ afkeating.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkeating.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間(飲食スペースでのさぼり)
//*********************************************************
namespace AFKEATING
{
	const D3DXVECTOR3 Pos = { -195.0f, 14.0, -202.0f };	// 位置
	constexpr float fRadius = 5.0f;						// 範囲の半径
};
//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { -225.0f, 38.0, -205.0f };	// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 0.5f, 0.5f };	// カラー
	constexpr float fRadius = 10.0f;					// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkEating::CAfkEating() : m_pParticle(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfkEating::~CAfkEating()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfkEating* CAfkEating::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfkEating* pAfkEating = new CAfkEating;
	if (pAfkEating == nullptr) return nullptr;

	// オブジェクトセット
	pAfkEating->SetPos(AFKEATING::Pos);

	// 初期化失敗時
	if (FAILED(pAfkEating->Init())) return nullptr;

	// ポインタを返す
	return pAfkEating;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkEating::Init(void)
{
	// 親クラスの初期化処理
	CAfk::Init(AFKEATING::Pos, AFKEATING::fRadius);

	// パーティクル生成
	m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_SMOKE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkEating::Uninit(void)
{
	// 親クラスの終了処理
	CAfk::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkEating::Update(void)
{
	// 親クラスの更新処理
	CAfk::Update();

	if (m_pParticle == nullptr)return;

	// プレイヤーがさぼっているか判定用の変数
	bool bAfkEating = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkEating();

	if (bAfkEating) m_pParticle->SetUse(true);
	else m_pParticle->SetUse(false);

	float fColR = (float)(rand() / (RAND_MAX + 1.0f));
	float fColG = (float)(rand() / (RAND_MAX + 1.0f));

	m_pParticle->SetColor(D3DXCOLOR(fColR, fColG, 0.0f, 1.0f));

}