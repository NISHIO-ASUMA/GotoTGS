//=========================================================
//
// 柱メッシュ処理 [ meshpiler.h ]
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
// 柱状メッシュクラスを定義
//*********************************************************
class CMeshPiler : public CObject
{
public:

	//**************************
	// 構造体宣言
	//**************************
	struct MeshPiler
	{
		int nTexIdx;		// テクスチャインデックス
		int nNumPrimitive;	// プリミティブ数
		int nNumIdx;		// インデックス数
		int nNumAllVtx;		// 全体頂点数
		int nNumDigitX;		// 分割数
		int nNumDigitZ;		// 分割数
		int nLife;			// 寿命
		int nLifeFrame;		// 生存時間（フレーム数）
		float fMoveSpeed;	// 移動速度
		float fNowHeight;	// 現在の高さ
	};

	CMeshPiler(int nPriority = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshPiler();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetTexture(void);
	bool Collision(D3DXVECTOR3* CollisionPos);

	D3DXVECTOR3 GetPos(void) { return m_pos; }

	static CMeshPiler* Create(D3DXVECTOR3 pos);

private:

	LPDIRECT3DINDEXBUFFER9 m_pIdx; // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// 頂点バッファ

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	MeshPiler m_MeshPiler;	// 構造体変数
};