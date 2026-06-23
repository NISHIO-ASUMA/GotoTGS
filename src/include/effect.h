//=========================================================
//
// エフェクト処理[effect.h]
// Author : Kaiti Aoto
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
// エフェクトクラス
//*********************************************************
class CEffect:public CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_THUNDER,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CEffect(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CEffect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, TYPE type = TYPE_NONE);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetType(TYPE type) { m_Type = type; }

private:
	void SetSize(float fRadius);
	void SetColor(D3DXCOLOR col);

	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// バッファ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 角度
	D3DXVECTOR3 m_move;						// 移動量
	D3DXCOLOR m_col;						// 色
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	TYPE m_Type;							// エフェクトタイプの変数
	float m_fRadius;						// 半径
	bool m_bUse;							// 使用しているか
	int m_nIdxTex;							// テクスチャのインデックス番号
	int m_nLife;							// 寿命
};