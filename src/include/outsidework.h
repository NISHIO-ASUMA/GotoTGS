//=========================================================
//
// 外出タスク処理 [ outsidework.h ]
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
#include "deskworkUImanager.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CParticle;

//*********************************************************
// 外出タスククラスを定義
//*********************************************************
class COutsideWork :public CDeskworkUIManager
{
public:

	COutsideWork();
	~COutsideWork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static COutsideWork* Create(const bool& bUse = false);

private:
	// メンバ変数
	CParticle* m_pParticle;	// パーティクルのポインタ
	int m_nParticleTime;	// パーティクルを表示する時間
};