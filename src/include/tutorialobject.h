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
	static CTutorialObject* GetInstance(void);

private:

	CTutorialObject();

	static CTutorialObject* m_pInstance;			// シングルトン変数

private:
	std::unique_ptr<CBlockManager>m_pBlockManager;	// ブロック管理クラス
	CTutorialPlayer* m_pTutoPlayer;					// チュートリアルプレイヤー
};