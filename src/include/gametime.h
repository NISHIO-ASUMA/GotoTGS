//=========================================================
//
// ゲームのタイマー処理 [ gametime.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;

//*********************************************************
// ゲームのタイマーに対するクラスを定義
//*********************************************************
class CGametime :public CObject
{
public:

	CGametime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CGametime();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CGametime* Create(const D3DXVECTOR3& pos,float fWidth, float fHeight);

private:

	// 定数構造体宣言
	struct Config
	{
		static constexpr int TIMEDIGIT = 4;					// 表示桁数
		static constexpr int TIMELIMIT = 180;				// 制限時間
		static constexpr int UNIT = 60;						// 単位
		static constexpr int DIGITNUM = 10;					// 分割桁数
		static constexpr float POS_Y = 50.0f;				// Y座標値
		static constexpr float POS_WIDTH_VALUE = 2.0f;		// ずらす横幅乗算
		static constexpr const char* TEXNAME = "time.png";	// テクスチャ名
	};

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	float m_fWidth;								// 横幅
	float m_fHeight;							// 縦幅
	int m_NowTime;								// 残り時間
	int m_nCountFream;							// フレーム数
	CNumber* m_pNumber[Config::TIMEDIGIT];		// 桁数
};