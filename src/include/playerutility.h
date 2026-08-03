//=========================================================
//
// プレイヤーを使うのに必要な宣言をまとめたファイル [ playerutility.h ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "PCdeskwork.h"				// 髙橋追加
#include "COPYdeskwork.h"			// 髙橋追加
#include "DOCUMENTdeskwork.h"		// 髙橋追加
#include "vigilanceUImanager.h"		// 髙橋追加
#include "vigilanceUImanager.h"		// 髙橋追加
#include "gaugeneedle.h"			// 髙橋追加
#include "progressgauge.h"
#include "worldUIcollision.h"
#include "collisionsphere.h"		// 近田追加
#include "afkmanager.h"				// 近田追加
#include "afktv.h"					// 近田追加
#include "afksmoke.h"				// 近田追加
#include "afkmagazine.h"			// 近田追加
#include "afkeating.h"				// 近田追加
#include "afkgamecenter.h"			// 近田追加
#include "titleuimanager.h"			// 近田追加
#include "score.h"					// 近田追加
#include "afkbench.h"				// 近田追加
#include "automaticdoormanager.h"	// 西尾追加
#include "automatic_door.h"			// 西尾追加
#include "autodoor_collision.h"		// 西尾追加
#include "slideopendoormanager.h"	// 西尾追加
#include "sideopendoor.h"			// 西尾追加
#include "sideopendoorcollision.h"	// 西尾追加
#include "enemymanager.h"			// 西尾追加
#include "enemy.h"					// 西尾追加
#include "scorepop.h"				// 西尾追加
#include "afk2dui.h"
#include "afkbattime.h"

//*********************************************************
// 基本の情報を保持している名前空間
//*********************************************************
namespace player
{
	constexpr float fSpeed = 5.0f;			// プレイヤーの移動スピード
	constexpr float fInput = 0.0001f;		// 移動処理に使うキーが入力されてるか比較する用の変数
	constexpr float fJoyInput = 2000.0f;	// ジョイパッドのスティック入力の値

	/// <summary>
	/// 西尾追加
	/// </summary>
	constexpr float BoxSize = 50.0f;									  // 矩形サイズ
	constexpr float SphereSize = 60.0f;									  // 球形サイズ
	constexpr const char* SCRIPT = "data/MOTION/Player/PlayerMotion.txt"; // テキストファイル
	constexpr int AFK_COOL_TIME = 600;									  // 再起動できるクールタイム時間
	constexpr int DEATH_LIMIT_FRAME = 360;								  // 死亡時間タイマーの上限値
	const D3DXVECTOR3 UI_POS_VALUE = { 0.0f,80.0f,0.0f };
};

//*********************************************************
// 定数名前空間 ( タスク関連時 )
//*********************************************************
namespace Player_Info
{
	// テレビ関係
	const D3DXVECTOR3 TV_CHARACTORPOS = { -244.0f, 10.5f,277.0f };
	const D3DXVECTOR3 TV_DESTPOS = { -248.0f, 12.0f, 362.0f };

	// デスクワーク関係
	const D3DXVECTOR3 DESK_DESTPOS = { -63.0f, 16.0f, 185.0f };		// デスクワーク中の座標
	const D3DXVECTOR3 DESK_RETURNPOS = { -100.0f, 0.0f, 175.0f };	// タスク終了時にもどる固定座標
};

//*********************************************************
// 定数名前空間 ( ベンチ関連 )
//*********************************************************
namespace Player_Bench
{
	// ベンチ関係
	const D3DXVECTOR3 STATION_CHARACTORPOS = { 792.4f, 18.0f, 1303.6f };	// 駅のベンチの座標
	const D3DXVECTOR3 STATION_DESTPOS = { 792.4f, 18.0f, 1403.6f };			// 駅のベンチの目的座標

	const D3DXVECTOR3 GAMECENTER_CHARACTORPOS = { 1461.1f, 18.0f, 317.0f };	// ゲームセンターのベンチの座標
	const D3DXVECTOR3 GAMECENTER_DESTPOS = { 1461.1f, 18.0f, 417.0f };		// ゲームセンターのベンチの目的座標

	const D3DXVECTOR3 IZAKAYA_CHARACTORPOS = { 1527.5f, 18.0f, -962.4f };	// 居酒屋のベンチの座標
	const D3DXVECTOR3 IZAKAYA_DESTPOS = { 1527.5f, 18.0f, -1062.4f };		// 居酒屋のベンチの目的座標

	const D3DXVECTOR3 OFFICE_CHARACTORPOS = { 734.4f, 18.0f, -468.0f };		// オフィス横のベンチの座標
	const D3DXVECTOR3 OFFICE_DESTPOS = { 834.4f, 18.0f, -468.0f };			// オフィス横のベンチの目的座標

	constexpr int BENCH_MAX = 4;
};