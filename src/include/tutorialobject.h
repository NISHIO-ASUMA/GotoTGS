//=========================================================
//
// チュートリアルのオブジェクト管理 [ tutorialobject.h ]
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
class CTutorialPlayer;
class CTutorialLines;		// Misaki
class CDeskwork;			// Misaki

//*********************************************************
// チュートリアルで使うオブジェクトを管理するクラスを定義
//*********************************************************
class CTutorialObject
{
public:

	~CTutorialObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	CTutorialPlayer* GetTutorilaPlayer(void) { return m_pTutoPlayer; }

	inline CBlockManager* GetBlockManager(void) const { return m_pBlockManager.get(); }
	CTutorialLines* GetTutoriallines(void)const { return m_plines; }	// Misaki
	CDeskwork* GetDesk(void)const { return m_pDeskwork; }				// Misaki
	static CTutorialObject* GetInstance(void);

private:

	CTutorialObject();

	static CTutorialObject* m_pInstance;			// シングルトン変数

private:
	std::unique_ptr<CBlockManager>m_pBlockManager;	// ブロック管理クラス
	CTutorialPlayer* m_pTutoPlayer;					// チュートリアルプレイヤー
	CTutorialLines* m_plines;						// セリフのポインタ Misaki
	CDeskwork* m_pDeskwork;							// タスクのポインタ	Misaki

};