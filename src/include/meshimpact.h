//=========================================================
//
// メッシュインパクト処理 [ meshimpact.h ]
// Author: Asuma Nishio
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
// 衝撃波のクラス定義
//*********************************************************
class CMeshImpact : public CObject
{
public:

	//**************************
	// 構造体宣言
	//**************************
	struct MeshImpact
	{
		float fInRadius;	// 内径
		float fOutRadius;	// 外径
		int nLife;			// 寿命
		float DecAlpha;		// α値減少
		float fSpeed;		// 拡大速度
	};

	CMeshImpact(int nPriority = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshImpact();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(D3DXVECTOR3* pPos);

	void SetCol(D3DXCOLOR col) { m_col = col; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	static CMeshImpact* Create(D3DXVECTOR3 pos, int nLife, float fRadius, float fInRadius,float fSpeed);

private:

	D3DXVECTOR3 m_pos;	// 座標
	D3DXVECTOR3 m_rot;  // 角度
	D3DXCOLOR m_col;	// 色
	D3DXMATRIX m_mtxWorld; // ワールドマトリックス
	LPDIRECT3DINDEXBUFFER9 m_pIdx;	// インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx; // 頂点バッファ

	MeshImpact m_MeshImpact;	// 構造体変数
};