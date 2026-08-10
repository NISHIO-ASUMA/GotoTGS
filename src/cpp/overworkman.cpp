//========================================================
//
//過労死キャラクター処理 [ overworkman.cpp ]
// Author: Asuma Nishio
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "overworkman.h"
#include "motion.h"

//========================================================
// コンストラクタ
//========================================================
COverWorkMan::COverWorkMan(int nPriority) : CNoMoveCharactor(nPriority)
{
}
//========================================================
// デストラクタ
//========================================================
COverWorkMan::~COverWorkMan()
{

}
//========================================================
// 生成処理
//========================================================
COverWorkMan* COverWorkMan::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	COverWorkMan* pWork= new COverWorkMan;
	if (pWork == nullptr) return nullptr;

	// オブジェクト設定
	pWork->SetPos(pos);
	pWork->SetRot(rot);
	pWork->SetUseOutLine(true);
	pWork->SetOutLineColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

	// 初期化失敗時
	if (FAILED(pWork->Init())) return nullptr;

	return pWork;
}
//========================================================
// 初期化処理
//========================================================
HRESULT COverWorkMan::Init(void)
{
	// 親クラスの初期化処理
	CNoMoveCharactor::Init();

	// モーションロード
	MotionLoad("data/MOTION/Player/LoseResultPlayerMotion.txt", MOTION::MAX, true);

	// モーションセット
	GetMotion()->SetMotion(MOTION::DEATH, true, 3,false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void COverWorkMan::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void COverWorkMan::Update(void)
{
	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void COverWorkMan::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}