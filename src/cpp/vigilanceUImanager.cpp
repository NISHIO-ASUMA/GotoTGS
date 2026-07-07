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
#include "vigilanceicon.h"
#include "vigilancegauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CVigilanceUIManager::CVigilanceUIManager(int nPriority) :CObject(nPriority),
pIcon(nullptr),
pGauge(nullptr),
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
	// アイコンの構造体
	CVigilanceicon::Icon icon;
	icon.pos = D3DXVECTOR3(Config::ICON_POS_X, Config::ICON_POS_Y, 0.0f);
	icon.fWidth = Config::ICON_WIDTH;
	icon.fHeight = Config::ICON_HEIGHT;

	// アイコンの生成処理
	pIcon = CVigilanceicon::Create(icon, Config::ICON_TEXNAME);

	// ゲージの構造体
	CVigilancegauge::Gauge gauge;
	gauge.pos = D3DXVECTOR3(Config::GAUGE_POS_X, Config::GAUGE_POS_Y + Config::GAUGE_HEIGHT, 0.0f);
	gauge.col = COLOR_WHITE;
	gauge.fWidth = Config::GAUGE_WIDTH;
	gauge.fHeight = Config::GAUGE_HEIGHT;
	gauge.nAnchorType = CObject2DMulti::ANCHORTYPE_BOTTOMSIDE;

	// ゲージの生成処理
	pGauge = CVigilancegauge::Create(gauge, Config::GAUGE_TEXNAME_BASE, Config::GAUGE_TEXNAME_MULTI);

	icon.pos.x = gauge.pos.x;
	icon.pos.y = Config::GAUGE_POS_Y;
	icon.fWidth = gauge.fWidth;
	icon.fHeight = gauge.fHeight;

	// ゲージのフレーム生成処理
	CVigilanceicon::Create(icon, Config::GAUGE_FREAM_TEXNAME);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilanceUIManager::Uninit(void)
{
	// 各ポインタの終了処理
	if (pIcon != nullptr)
	{
		pIcon->Uninit();
		pIcon = nullptr;
	}

	if (pGauge != nullptr)
	{
		pGauge->Uninit();
		pGauge = nullptr;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CVigilanceUIManager::Update(void)
{

}

//=========================================================
// 描画処理
//=========================================================
void CVigilanceUIManager::Draw(void)
{

}