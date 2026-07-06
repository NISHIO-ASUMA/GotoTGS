//=========================================================
//
// AFKたばこ処理 [ afksmoke.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afksmoke.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間(たばこさぼり)
//*********************************************************
namespace AFKSMOKE
{
	const D3DXVECTOR3 Pos = { 295.0f, 0.0f, 325.0f };	// たばこさぼりの範囲
	constexpr float fRadius = 25.0f;					// 範囲の半径
};
//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { 321.0f, 25.0f, 320.0f };	// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 0.5f, 0.5f };	// カラー
	constexpr float fRadius = 10.0f;					// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfksmoke::CAfksmoke() : m_pParticle(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfksmoke::~CAfksmoke()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfksmoke* CAfksmoke::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfksmoke* pAfkSmoke = new CAfksmoke;
	if (pAfkSmoke == nullptr) return nullptr;

	// オブジェクトセット
	pAfkSmoke->SetPos(AFKSMOKE::Pos);
	pAfkSmoke->SetRadius(AFKSMOKE::fRadius);

	// 初期化失敗時
	if (FAILED(pAfkSmoke->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pAfkSmoke;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfksmoke::Init(void)
{
	// 親クラスの初期化処理
	CAfk::Init(AFKSMOKE::Pos,AFKSMOKE::fRadius);

	// パーティクル生成
	m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_SMOKE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfksmoke::Uninit(void)
{
	// 親クラスの終了処理
	CAfk::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfksmoke::Update(void)
{
	// 親クラスの更新処理
	CAfk::Update();

	// プレイヤーがさぼっているか判定用の変数
	bool bAfkSmoke = CGameSceneObject::GetInstance()->GetPlayer()->GetAfkSmoke();

	if (bAfkSmoke) m_pParticle->SetUse(true);
	else m_pParticle->SetUse(false);

}