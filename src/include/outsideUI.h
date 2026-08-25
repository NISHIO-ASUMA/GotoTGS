//=========================================================
//
// 外出タスク用UI処理 [ outsideUI.h ]
// Author: Takahashi Misaki
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
// インクルードファイル
//*********************************************************
#include "tutorialui.h"

//*********************************************************
// オブジェクトクラスを定義
//*********************************************************
class COutsideUI : public CTutorialUI
{
public:

	COutsideUI(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~COutsideUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成処理</param>
	/// <param name="rot">角度</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pTexName">テクスチャファイル名</param>
	/// <returns></returns>
	static COutsideUI* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const char* pTexName
	);
};

