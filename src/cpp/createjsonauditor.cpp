//===================================================================
//
// AuditorクラスをJsonから生成する処理 [ createjsonauditor.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonauditor.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "jsonconverter.h"
#include "auditormanager.h"
#include "jsonmanager.h"
#include "auditor.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateAuditor::Create(const json& Jsondata)
{
	// マネージャーからjsonマネージャーを取得
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	auto AuditorManager = jsonManager->GetAuditorManager();

	// ポインタが設定されていなかったら
	if (!AuditorManager) return;

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 種類情報の設定
	int nType = CJsonConverter::ConverterInt(Jsondata, "movetype");

	// 変換する
	CAuditor::MOVE_POINTTYPE type = static_cast<CAuditor::MOVE_POINTTYPE>(nType);

	// 実際のキャラクター生成
	AuditorManager->CreateInManager(pos, rot, type);
}