//=========================================================
//
// ゲームのオブジェクト管理処理 [ gamesceneobject.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CBlockManager;
class CPlayer;				// [ Asuma add ]
class CGametime;
class CScore;
class CWorldWallManager;
class CDeskwork;			// Misaki
class CProgressgauge;		// Misaki
class CEventUI;				// Misaki
class CTutorialLines;		// Misaki
class CVigilanceUIManager;	// Misaki

//*********************************************************
// ゲームシーンで使うオブジェクト管理クラスを定義
//*********************************************************
class CGameSceneObject
{
public:

	~CGameSceneObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CGametime* GetTime(void) const { return m_pTimer; }
	CScore* GetScore(void) const { return m_pScore; }
	CPlayer* GetPlayer(void) const { return m_pPlayer; }								// [ Asuma add ]
	CDeskwork* GetDesk(void) const { return m_pDeskwork; }								// Misaki
	CProgressgauge* GetProgressgauge(void) const { return m_pProgressgauge; }			// Misaki
	CEventUI* GetEventUI(void) const { return m_pEventUI; }								// Misaki
	CWorldWallManager* GetWorldWall(void)const { return m_pWorldWallManager.get(); }
	CTutorialLines* GetTutoriallines(void)const { return m_pTutoriallines; }			// Misaki

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CGameSceneObject* GetInstance(void);

private:

	static CGameSceneObject* m_pInstance;	// シングルトン変数

	CGameSceneObject();			// コンストラクタ
	void CreatePointer(void);	// 初期化処理関数分け

private:

	CScore* m_pScore;								// スコアクラスのポインタ
	CGametime* m_pTimer;							// タイマークラスのポインタ
	CDeskwork* m_pDeskwork;							// タスクのポインタ Misaki
	CProgressgauge* m_pProgressgauge;				// 進捗ゲージのポインタ Misaki
	CEventUI* m_pEventUI;							// イベントUIのポインタ Misaki
	CPlayer* m_pPlayer;								// プレイヤーのクラスポインタ [ Asuma add ]
	CTutorialLines* m_pTutoriallines;				// チュートリアルのセリフのポインタ Misaki
	CVigilanceUIManager* m_pVigilanceUImanager;		// 警戒ゲージのポインタ Misaki

	std::unique_ptr<CBlockManager>m_pBlocks;				// ブロックマネージャークラスのポインタ
	std::unique_ptr<CWorldWallManager>m_pWorldWallManager;	// 世界の壁管理クラスのポインタ
};