//=========================================================
//
// ブロック管理処理 [ blockmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "blockmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "jsonmanager.h"
#include "manager.h"
#include "camera.h"
#include "movecharactor.h"

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json; // jsonクラスの使用

//=========================================================
// コンストラクタ
//=========================================================
CBlockManager::CBlockManager() : m_pBlocks{},
m_pCharactor(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CBlockManager::~CBlockManager()
{
	Uninit();
}
//=========================================================
// ブロック生成関数
//=========================================================
CBlock* CBlockManager::CreateManager
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale,
	const char* pModelName
)
{
	// インスタンス生成
	CBlock* pNewBlock = CBlock::Create(pos, rot, scale, pModelName);

	// 配列に追加
	if (pNewBlock) m_pBlocks.push_back(pNewBlock);

	return pNewBlock;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBlockManager::Init(void)
{
	// 配列のクリア
	m_pBlocks.clear();

	// ファイル読み込み
	Load();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBlockManager::Uninit(void)
{
	// 配列のクリア
	m_pBlocks.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CBlockManager::Update(void)
{
	// カメラ取得
	const auto& Camera = CManager::GetInstance()->GetCamera();

	// 判定式生成
	for (auto Blocks : m_pBlocks)
	{
		// カメラの透過条件に入っているかチェック
		if (Camera->CollisionTorayBlock(Blocks))
		{
			// 対象モデルの透明度を設定する
			Blocks->SetCol(D3DCOLORVALUE(1.0f, 1.0f, 1.0f, 0.3f));
			Blocks->SetZEneble(true);
		}
		else
		{
			// デフォルトのカラーを使う
			Blocks->SetCol(); 
			Blocks->SetZEneble(false);
		}
	}
}
//=========================================================
// json読み込み
//=========================================================
HRESULT CBlockManager::Load(void)
{
	// jsonファイル読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(FILE_NAME);

	return S_OK;
}