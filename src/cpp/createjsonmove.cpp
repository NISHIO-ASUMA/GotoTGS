//===================================================================
//
// 動くキャラクターをJsonから生成する処理 [ createjsonmove.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonmove.h"

//*******************************************************************
// システムインクルード
//*******************************************************************
#include <string>

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "jsonconverter.h"
#include "mobcharactormanager.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateMove::Create(const JsonMove::json& Jsondata)
{
	// タグの一致を見る
	std::string type = Jsondata["CharactorName"];

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "Pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "Rot");

	// パス情報の設定
	std::string str = CJsonConverter::ConverterPath(Jsondata, "MotionName");

	if (type == "Walking")
	{// 歩く同僚生成
		CMobCharactorManager::GetInstance()->CreateManager(pos, rot, str.c_str());
	}
}