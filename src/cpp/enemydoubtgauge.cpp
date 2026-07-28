//=========================================================
//
// 疑いゲージマルチオブジェクトクラス [ enemydoubtgauge.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemydoubtgauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyDoubtGauge::CEnemyDoubtGauge(int nPriority) : CBillboardMulti(nPriority),
m_fRatio(NULL),
m_isDraw(false),
m_isComplete(false),
m_isUpGauge(false),
m_isNormalSet(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemyDoubtGauge::~CEnemyDoubtGauge()
{

}
//=========================================================
// 生成処理
//=========================================================
CEnemyDoubtGauge* CEnemyDoubtGauge::Create
(
	const D3DXVECTOR3& pos,
	const float fWidth,
	const float fHeight,
	const char* pBaseTex,
	const char* pMultitexture
)
{
	// インスタンス生成
	CEnemyDoubtGauge* pGauge = new CEnemyDoubtGauge;
	if (pGauge == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pGauge->Init())) return nullptr;

	// オブジェクト設定
	pGauge->SetPos(pos);
	pGauge->SetSize(fWidth, fHeight);
	pGauge->SetTexture(pBaseTex, 0);
	pGauge->SetTexture(pMultitexture, 1);

	return pGauge;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemyDoubtGauge::Init(void)
{
	// 親クラスの初期化処理
	CBillboardMulti::Init();
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemyDoubtGauge::Uninit(void)
{
	// 親クラスの終了処理
	CBillboardMulti::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemyDoubtGauge::Update(void)
{
	// フラグがoffなら
	if (!m_isDraw) return;

	// 親クラスの更新処理
	CBillboardMulti::Update();

	// 比率を増やす
	if (m_isUpGauge)
		m_fRatio += 0.0040f;

	// 最大比率を超えたら
	if (m_fRatio >= 1.0f)
	{
		// 比率を最大にする
		m_fRatio = 1.0f;

		// フラグを設定
		m_isComplete = true;
		return;
	}
	else
	{
		m_isComplete = false;
	}

	// テクスチャのUVを比率分動かす
	CBillboardMulti::SetUV(m_fRatio);
}
//=========================================================
// 描画処理
//=========================================================
void CEnemyDoubtGauge::Draw(void)
{
	// 描画していないなら
	if (!m_isDraw) return;

	// 親クラスの描画
	CBillboardMulti::Draw();
}
//=========================================================
// 割合値の変更
//=========================================================
void CEnemyDoubtGauge::SetRatio(const float fRatio)
{
	// フラグ変更
	m_isComplete = false;

	// 少しずつ減らす
	m_fRatio -= fRatio;

	// フラグ変更
	if (m_fRatio <= 0.0f)
	{
		m_fRatio = 0.0f;
		m_isDraw = false;
		m_isNormalSet = true;
	}
}