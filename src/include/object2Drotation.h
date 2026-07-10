//=========================================================
//
// 回転する2Dオブジェクト基底クラス処理 [ object2Drotation.h ]
// Author: Misaki Takahashi
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
// ローテーションオブジェクト2Dクラスを定義
//*********************************************************
class CObject2DRotation : public CObject
{
public:

	CObject2DRotation(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CObject2DRotation();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexture(const char* pRegisterName);

	// 設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetPivot(const D3DXVECTOR3& pivot) { m_pivot = pivot; }
	inline void SetAngle(const float& fAngle) { m_fAngle = fAngle; }
	inline void SetCol(const D3DXCOLOR col) { m_col = col; }
	inline void SetSize(const D3DXVECTOR2 size) { m_fWidth = size.x, m_fHeight = size.y; }
	inline void SetWidth(float fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(float fHeight) { m_fWidth = fHeight; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetRPos(void) const { return m_pivot; }
	inline float GetAngle(void) const { return m_fAngle; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

	// 生成処理
	static CObject2DRotation* Create(const D3DXVECTOR3 pos,
		const D3DXVECTOR3 Rpos,
		const float fAngle,
		const D3DXCOLOR col,
		const D3DXVECTOR2 size,
		const char* pRegisterName);

private:

	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファのポインタ

	D3DXVECTOR3 m_pos;		// 座標情報
	D3DXVECTOR3 m_pivot;	// 回転の基準点
	D3DXCOLOR m_col;		// 色情報

	float m_fAngle;			// 角度
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
	int m_nAnchorType;		// 基準座標設定
	int m_nIdxTexture;		// テクスチャインデックス
};
