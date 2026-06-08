//=========================================================
//
// 警告表示処理 [ alert.h ]
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
// 警告表示に対するクラスを定義
//*********************************************************
class CAlert :public CAnimationObject2D
{
public:

	// 警告表示の構造体
	struct Alert
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		D3DXVECTOR2 tex;
		float fWidth;
		float fHeight;
		bool isLoop;
		bool bUse;
	};

	CAlert(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CAlert();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CAlert* Create(const Alert& alert);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr int MAXFREAM = 180;					// 最大フレーム数
		static constexpr const char* TEXNAME = "alert000.png";	// テクスチャ名
	};
};