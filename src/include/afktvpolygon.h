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
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CAfkTVPolygon : public CObject
{
public:
	CAfkTVPolygon(int nPriority = static_cast<int>(CObject::PRIORITY::BASENUMBER));
	~CAfkTVPolygon();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAfkTVPolygon* Create(void);

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetTexture(const char* pTexName);
	void Flash(const int nMaxFlashTime, const int Digittime);

	// ゲッター
	bool GetAfkDisplay(void) { return m_bDisplay; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ

	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// カラー

	int m_nIdxTexture;					// テクスチャインデックス
	int m_nTimeCnt;						// カウント用変数
	int m_FlashCount;					// 点滅間隔
	bool m_bDisplay;					// 表示するかの判定変数
};

