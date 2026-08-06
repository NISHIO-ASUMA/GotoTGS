//=========================================================
//
// 警戒ゲージ処理 [ vigilancegauge.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "vigilancegauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "enemymanager.h"
#include "gamesceneobject.h"
#include "gametime.h"
#include "ui.h"
#include "sound.h"

//=========================================================
// コンストラクタ
//=========================================================
CVigilancegauge::CVigilancegauge(int nPriority) :CObject2DMulti(nPriority),
m_fRatio(NULL),
m_nActiveCount(NULL),
m_pActiveUi(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CVigilancegauge::~CVigilancegauge()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CVigilancegauge* CVigilancegauge::Create(const Gauge& gauge, const char* BTEXTURE, const char* MTEXTURE)
{
	// インスタンス生成
	CVigilancegauge* pGauge = new CVigilancegauge;

	// ヌルチェック
	if (pGauge == nullptr) return nullptr;

	// 初期化が失敗したとき
	if (FAILED(pGauge->Init())) return nullptr;

	// 各設定処理
	pGauge->SetPos(gauge.pos);						// 位置
	pGauge->SetCol(gauge.col);						// 色
	pGauge->SetSize(gauge.fWidth, gauge.fHeight);	// サイズ
	pGauge->SetTexture(BTEXTURE, 0);				// ベースのテクスチャ
	pGauge->SetTexture(MTEXTURE, 1);				// 上に重ねるテクスチャ
	pGauge->SetAnchorType(gauge.nAnchorType);		// アンカーポイント

	return pGauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilancegauge::Init(void)
{
	// 親の初期化処理
	CObject2DMulti::Init();

	// ui生成
	m_pActiveUi = CUi::Create(D3DXVECTOR3(640.0f, 90.0f, 0.0f), 0, 300.0f, 65.0f, "addchar00.png");
	m_pActiveUi->SetUse(false);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilancegauge::Uninit(void)
{
	// 親の終了処理
	CObject2DMulti::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CVigilancegauge::Update(void)
{
	// 比率を増やす
	m_fRatio += Config::RATIO_VALUE;

	// 最大比率を超えたら
	if (m_fRatio >= 1.0f)
	{
		// 1 or 2をランダム化
		int nRand = rand() % 2 + 1;

		// 敵を生成する
		switch (nRand)
		{
		case 1:
			CEnemyManager::GetInstance()->CreateManager(D3DXVECTOR3(420.0f,0.0f,190.0f), VECTOR3_NULL, CEnemy::MOVETYPE_SMOKE); // 煙草
			break;

		case 2:
			CEnemyManager::GetInstance()->CreateManager(D3DXVECTOR3(-120.0f, 0.0f, 240.0f), VECTOR3_NULL, CEnemy::MOVETYPE_TV);	 // TV
			break;

		default:
			CEnemyManager::GetInstance()->CreateManager(VECTOR3_NULL, VECTOR3_NULL, CEnemy::MOVETYPE_NORMAL);					// デフォルト
			break;
		}

		// uiを表示する
		m_pActiveUi->SetUse(true);
		m_nActiveCount = NULL;

		// サウンド再生
		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_ACTIVECHARACTOR_SE);

		// 比率を元に戻す
		m_fRatio = 0.0f;
	}

	if (m_pActiveUi->GetUse())
	{
		// カウントを加算
		m_nActiveCount++;

		// 3秒経過したら表示をOFFにする
		if (m_nActiveCount >= 180)
		{
			m_pActiveUi->SetUse(false);
			m_nActiveCount = NULL;
		}
	}
	// テクスチャのUVを比率分動かす
	CObject2DMulti::SetUV(m_fRatio);

	// 親の更新処理
	CObject2DMulti::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	// 親の描画処理
	CObject2DMulti::Draw();
}

//=========================================================
// 生成するものを変化させる処理 ( 後々改良案件 )
//=========================================================
void CVigilancegauge::CreateCharactor(const int nNumber)
{
	switch (nNumber)
	{
	case 0:
		// 敵を生成
		//CEnemyManager::GetInstance()->CreateManager(VECTOR3_NULL, VECTOR3_NULL);
		break;

	case 1:
		// ボス ( 社長 )を生成 → これは一回だけかな

		break;

	default:
		break;
	}
}