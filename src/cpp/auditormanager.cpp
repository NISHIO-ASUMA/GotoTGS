//========================================================
//
// 外の監査役複数管理処理 [ auditormanager.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "auditormanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "jsonmanager.h"
#include "auditorstatechase.h"
#include "auditorstateneutral.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace AUDITOR_INFO
{
	constexpr const char* FILE = "data/JSON/AuditorList.json";
};

//========================================================
// コンストラクタ
//========================================================
CAuditorManager::CAuditorManager() : m_pAuditor{},
m_pOutSidePlayer(nullptr)
{

}
//========================================================
// デストラクタ
//========================================================
CAuditorManager::~CAuditorManager()
{
	m_pAuditor.clear();
}
//========================================================
// 初期化処理
//========================================================
HRESULT CAuditorManager::Init(void)
{
	// 配列クリア
	m_pAuditor.clear();

	// 外部ファイルロード
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->SetAuditorManager(GetInstance());
	jsonmanager->Load(AUDITOR_INFO::FILE);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CAuditorManager::Uninit(void)
{
	// 配列クリア
	m_pAuditor.clear();
}
//========================================================
// 更新処理
//========================================================
void CAuditorManager::Update(void)
{

}
//========================================================
// 全体の動きのシステム変更
//========================================================
void CAuditorManager::ChangeSystem(void)
{
	// ステートを"猛追"に変更
	for (int nCnt = 0; nCnt < static_cast<int>(m_pAuditor.size()); nCnt++)
	{
		if (m_pAuditor[nCnt])
			m_pAuditor[nCnt]->ChangeState(new CAuditorStateChase(), CAuditorStateBase::ID_CHASE);
	}
}
//========================================================
// 全体の動きのシステム変更
//========================================================
void CAuditorManager::ChangeSystemNeutral(void)
{
	// ステートを"通常"に変更
	for (int nCnt = 0; nCnt < static_cast<int>(m_pAuditor.size()); nCnt++)
	{
		if (m_pAuditor[nCnt])
			m_pAuditor[nCnt]->ChangeState(new CAuditorStateNeutral(), CAuditorStateBase::ID_NEUTRAL);
	}
}
//========================================================
// ポインタ生成と配列追加関数
//========================================================
void CAuditorManager::CreateInManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const CAuditor::MOVE_POINTTYPE& type)
{
	// 新規追加生成
	m_pAuditor.push_back(CAuditor::Create(pos, rot, type));

	// プレイヤーのポインタを設定する
	if (m_pOutSidePlayer)
		m_pAuditor.back()->SetPlayer(m_pOutSidePlayer);
}