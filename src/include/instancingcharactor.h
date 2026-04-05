//===================================================================
//
// インスタンシングキャラクタークラス [ instancingcharactor.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once

//*******************************************************************
// システムインクルードファイル
//*******************************************************************
#include <memory>
#include <vector>

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "object.h"

//*******************************************************************
// 前方宣言
//*******************************************************************
class CShadowStencil;
class CBlackBoard;
class CNode;
class CInstanceModel;
class CMotionInstancing;

//*******************************************************************
// インスタンシング適用のキャラクタークラスを定義
//*******************************************************************
class CInstancingCharactor : public CObject
{
public:

	CInstancingCharactor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CInstancingCharactor();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void MotionLoad(const char* pScriptName, int nDestMotions, const bool isShadow);
	void UpdatePosition(void);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetOldPos(const D3DXVECTOR3& posOld) { m_posOld = posOld; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetRotDest(const D3DXVECTOR3& rotdest) { m_rotDest = rotdest; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_scale = scale; }
	inline void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	inline void SetMoveValue(const float fValue) { m_fMoveValue = fValue; }
	inline void SetType(TYPE Type) { m_type = Type; }
	inline void SetUseStencil(const bool isUse) { m_isStencilUse = isUse; }
	inline void SetUseOutLine(const bool isUse = false) { m_isOutLine = isUse; }
	inline void SetUseInstancing(const bool isInstance = false) { m_isInstancing = isInstance; }
	inline 	void SetCharactor(TYPE type) { m_type = type; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetOldPos(void) const { return m_posOld; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXVECTOR3 GetMove(void) const { return m_move; }
	inline D3DXVECTOR3 GetRotDest(void) const { return m_rotDest; }
	inline CMotionInstancing* GetMotion(void) { return m_pMotion.get(); }

protected:
	CBlackBoard* m_pBlackBoard = nullptr;	// 共通ブラックボード
	CNode* m_pBehaviorTree = nullptr;		// ツリーノード
private:

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_posOld;	// 過去座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_rotDest;	// 目的角
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_scale;	// 拡大率
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス
	TYPE m_type;			// 種類

	std::unique_ptr<CMotionInstancing>m_pMotion;	// モーションポインタ
	std::vector<CInstanceModel*>m_pModel;			// モデルクラスの動的配列
	CShadowStencil* m_pShadowS;						// ステンシルシャドウクラスのポインタ

	float m_fMoveValue;		// 移動加算値
	bool m_isStencilUse;	// ステンシルシャドウを使うか
	bool m_isOutLine;		// アウトラインを使うかどうか
	bool m_isInstancing;	// 適用させるかどうか
};