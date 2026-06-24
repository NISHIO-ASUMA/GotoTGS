//=========================================================
//
// パーティクル処理[particle.h]
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
// パーティクルクラス
//*********************************************************
class CParticle:public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,	// 通常
		TYPE_SMOKE,		// 上昇煙
		TYPE_SPRAY,		// スプレー噴射
		TYPE_FLAME,		// 火炎放射
		TYPE_THUNDER,	// 落雷
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CParticle();
	~CParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	int GetLife(void) { return m_nLife; }
	bool GetUse(void) { return m_bUse; }

	// 静的メンバ関数
	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fRadius, TYPE type);
	static void SetTargetPos(D3DXVECTOR3 pos) { m_TargetPos = pos; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetType(TYPE type) { m_type = type; }
	void SetUse(bool bUse) { m_bUse = bUse; }

private:
	void None(void);				// 通常状態
	void Smoke(void);				// スモーク状態
	void Spray(void);				// スプレー状態
	void Flame(void);				// 火炎放射状態
	void Thunder(void);				// 雷状態

	// 静的メンバ変数
	static D3DXVECTOR3 m_TargetPos;	// 目標位置

	// メンバ変数
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot;				// 角度
	D3DXCOLOR m_col;				// 色
	TYPE m_type;					// 種類
	int m_nLife;					// 寿命
	int m_nCreateTime;				// 生成時間
	float m_fRadius;				// 半径
	bool m_bUse;					// 使用しているか

};