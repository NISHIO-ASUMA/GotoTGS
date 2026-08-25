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
#include "worldUIcollision.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CPCDeskwork;
class CCOPYDeskwork;
class CDOCUMENTDeskwork;
class COutsideWork;
class CPlayer;

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
	void SetTaskType(const CWorldUICollision::TYPE& TaskType, const bool& bUse = false);
	void SetPtrForOutPlayer(CPlayer* pPlayer = nullptr) { m_pPlayer = pPlayer; }

	// 情報取得処理
	CPCDeskwork* GetPCDeskUI(void) const { return m_pPCDeskUI; }
	CCOPYDeskwork* GetCOPYDeskUI(void) const { return m_pCOPYDeskUI; }
	CDOCUMENTDeskwork* GetDOCUMENTDesk(void)const { return m_pDOCUMENTDesk; }
	COutsideWork* GetOutsideDesk(void)const { return m_pOutsideDesk; }
	static CWorldUICollision::TYPE GetTaskType(void) { return m_TaskType; }

	// 生成処理
	static CDeskwork* Create(const D3DXVECTOR3& pos, CPlayer* pPlayer);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		// PCタスク用
		static constexpr float PC_VALUE_Y = 50.0f;								// Y軸の位置の増加量
		static constexpr float PC_WIDTH = 700.0f;								// 横幅
		static constexpr float PC_HEIGHT = 300.0f;								// 縦幅
		static constexpr const char* PC_TEXNAME = "deskwork_background2.png";	// PCタスクのテクスチャ名

		// コピー機タスク用
		static constexpr float COPY_VALUE_Y = 50.0f;						// Y軸の位置の増加量
		static constexpr float COPY_WIDTH = 600.0f;							// 横幅
		static constexpr float COPY_HEIGHT = 300.0f;						// 縦幅
		static constexpr const char* COPY_TEXNAME = "copymachine.png";		// コピー機タスクのテクスチャ名
		static constexpr const char* CANCEL_TEXNAME = "nocopymachine.png";	// コピー機を使用できない時用のテクスチャ名

	};

	// メンバ変数
	CPCDeskwork* m_pPCDeskUI;				// PCタスクUIのポインタ
	CCOPYDeskwork* m_pCOPYDeskUI;			// コピー機タスクUIのポインタ
	CDOCUMENTDeskwork* m_pDOCUMENTDesk;		// 書類タスクのポインタ
	COutsideWork* m_pOutsideDesk;			// 外出タスクのポインタ
	CPlayer* m_pPlayer;						// 入れ子の入れ子

	// 静的メンバ変数
	static CWorldUICollision::TYPE m_TaskType;	// タスクの種類
};