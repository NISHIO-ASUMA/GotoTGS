//=========================================================
//
// AFKTVPOLYGON処理 [ afktvpolygon.h ]
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
// インクルードファイル
//*********************************************************
#include "object3D.h"

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CAfkTVPolygon :public CObject3D
{
public:
	CAfkTVPolygon();
	~CAfkTVPolygon();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAfkTVPolygon* Create(void);

	bool GetAfkDisplay(void) { return m_bDisplay; }

	// インスタンス生成用関数
	static CAfkTVPolygon* Instance(void);

private:
	static CAfkTVPolygon* m_pInstance;	// シングルトン変数

	D3DXVECTOR3 m_pos;		// 位置
	bool m_bDisplay;		// 表示するかの判定変数
};

