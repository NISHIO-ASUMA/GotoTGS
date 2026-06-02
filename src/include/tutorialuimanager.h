//=========================================================
//
// TutorialUIManager処理 [ tutorialuimanager.h ]
// Author: Shouya Chikada
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
// ブロックオブジェクトクラスを定義
//*********************************************************
class CTutorialUIManager
{
public:

	CTutorialUIManager();
	~CTutorialUIManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// インスタンス生成用関数
	static CTutorialUIManager* Instance(void);

private:
	static CTutorialUIManager* m_pInstance;	// シングルトン変数

};

