//=========================================================
//
// イベントUI処理 [ eventUi.h ]
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
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CEventcutin;
class CAlert;

//*********************************************************
// イベントUIに対するクラスを定義
//*********************************************************
class CEventUI:public CObject
{
public:

	CEventUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CEventUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CEventUI* Create(void);

	// 情報取得処理
	CEventcutin* GetEventcutin(void) { return m_pEventcutin; }
	CAlert* GetAlert(void) { return m_pAlert; }

private:

	// メンバ変数
	CEventcutin* m_pEventcutin;
	CAlert* m_pAlert;
};