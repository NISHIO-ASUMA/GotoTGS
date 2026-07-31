//=========================================================
//
// サボりの出来ない時間を表示するui処理 [ afkbattime.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkbattime.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "template.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace COOLTIME_UI
{
	constexpr float SIZE = 15.0f; // 大きさ
	constexpr const char* BASE_TEX = "bat_frame.png";		// ベース
	constexpr const char* MULTI_TEX = "gauge_battime.png";	// マルチ
	
	constexpr float COOL_TIME_SEC = 10.0f;					// クールタイムフレーム
	constexpr float FPS = 60.0f;							// 対象FPS
	constexpr float TOTAL_FRAMES = COOL_TIME_SEC * FPS;		// 全フレーム数（600）
	constexpr float RATIO_VALUE = 1.0f / TOTAL_FRAMES;		// 割合値
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkCoolTimeUi::CAfkCoolTimeUi(int nPriority) : CBillboardMulti(nPriority),
m_isUse(false),
m_fRatio(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CAfkCoolTimeUi::~CAfkCoolTimeUi()
{

}
//=========================================================
// 生成処理
//=========================================================
CAfkCoolTimeUi* CAfkCoolTimeUi::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CAfkCoolTimeUi* pUi = new CAfkCoolTimeUi;
	if (pUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init())) return nullptr;

	// オブジェクト設定
	pUi->SetPos(pos);									// 位置
	pUi->SetSize(COOLTIME_UI::SIZE, COOLTIME_UI::SIZE);	// サイズ
	pUi->SetTexture(COOLTIME_UI::BASE_TEX, 0);			// ベースのテクスチャ
	pUi->SetTexture(COOLTIME_UI::MULTI_TEX, 1);			// マルチテクスチャ

	return pUi;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkCoolTimeUi::Init(void)
{
	// 親クラスの初期化処理
	CBillboardMulti::Init();

	// 変数の初期化
	m_fRatio = 1.0f;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkCoolTimeUi::Uninit(void)
{
	// 親クラスの終了処理
	CBillboardMulti::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAfkCoolTimeUi::Update(void)
{
	if (!m_isUse) return;

	// 親クラスの更新処理
	CBillboardMulti::Update();

	// 比率を減らす
	m_fRatio -= COOLTIME_UI::RATIO_VALUE;

	if (m_fRatio <= 0.0f)
	{
		m_fRatio = 0.0f;
		m_isUse = false; // 未使用状態にする
		return;
	}

	// テクスチャのUVを比率分動かす
	CBillboardMulti::SetUV(m_fRatio);
}
//=========================================================
// 描画処理
//=========================================================
void CAfkCoolTimeUi::Draw(void)
{
	if (!m_isUse) return;

	// 親クラスの描画処理
	CBillboardMulti::Draw();
}
//=========================================================
// 開始関数
//=========================================================
void CAfkCoolTimeUi::StartSet(void)
{
	SetUse(true);
	m_fRatio = 1.0f;
}