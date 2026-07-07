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

public:

	//*************************
	// 操作メニュータイプ
	//*************************
	enum CONTROLTYPE
	{
		CONTROLTYPE_NONE,
		CONTROLTYPE_KEY,
		CONTROLTYPE_PAD,
		CONTROLTYPE_MAX
	};

private:
	static CTutorialUIManager* m_pInstance;	// シングルトン変数
	int m_nControlTypes;					// 操作種類
};

