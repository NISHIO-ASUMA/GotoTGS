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
// ブロックオブジェクトクラスを定義
//*********************************************************
class CAfkTVPolygon
{
public:
	CAfkTVPolygon();
	~CAfkTVPolygon();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAfkTVPolygon* Create(void);

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTexture(const char* pTexName);

	// ゲッター
	bool GetAfkDisplay(void) { return m_bDisplay; }

	// インスタンス生成用関数
	static CAfkTVPolygon* Instance(void);

private:
	static CAfkTVPolygon* m_pInstance;	// シングルトン変数

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ

	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// カラー

	int m_nIdxTexture;					// テクスチャインデックス
	bool m_bDisplay;					// 表示するかの判定変数
};

