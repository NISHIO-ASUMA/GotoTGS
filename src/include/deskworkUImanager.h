//=========================================================
//
// タスクUIマネージャー処理 [ deskworkUIManager.h ]
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
#include "deskworkUI.h"

//*********************************************************
// タスクUIマネージャークラスを定義
//*********************************************************
class CDeskworkUIManager
{
public:

	CDeskworkUIManager();
	~CDeskworkUIManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }

	// 生成処理
	static CDeskworkUIManager* Create(const D3DXVECTOR3& pos);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 200.0f;				// 横幅の増加分
		static constexpr float VALUE_TEXU = 0.25f;					// テクスチャのUV座標
		static constexpr int UI_NUM  = 3;							// UIの数
		static constexpr float UI_WIDTH = 100.0f;					// UIの横幅
		static constexpr float UI_HEIGHT = 100.0f;					// UIの縦幅
	};

	// メンバ変数
	D3DXVECTOR3 m_pos;								// 座標
	CDeskworkUI* m_pDeskworkUI[Config::UI_NUM];		// UIのポインタ
};