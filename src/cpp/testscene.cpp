//==========================================================
//
// テストシーン処理 [ testscene.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "testscene.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boss.h"
#include "manager.h"
#include "jsonmanager.h"
#include "meshfield.h"
#include "input.h"
#include "blockmanager.h"

//==========================================================
// コンストラクタ
//==========================================================
CTest::CTest() : CScene(MODE_TEST),
m_pBoss(nullptr),
m_pBlocks(nullptr)
{

}
//==========================================================
// デストラクタ
//==========================================================
CTest::~CTest()
{

}
//==========================================================
// 初期化処理
//==========================================================
HRESULT CTest::Init(void)
{
	// マップ生成
	m_pBlocks = std::make_unique<CBlockManager>();
	const auto& jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlocks.get());

	m_pBlocks->SetLoadFileName();
	m_pBlocks->Init();

	// キャラクター生成
	m_pBoss = CBoss::Create({ 677.5f,0.0f,320.0f }, VECTOR3_NULL);

	// メッシュフィールド生成
	CMeshField::Create(VECTOR3_NULL,4000.0f,4000.0f,1,1,"data/TEXTURE/Floor.jpg");

	return S_OK;
}
//==========================================================
// 終了処理
//==========================================================
void CTest::Uninit(void)
{
	m_pBlocks.reset();
}
//==========================================================
// 更新処理
//==========================================================
void CTest::Update(void)
{
	// キー入力でフラグ制御解放する
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_L))
	{
		m_pBoss->SetActiveFlags(true);
	}
}
//==========================================================
// 描画処理
//==========================================================
void CTest::Draw(void)
{

}