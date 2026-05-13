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
class CDeskwork;			// Misaki
class CWorldWallManager;

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
	CPlayer* GetPlayer(void) const { return m_pPlayer; } // [ Asuma add ]

	CDeskwork* GetDesk(void) const { return m_pDeskwork; }	// Misaki
	CWorldWallManager* GetWorldWall(void)const { return m_pWorldWallManager.get(); }

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

	CScore* m_pScore;			// スコアクラスのポインタ
	CGametime* m_pTimer;		// タイマークラスのポインタ
	CDeskwork* m_pDeskwork;		// タスクのポインタ Misaki
	CPlayer* m_pPlayer;			// プレイヤーのクラスポインタ [ Asuma add ]

	std::unique_ptr<CBlockManager>m_pBlocks;				// ブロックマネージャークラスのポインタ
	std::unique_ptr<CWorldWallManager>m_pWorldWallManager;	// 世界の壁管理クラスのポインタ
};