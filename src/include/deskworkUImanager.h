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
// 前方宣言
//*********************************************************
class CUi;

//*********************************************************
// タスクUIマネージャークラスを定義
//*********************************************************
class CDeskworkUIManager
{
public:

	//**********************
	// UIの種類
	//**********************
	enum TYPE
	{
		TYPE_PC = 0,
		TYPE_COPY,
		TYPE_MAX
	};

	CDeskworkUIManager();
	virtual ~CDeskworkUIManager();

	virtual HRESULT Init(void);
	virtual void Uninit(void) = 0;
	virtual void Update(void);
	virtual void Draw(void) = 0;

	// 設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetUse(const bool& bUse) { m_bUse = bUse; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	bool GetUse(void) const { return m_bUse; }
	CUi* GetClearUI(void) { return m_pClearUI; }

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 座標
	bool m_bUse;			// 使用しているかどうか	
	bool m_bClear;			// クリアしているかどうか	
	CUi* m_pClearUI;		// クリアUIのポインタ
};
