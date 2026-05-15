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
// 前方宣言
//*********************************************************
class CPCDeskwork;

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

	// 設定処理
	inline void SetAlpha(const bool bUse);

	// 情報取得処理
	CPCDeskwork* GetPCDeskUI(void) const { return m_pPCDeskUI; }

	// 生成処理
	static CDeskwork* Create(const D3DXVECTOR3& pos);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float WIDTH = 500.0f;								// 横幅
		static constexpr float HEIGHT = 200.0f;								// 縦幅
		static constexpr const char* TEXNAME = "deskwork_background.jpg";	// テクスチャ名
	};

	// メンバ変数
	CPCDeskwork* m_pPCDeskUI;	// PCタスクUIのポインタ
	//CDeskworkUIManager* m_pDeskUIManager;	// コピータスクUIのポインタ
};