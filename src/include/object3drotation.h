//=========================================================
//
// 回転適用する3Dポリゴンクラス [ object3drotation.h ]
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
// 回転適用する3Dポリゴンクラスを定義
//*********************************************************
class CObjectRotation3D : public CObject
{
public:

	CObjectRotation3D(int nPriority = static_cast<int>(CObject::PRIORITY::BASENUMBER));
	~CObjectRotation3D();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetTexture(const char* pTexName);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">回転</param>
	/// <returns></returns>
	static CObjectRotation3D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetCol(D3DXCOLOR col = COLOR_WHITE) { m_col = col; }
	inline void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }
	inline void SetIsDraw(bool isDraw) { m_isDraw = isDraw; }

public:
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ

private:
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;		// 座標情報
	D3DXVECTOR3 m_rot;		// 角度情報
	D3DXCOLOR m_col;		// 色情報

private:
	int m_nIdxTexture;		// テクスチャインデックス
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
	bool m_isDraw;			// 描画フラグ
};