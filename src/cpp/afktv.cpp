//=========================================================
//
// AFKTV処理 [ afktv.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afktv.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間(テレビさぼり)
//*********************************************************
namespace AFKTV
{
	const D3DXVECTOR3 Pos = { -253.0f, 0.0f, 313.0f };	// さぼりの範囲
	constexpr float fRadius = 0.025f;					// 範囲の半径
};
//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { -249.0f, 47.0f, 380.0f };	// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 0.5f, 0.5f };	// カラー
	constexpr float fRadius = 10.0f;					// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkTV::CAfkTV() : m_pParticle(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfkTV::~CAfkTV()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfkTV* CAfkTV::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfkTV* pAfkTV = new CAfkTV;
	if (pAfkTV == nullptr) return nullptr;

	// オブジェクトセット
	pAfkTV->SetPos(AFKTV::Pos);
	pAfkTV->SetRadius(AFKTV::fRadius);

	// 初期化失敗時
	if (FAILED(pAfkTV->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pAfkTV;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkTV::Init(void)
{
	// 親クラスの初期化処理
	CAfk::Init(AFKTV::Pos, AFKTV::fRadius);

	// パーティクル生成
	m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_NEARBY);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkTV::Uninit(void)
{
	// 親クラスの終了処理
	CAfk::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkTV::Update(void)
{
	// 親クラスの更新処理
	CAfk::Update();

	if (m_pParticle == nullptr)return;

	// プレイヤーがさぼっているか判定用の変数
	bool bAfkTV = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkTV();

	if (bAfkTV) m_pParticle->SetUse(true);
	else m_pParticle->SetUse(false);

	float fColR = (float)(rand() / (RAND_MAX + 1.0f));
	float fColG = (float)(rand() / (RAND_MAX + 1.0f));

	m_pParticle->SetColor(D3DXCOLOR(fColR, fColG, 0.0f, 1.0f));

}