//=========================================================
//
// カットイン処理 [ eventcutin.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルード
//*********************************************************
#include "animationobject2d.h"

//*********************************************************
// カットインに対するクラスを定義
//*********************************************************
class CEventcutin :public CAnimationObject2D
{
public:

	// カットイン表示の構造体
	struct EventCutin
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		D3DXVECTOR2 tex;
		float fWidth;
		float fHeight;
		bool isLoop;
		bool bUse;
	};

	CEventcutin(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CEventcutin();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CEventcutin* Create(const EventCutin& eventcutin);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr int MOVE_FREAM = 60;					// 移動時のフレーム数
		static constexpr int STOP_FREAM = 120;					// 表示するフレーム数
		static constexpr const char* TEXNAME = "cutin000.png";	// テクスチャ名
	};

	// メンバ変数
	D3DXVECTOR3 m_Offsetpos;	// 初期位置
	int m_nStopCount;			// 表示するフレームのカウント
	bool m_bTurn;				// 折り返したかどうか
};