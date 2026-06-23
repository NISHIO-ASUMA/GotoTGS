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

// テスト
#include "particle.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace AFKSMOKE
{
	const D3DXVECTOR3 Pos = { 295.0f, 0.0f, 325.0f };			// たばこさぼりの範囲
	const D3DXVECTOR3 ParticlePos = { 321.0f, 50.0f, 320.0f };	// パーティクル位置
	constexpr float fRadius = 25.0f;							// 範囲の半径
};

//=========================================================
// コンストラクタ
//=========================================================
CAfksmoke::CAfksmoke()
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

	CParticle::Create(AFKSMOKE::ParticlePos, VECTOR3_NULL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 15000, 15.0f, CParticle::TYPE_SMOKE);

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
}