//===================================================================
//
// 動かないキャラクターをJsonから生成する処理 [ createjsoncharactor.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonnomove.h"

//*******************************************************************
// システムインクルード
//*******************************************************************
#include <string>

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "jsonconverter.h"
#include "friend.h"
#include "smokefriend.h"
#include "meetingfriend.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateNoMove::Create(const JsonNoMove::json& Jsondata)
{
	// タグの一致を見る
	std::string type = Jsondata["CharactorName"];

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "Pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "Rot");

	// パス情報の設定
	std::string str = CJsonConverter::ConverterPath(Jsondata, "MotionName");

	if (type == "Friend")
	{// 同僚生成
		CFriend::Create(pos, rot, str);
	}
	else if (type == "SmokeFirend")
	{// 煙草同僚生成
		CSmokeFriend::Create(pos, rot,str.c_str());
	}
	else if (type == "MeetingFriend")
	{// 会議室キャラクター
		CMeetingFriend::Create(pos,rot,str.c_str());
	}
}