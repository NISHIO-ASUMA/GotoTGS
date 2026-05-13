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
	void SetAlphaUI(const bool bUse);

	// 情報取得処理
	inline CDeskworkUI* GetDeskUI(const int nIdx) const { return m_pDeskUI[nIdx]; }
	inline int GetNowIdx(void) const { return m_nNowIdx; }

	// 生成処理
	static CDeskworkUIManager* Create(const D3DXVECTOR3& pos);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 200.0f;	// 横幅の増加分
		static constexpr float VALUE_TEXU = 0.25f;		// テクスチャのUV座標
		static constexpr int UI_NUM  = 3;				// UIの数
		static constexpr float UI_WIDTH = 100.0f;		// UIの横幅
		static constexpr float UI_HEIGHT = 100.0f;		// UIの縦幅
		static constexpr int TIME_COOL = 60;			// クールタイム
	};

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 座標
	CDeskworkUI* m_pDeskUI[Config::UI_NUM];		// UIのポインタ
	int m_nNowIdx;								// 現在選択している番号
	int m_nCountTime;							// タイマーカウント
	bool m_bTime;								// クールタイムが始まっているかどうか

};