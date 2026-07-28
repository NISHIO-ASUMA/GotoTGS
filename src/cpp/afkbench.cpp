//=========================================================
//
// AFKBench処理 [ afkbench.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkbench.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間(ゲームセンターさぼり)
//*********************************************************
namespace AFKBENCH
{
	constexpr float fRadius = 25.0f;					// 範囲の半径
};
//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { 792.4f, 14.0f, 1303.6f };	// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 0.5f, 0.5f };	// カラー
	constexpr float fRadius = 10.0f;					// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkBench::CAfkBench() : m_pParticle(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfkBench::~CAfkBench()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfkBench* CAfkBench::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfkBench* pAfkBench = new CAfkBench;
	if (pAfkBench == nullptr) return nullptr;

	// オブジェクトセット
	pAfkBench->SetPos(pos);

	// 初期化失敗時
	if (FAILED(pAfkBench->Init(pos))) return nullptr;

	// ポインタを返す
	return pAfkBench;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkBench::Init(D3DXVECTOR3 pos)
{
	// 親クラスの初期化処理
	CAfk::Init(pos, AFKBENCH::fRadius);

	//// パーティクル生成
	//m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_NONE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkBench::Uninit(void)
{
	// 親クラスの終了処理
	CAfk::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkBench::Update(void)
{
	// 親クラスの更新処理
	CAfk::Update();

	//if (m_pParticle == nullptr)return;

	//// プレイヤーがさぼっているか判定用の変数
	//bool bAfkGameCenter = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkGameCenter();

	//if (bAfkGameCenter) m_pParticle->SetUse(true);
	//else m_pParticle->SetUse(false);

	//float fColR = (float)(rand() / (RAND_MAX + 1.0f));
	//float fColG = (float)(rand() / (RAND_MAX + 1.0f));
	//float fColB = (float)(rand() / (RAND_MAX + 1.0f));

	//m_pParticle->SetColor(D3DXCOLOR(fColR, fColG, fColB, 1.0f));
}