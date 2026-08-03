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
	inline void SetTime(const bool& bTime) { m_bTime = bTime; }
	inline void SetCountTime(const int& nCountTime) { m_nCountTime = nCountTime; }
	inline void SetCan(const bool& bCan = false) { m_bCan = bCan; }
	static void SetTutorial(const bool& bTutorial = false) { m_bTutorial = bTutorial; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline CUi* GetClearUI(void) { return m_pClearUI; }
	inline bool GetUse(void) const { return m_bUse; }
	inline bool GetTime(void) const { return m_bTime; }
	inline int GetCountTime(void) const { return m_nCountTime; }
	inline bool GetCan(void)const { return m_bCan; }

	// 静的メンバ関数
	static void AddPCTask(void) { m_nPCTaskNum++; }
	static void AddCOPYTask(void) { m_nCOPYTaskNum++; }
	static void AddDOCUMENTTask(void);
	static void AddOutsideTask(void);
	static int GetPCTaskNum(void) { return m_nPCTaskNum; }
	static int GetCOPYTaskNum(void) { return m_nCOPYTaskNum; }
	static int GetDOCUMENTTaskNum(void) { return m_nDOCUMENTTaskNum; }
	static int GetOutsideTaskNum(void) { return m_nOutsideTaskNum; }
	static bool GetTutorial(void) { return m_bTutorial; }
	static void MinusPCTask(void) { m_nPCTaskNum--; }
	static void MinusCOPYTask(void) { m_nCOPYTaskNum--; }
	static void MinusDOCUMENTTask(void) { m_nDOCUMENTTaskNum--; }
	static void MinusOutsideTask(void) { m_nOutsideTaskNum--; }

public: // 西尾追加
	//**********************
	// コントロールタイプ
	//**********************
	enum CONTROL
	{
		CONTROL_NONE,
		CONTROL_KEY,	// キーマウ
		CONTROL_PAD,	// パッド
		CONTROL_MAX
	};


private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 座標
	CUi* m_pClearUI;		// クリアUIのポインタ
	bool m_bUse;			// 使用しているかどうか	
	bool m_bTime;			// クールタイム中かどうか
	int m_nCountTime;		// 時間をカウントする変数
	bool m_bCan;			// 使えるかどうか

	// 静的メンバ変数
	static int m_nPCTaskNum;		// こなしたPCタスクの数
	static int m_nCOPYTaskNum;		// こなしたコピー機タスクの数
	static int m_nDOCUMENTTaskNum;	// こなした書類タスクの数
	static int m_nOutsideTaskNum;	// こなした外出タスクの数
	static bool m_bTutorial;		// チュートリアルをこなしたかどうか
};