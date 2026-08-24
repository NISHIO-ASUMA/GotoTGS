//=========================================================
//
// ランキングのオブジェクト管理 [ rankingobject.cpp ]
// Author: Asuma Nishio
//
// NOTE : ださいのなおしましょう
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankingobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "rankingscore.h"
#include "jsonmanager.h"
#include "ui.h"
#include "blockmanager.h"
#include "rankinggraphmanager.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CRankingObject* CRankingObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace RANKINGOBJECT
{
	const D3DXVECTOR3 ScorePos		= { 1100.0f, 160.0f, 0.0f };	// ランキングスコアの座標
	constexpr float ScoreWidth		= 200.0f;					// ポリゴンの横幅
	constexpr float ScoreHeight		= 40.0f;					// ポリゴンの高さ
	constexpr const char* LoadName	= "data/JSON/Rankingobject.json"; // 読み込むjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CRankingObject::CRankingObject() : m_pBlock(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CRankingObject::~CRankingObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingObject::Init(void)
{
	// ブロックマネージャー生成
	m_pBlock = std::make_unique<CBlockManager>();
	const auto& jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlock.get());

	// 初期化とポインタセット
	m_pBlock->SetLoadFileName();
	m_pBlock->Init();

	// フィールド読み込み
	jsonManager->Load(RANKINGOBJECT::LoadName);

	// ランキングスコア生成
	CRankingScore::Create(RANKINGOBJECT::ScorePos, RANKINGOBJECT::ScoreWidth, RANKINGOBJECT::ScoreHeight);

	// ランキングの横棒生成
	CRankingGraphManager::GetInstance()->Init(D3DXVECTOR3(230.0f, 135.0f, 0.0f), 60.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingObject::Uninit(void)
{
	// ランキングの横棒生成
	CRankingGraphManager::GetInstance()->Uninit();

	// ポインタの破棄
	m_pBlock.reset();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// インスタンス取得処理
//=========================================================
CRankingObject* CRankingObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CRankingObject;

	// 生成されたインスタンスを返す
	return m_pInstance;
}