//=========================================================
//
// 警戒度UIマネージャー処理 [ vigilanceUImanager.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "vigilanceUImanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CVigilanceUIManager::CVigilanceUIManager(int nPriority) :CObject(nPriority),
m_pIcon(nullptr),
m_pGauge(nullptr),
m_bUse(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CVigilanceUIManager::~CVigilanceUIManager()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CVigilanceUIManager* CVigilanceUIManager::Create(const bool& bUse)
{
	// 生成
	CVigilanceUIManager* pVigilanceUIManager = new CVigilanceUIManager;

	// ヌルチェック
	if (pVigilanceUIManager == nullptr) return nullptr;

	// 表示するかどうかを設定
	pVigilanceUIManager->SetUse(bUse);

	// 初期化が失敗したとき
	if (FAILED(pVigilanceUIManager->Init())) return nullptr;

	return pVigilanceUIManager;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilanceUIManager::Init(void)
{
	// 位置と横幅
	D3DXVECTOR3 pos = D3DXVECTOR3(Config::ICON_POS_X, Config::ICON_POS_Y, 0.0f);
	float fWidth = Config::ICON_WIDTH;
	float fHeight = Config::ICON_HEIGHT;

	// ゲージの構造体
	CVigilancegauge::Gauge gauge;
	gauge.pos.x = pos.x;
	gauge.pos.y = pos.y + Config::GAUGE_HEIGHT;
	gauge.col = COLOR_WHITE;
	gauge.fWidth = Config::GAUGE_WIDTH;
	gauge.fHeight = Config::GAUGE_HEIGHT;
	gauge.nAnchorType = CObject2DMulti::ANCHORTYPE_BOTTOMSIDE;

	// ゲージの生成処理
	m_pGauge = CVigilancegauge::Create(gauge, Config::GAUGE_BASE, Config::GAUGE_MULTI);

	// アイコンの構造体
	CVigilanceicon::Icon icon;
	icon.pos = pos;
	icon.fWidth = fWidth;
	icon.fHeight = fHeight;

	// アイコンの生成処理
	m_pIcon = CVigilanceicon::Create(icon, Config::ICON_ENEMY);

	// フレームの構造体
	icon.fWidth = gauge.fWidth + 5.0f;
	icon.fHeight = gauge.fHeight + 5.0f;

	// ゲージのフレーム生成処理
	CVigilanceicon::Create(icon, Config::GAUGE_FREAM);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilanceUIManager::Uninit(void)
{
	// 各ポインタの終了処理
	if (m_pIcon != nullptr)
	{// アイコン
		m_pIcon->Uninit();
		m_pIcon = nullptr;
	}

	if (m_pGauge != nullptr)
	{// ゲージ
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}

}

//=========================================================
// 更新処理
//=========================================================
void CVigilanceUIManager::Update(void)
{
	// 各ポインタの更新処理
	m_pIcon->Update();
	m_pGauge->Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilanceUIManager::Draw(void)
{
	// 各ポインタの更新処理
	m_pIcon->Draw();
	m_pGauge->Draw();
}