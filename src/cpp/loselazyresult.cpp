//=========================================================
//
// 捕まった時のリザルトシーン処理 [ loselazyresult.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "loselazyresult.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "camera.h"
#include "meshfield.h"
#include "blockmanager.h"
#include "jsonmanager.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CLoseLazyResult::CLoseLazyResult() : CScene(CScene::MODE_LOSELAZY),
m_pBlock(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CLoseLazyResult::~CLoseLazyResult()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CLoseLazyResult::Init(void)
{
	// カメラの設定を変更する
	CManager::GetInstance()->GetCamera()->ResultCamera();

	// メッシュ生成
	CMeshField::Create(VECTOR3_NULL, 1000.0f, 1000.0f, 1, 1, "data/TEXTURE/field00.jpg");

	// ブロックマネージャー生成
	m_pBlock = std::make_unique<CBlockManager>();
	const auto& jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlock.get());

	// 初期化とポインタセット
	m_pBlock->SetLoadFileName();
	m_pBlock->Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CLoseLazyResult::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CLoseLazyResult::Update(void)
{

}
//=========================================================
// 描画処理
//=========================================================
void CLoseLazyResult::Draw(void)
{

}