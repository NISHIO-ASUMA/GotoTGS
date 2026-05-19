//=========================================================
//
// 動かないキャラクタークラス処理 [ nomovecharactor.h ]
// Author: Asuma Nishio
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
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"
#include "model.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CShadowStencil;
class CMotion;

//*********************************************************
// 動かないキャラクタークラスを定義
//*********************************************************
class CNoMoveCharactor : public CObject
{
public:

	CNoMoveCharactor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CNoMoveCharactor();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	void MotionLoad(const char* pScriptName, int nDestMotions, const bool isShadow);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_scale = scale; }
	inline void SetOldPos(const D3DXVECTOR3& posOld) { m_posOld = posOld; }
	inline void SetUseStencil(const bool isUse = false) { m_isStencilUse = isUse; }
	inline void SetUseOutLine(const bool isUse = false) { m_isOutLine = isUse; }
	inline void SetOutLineSize(const float& fSize = 0.44f) { m_fOutLineSize = fSize; }
	inline void SetOutLineColor(const D3DXVECTOR4& color = D3DXVECTOR4(0.65f, 1.0f, 0.2f, 1.0f)) { m_OutLineColor = color; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetOldPos(void) const { return m_posOld; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXMATRIX GetWorldMtx(void) { return m_mtxworld; }

	/// <summary>
	/// モーションの生ポインタを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	CMotion* GetMotion(void) { return m_pMotion.get(); }

private:

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_posOld;	// 1フレーム前の座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_rotDest;  // 目的角
	D3DXVECTOR3 m_scale;	// 拡大率
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス
	D3DXVECTOR4 m_OutLineColor; // アウトラインの色

	std::unique_ptr<CMotion>m_pMotion;	// モーションクラスのユニークポインタ
	std::vector<CModel*>m_pModel;		// モデルクラスの動的配列
	CShadowStencil* m_pShadowS;			// ステンシルシャドウクラスのポインタ

	bool m_isStencilUse;				// ステンシルを使うかどうか
	bool m_isOutLine;					// アウトラインを入れるかどうか
	float m_fOutLineSize;				// アウトラインのサイズ
};