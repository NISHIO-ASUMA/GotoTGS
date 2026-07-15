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

//=========================================================
// コンストラクタ
//=========================================================
CVigilancegauge::CVigilancegauge(int nPriority) :CObject2DMulti(nPriority),
m_fRatio(NULL),
m_nLevelCount(NULL)
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
	// 親の更新処理
	CObject2DMulti::Update();

	// 実験
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_M))
	{
		// 比率を増やす
		m_fRatio += 0.05f;
	}

	// 最大比率を超えたら
	if (m_fRatio >= 1.0f)
	{
		// 比率を元に戻す
		m_fRatio = 0.0f;

		if (m_nLevelCount < 10)
		{// レベル10を最大にする
			// レベルを上げる
			m_nLevelCount++;
		}
	}

	// テクスチャのUVを比率分動かす
	CObject2DMulti::SetUV(m_fRatio);
}

//=========================================================
// 描画処理
//=========================================================
void CVigilancegauge::Draw(void)
{
	// 親の描画処理
	CObject2DMulti::Draw();
}