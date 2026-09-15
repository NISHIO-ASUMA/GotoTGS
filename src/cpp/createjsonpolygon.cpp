//===================================================================
//
// 3dポリゴンをJsonから生成する処理 [ createjsonpolygon.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonpolygon.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "jsonconverter.h"
#include "polygonmanager.h"

//===================================================================
// 生成関数
//===================================================================
void CJsonCreatePolygon::Create(const json& Jsondata)
{
	// 種類情報の設定
	int nGroup = CJsonConverter::ConverterInt(Jsondata, "group");

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 大きさの設定
	D3DXVECTOR2 size = CJsonConverter::ConverterVec2(Jsondata, "size");

	// 実際の生成
	CPolygonManager::GetInstance()->AddPolygon(nGroup,pos, rot, size);
}