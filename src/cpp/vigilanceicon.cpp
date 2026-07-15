//=========================================================
//
// 警戒ゲージ処理 [ vigilancegauge.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "vigilanceicon.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CVigilanceicon::CVigilanceicon(int nPriority) :CObject2D(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CVigilanceicon::~CVigilanceicon()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CVigilanceicon* CVigilanceicon::Create(const Icon& icon, const char* TEXNAME)
{
	// インスタンス生成
	CVigilanceicon* pVigilancegauge = new CVigilanceicon;

	// ヌルチェック
	if (pVigilancegauge == nullptr) return nullptr;

	// 初期化が失敗したとき
	if (FAILED(pVigilancegauge->Init())) return nullptr;

	// 各設定処理
	pVigilancegauge->SetPos(icon.pos);						// 位置
	pVigilancegauge->SetSize(icon.fWidth, icon.fHeight);	// サイズ
	pVigilancegauge->SetTexture(TEXNAME);					// テクスチャ名

	return pVigilancegauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CVigilanceicon::Init(void)
{
	// 親の初期化処理
	CObject2D::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CVigilanceicon::Uninit(void)
{
	// 親の終了処理
	CObject2D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CVigilanceicon::Update(void)
{
	// 親の更新処理
	CObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CVigilanceicon::Draw(void)
{
	// 親の描画処理
	CObject2D::Draw();
}