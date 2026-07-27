//=========================================================
//
// リザルトのオブジェクト管理 [ resultobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "resultscoremanager.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "resulticon.h"

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace RESULTOBJECT
{
	constexpr const char* LoadName = "data/JSON/Resultobject.json"; // 配置物jsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CResultObject::CResultObject()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CResultObject::~CResultObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultObject::Init(void)
{
#ifdef NDEBUG
	// マップ読み込み
	m_pBlock = std::make_unique<CBlockManager>();
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlock.get());

	// ブロック管理クラスの初期化
	m_pBlock->Init();
#endif // NDEBUG

	// リザルトシーンオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(RESULTOBJECT::LoadName);

	// リザルトのスコア生成
	CResultScoreManager::GetInstancce()->Init();

	// アイコン生成
	//CResultIcon::Create(D3DXVECTOR3(1020.0f, 570.0f, 0.0f), 130.0f, 130.0f, "ResultIcon/Saboriman.png");

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultObject::Uninit(void)
{
	// リザルトのスコア破棄
	CResultScoreManager::GetInstancce()->Uninit();

	// ステージマップの破棄
	m_pBlock.reset();
}
//=========================================================
// インスタンス取得処理
//=========================================================
CResultObject* CResultObject::GetInstance(void)
{
	static CResultObject Instance;
	return &Instance;
}