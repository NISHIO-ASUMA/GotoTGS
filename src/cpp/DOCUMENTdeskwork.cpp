//=========================================================
//
// 書類タスク処理 [ DOCUMENTdeskwork.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "DOCUMENTdeskwork.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "score.h"
#include "progressgauge.h"
#include "gaugeneedle.h"
#include "gamesceneobject.h"
#include "titleuimanager.h"
#include "tutorialobject.h"
#include "tutoriallines.h"
#include "particle.h"

//*********************************************************
// 名前空間(パーティクル)
//*********************************************************
namespace PARTICLE
{
	const D3DXVECTOR3 Pos = { 40.0, 36.0, 280.0 };		// 位置
	const D3DXCOLOR col = { 0.5f, 0.5f, 1.0f, 0.5f };	// カラー
	constexpr float fRadius = 15.0f;						// 範囲の半径
	constexpr int nTime = 15;							// 表示する時間
};

//=========================================================
// コンストラクタ
//=========================================================
CDOCUMENTDeskwork::CDOCUMENTDeskwork() :CDeskworkUIManager(),
m_pParticle(nullptr),
m_nParticleTime(NULL)
{

}

//=========================================================
// デストラクタ
//=========================================================
CDOCUMENTDeskwork::~CDOCUMENTDeskwork()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CDOCUMENTDeskwork* CDOCUMENTDeskwork::Create(const bool& bUse)
{
	// 書類タスクのポインタ
	static CDOCUMENTDeskwork pDOCUMENTDeskwork;

	// 設定処理
	pDOCUMENTDeskwork.SetUse(bUse);

	// 初期化が失敗した場合
	if (FAILED(pDOCUMENTDeskwork.Init())) return nullptr;

	return &pDOCUMENTDeskwork;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDOCUMENTDeskwork::Init(void)
{
	// パーティクルの生成処理
	m_pParticle = CParticle::Create(PARTICLE::Pos, VECTOR3_NULL, PARTICLE::col, PARTICLE::fRadius, CParticle::TYPE_NONE);

	// 使っていない状態にする
	m_pParticle->SetUse(false);


	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDOCUMENTDeskwork::Uninit(void)
{
	if (m_pParticle != nullptr)
	{// パーティクルの終了処理
		m_pParticle = nullptr;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CDOCUMENTDeskwork::Update(void)
{
	if (!m_pParticle->GetUse()) return;

	// 時間を進める
	m_nParticleTime++;

	if (m_pParticle != nullptr)
	{// パーティクルの更新処理
		m_pParticle->Update();
	}

	// 表示する時間を超えていたら
	if (m_nParticleTime > PARTICLE::nTime)
	{
		// パーティクルをOFFにする
		m_pParticle->SetUse(false);

	}

}

//=========================================================
// 描画処理
//=========================================================
void CDOCUMENTDeskwork::Draw(void)
{
	if (m_pParticle != nullptr)
	{// パーティクルの描画処理
		m_pParticle->Draw();
	}

}

//=========================================================
// こなした書類タスクの数の設定処理
//=========================================================
void CDOCUMENTDeskwork::SetDOCUMENTValue(void)
{
	// 1つもコピー機タスクをこなしていない場合
	if (GetCOPYTaskNum() <= 0) return;

	// パーティクルをONにする
	m_pParticle->SetUse(true);

	// 書類タスクの数の加算処理
	AddDOCUMENTTask();

	// チュートリアルでの処理
	if (CManager::GetInstance()->GetScene() == CScene::MODE::MODE_TUTORIAL && 
		!CTutorialObject::GetInstance()->GetTutoriallines()->GetIsComp())
	{
		// チュートリアルを進める
		CTutorialObject::GetInstance()->GetTutoriallines()->SetNextTutorial();
		return;
	}

	// もしチュートリアル以外であったなら
	if (CManager::GetInstance()->GetScene() != CScene::MODE::MODE_TUTORIAL)
	{
		// スコアのポインタ
		auto* pScore = CGameSceneObject::GetInstance()->GetScore();
		// 指針のポインタ
		CGaugeneedle* pGaugeneedle = CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle();
		if (pScore == nullptr || pGaugeneedle == nullptr) return;

		// スコア加算
		pScore->AddScoreMinus(-1000);

		// こなしたタスクの数を増やす
		pGaugeneedle->AddTask();
	}
}
