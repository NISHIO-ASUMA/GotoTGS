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
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void) = 0;

	// 設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetUse(const bool& bUse) { m_bUse = bUse; }
	inline void AddPCTask(void) { m_nPCTaskNum++; }
	inline void AddCOPYTask(void) { m_nCOPYTaskNum++; }
	void AddDOCUMENTTask(void);
	inline void MinusPCTask(void) { m_nPCTaskNum--; }
	inline void MinusCOPYTask(void) { m_nCOPYTaskNum--; }
	inline void MinusDOCUMENTTask(void) { m_nDOCUMENTTaskNum--; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	bool GetUse(void) const { return m_bUse; }
	CUi* GetClearUI(void) { return m_pClearUI; }
	static int GetPCTaskNum(void) { return m_nPCTaskNum; }
	static int GetCOPYTaskNum(void) { return m_nCOPYTaskNum; }
	static int GetDOCUMENTTaskNum(void) { return m_nDOCUMENTTaskNum; }

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 座標
	bool m_bUse;			// 使用しているかどうか	
	CUi* m_pClearUI;		// クリアUIのポインタ

	// 静的メンバ変数
	static int m_nPCTaskNum;		// こなしたPCタスクの数
	static int m_nCOPYTaskNum;		// こなしたコピー機タスクの数
	static int m_nDOCUMENTTaskNum;	// こなした書類タスクの数

};
