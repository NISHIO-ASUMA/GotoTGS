//=========================================================
//
// AFKGameCenter処理 [ afkgamecenter.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkgamecenter.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間(ゲームセンターさぼり)
//*********************************************************
namespace AFKGAMECENTER
{
	const D3DXVECTOR3 Pos = { 1705.0f, 0.0f, 166.0f };	// 位置
	constexpr float fRadius = 5.0f;						// 範囲の半径
};
//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { 1705.0f, 10.0f, 166.0f };	// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 0.5f, 0.5f };	// カラー
	constexpr float fRadius = 10.0f;					// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkGameCenter::CAfkGameCenter() : m_pParticle(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfkGameCenter::~CAfkGameCenter()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfkGameCenter* CAfkGameCenter::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfkGameCenter* pAfkGameCenter = new CAfkGameCenter;
	if (pAfkGameCenter == nullptr) return nullptr;

	// オブジェクトセット
	pAfkGameCenter->SetPos(AFKGAMECENTER::Pos);

	// 初期化失敗時
	if (FAILED(pAfkGameCenter->Init())) return nullptr;

	// ポインタを返す
	return pAfkGameCenter;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkGameCenter::Init(void)
{
	// 親クラスの初期化処理
	CAfk::Init(AFKGAMECENTER::Pos, AFKGAMECENTER::fRadius);

	// パーティクル生成
	m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_NONE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkGameCenter::Uninit(void)
{
	// 親クラスの終了処理
	CAfk::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkGameCenter::Update(void)
{
	// 親クラスの更新処理
	CAfk::Update();

	if (m_pParticle == nullptr)return;

	// プレイヤーがさぼっているか判定用の変数
	bool bAfkGameCenter = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkGameCenter();

	if (bAfkGameCenter) m_pParticle->SetUse(true);
	else m_pParticle->SetUse(false);

	float fColR = (float)(rand() / (RAND_MAX + 1.0f));
	float fColG = (float)(rand() / (RAND_MAX + 1.0f));
	float fColB = (float)(rand() / (RAND_MAX + 1.0f));

	m_pParticle->SetColor(D3DXCOLOR(fColR, fColG, fColB,1.0f));
}