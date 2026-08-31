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
class CBlockManager;		// Asuma
class CGametime;			// Asuma
class CScore;				// Asuma
class CAfk2DUI;				// Asuma
class CPlayer;				// Asuma
class CReceptionist;		// Asuma
class CBoss;				// Asuma

class CDeskwork;			// Misaki
class CProgressgauge;		// Misaki
class CEventUI;				// Misaki
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

	/// <summary>
	/// 西尾 : ゲームでボスが追加されるときに起こすもの
	/// </summary>
	/// <param name=""></param>
	void SetEventGameBoss(void);

//*********************************
// 西尾 : スコア関連の取得
	CScore* GetScore(void) const { return m_pScoreTask; }		// タスクのスコアの物
	CScore* GetScoreDitch(void) const { return m_pScoreDitch; } // サボりのスコアの物
	CScore* GetScoreAllGame(void) const { return m_pScoreAll; }	// 今現在ゲームで表示している物
//*********************************

	CGametime* GetTime(void) const { return m_pTimer; }
	CPlayer* GetPlayer(void) const { return m_pPlayer; }										// Asuma
	CReceptionist* GetReception(void) const { return m_pReception; }							// Asuma
	CAfk2DUI* GetAfk2DUIPointer(void) const { return m_pAfk2DUI; }								// Asuma
	CBoss* GetBoss(void) const { return m_pBoss; }												// Asuma

	CDeskwork* GetDesk(void) const { return m_pDeskwork; }										// Misaki
	CEventUI* GetEventUI(void) const { return m_pEventUI; }										// Misaki
	CProgressgauge* GetProgressgauge(void) const { return m_pProgressgauge; }					// Misaki
	CVigilanceUIManager* GetVigilanceUIManager(void)const { return m_pVigilanceUImanager; }		// Misaki

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CGameSceneObject* GetInstance(void);

private:

	CGameSceneObject();			// コンストラクタ
	void CreatePointer(void);	// 初期化処理関数分け

private:

	CScore* m_pScoreTask;							// スコアクラスのポインタ [ タスク分 ]
	CScore* m_pScoreDitch;							// スコアクラスのポインタ [ サボる分 ]
	CScore* m_pScoreAll;							// スコアクラスのポインタ [ 実際のゲームで表示するもの ]

private:
	CGametime* m_pTimer;							// タイマークラスのポインタ		Misaki
	CDeskwork* m_pDeskwork;							// タスクのポインタ				Misaki
	CProgressgauge* m_pProgressgauge;				// 進捗ゲージのポインタ			Misaki
	CEventUI* m_pEventUI;							// イベントUIのポインタ			Misaki
	CVigilanceUIManager* m_pVigilanceUImanager;		// 警戒ゲージのポインタ			Misaki

private:
	CPlayer* m_pPlayer;								// プレイヤーのクラスポインタ	[ Asuma add ]
	CReceptionist* m_pReception;					// 外に行く受付人				[ Asuma Add ]
	CAfk2DUI* m_pAfk2DUI;							// サボりの2DUI
	CBoss* m_pBoss;									// 社長キャラクター				[ Asuma Add ]

	std::unique_ptr<CBlockManager>m_pBlocks;		// ブロックマネージャークラスのポインタ
};