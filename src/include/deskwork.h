//=========================================================
//
// タスク処理 [ deskwork.h ]
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
#include "object2D.h"

//*********************************************************
// タスククラスを定義
//*********************************************************
class CDeskwork :public CObject2D
{
public:

	CDeskwork(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CDeskwork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CDeskwork* Create(const D3DXVECTOR3& pos);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float WIDTH = SCREEN_WIDTH * 0.6f;					// 横幅
		static constexpr float HEIGHT = SCREEN_HEIGHT * 0.6f;				// 縦幅
		static constexpr const char* TEXNAME = "deskwork_background.jpg";	// テクスチャ名
		static constexpr int UI_WORK = 3;									// 表示するUIの数
		static constexpr float VALUE_WIDTH = Config::WIDTH * 0.125;			// 横幅の増加する幅

	};

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 座標
	float m_fWIdth;		// 横幅
	float m_fHeight;	// 縦幅

};