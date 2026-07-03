//===================================================================
//
// 敵をJsonから生成する処理 [ createjsonenemy.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonenemy.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "jsonconverter.h"
#include "enemymanager.h"
#include "jsonmanager.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateEnemy::Create(const json& Jsondata)
{
	// マネージャーからjsonマネージャーを取得
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	auto EnemyManager = jsonManager->GetEnemyManager();

	// ポインタが設定されていなかったら
	if (!EnemyManager) return;

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 実際のブロック生成
	EnemyManager->CreateManager(pos, rot);
}