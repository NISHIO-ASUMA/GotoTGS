//=========================================================
//
// 過労死シーンのオブジェクト管理 [ overworkresultobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "overworkresultobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "camera.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "ui.h"
#include "meshfield.h"
#include "light.h"

// #include "overworkman.h"

//=========================================================
// コンストラクタ
//=========================================================
COverWorkResultObject::COverWorkResultObject() : m_pBlock(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
COverWorkResultObject::~COverWorkResultObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT COverWorkResultObject::Init(void)
{
	// ライトの値チェンジ
	CManager::GetInstance()->GetLight()->SetLight();

	// カメラ設定
	CManager::GetInstance()->GetCamera()->Init();

	// ロゴuiを生成
	CUi::Create(CENTERPOS, 0, 450.0f, 130.0f, "toomuchwork.png");
	
	// メッシュフィールド
	CMeshField::Create(VECTOR3_NULL, 1000.0f, 1000.0f,30,30, "data/TEXTURE/field00.jpg");

	// 倒れているキャラクターを生成


	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void COverWorkResultObject::Uninit(void)
{
	m_pBlock.reset();
}