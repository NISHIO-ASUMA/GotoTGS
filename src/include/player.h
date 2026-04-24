//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Shouya Chikada
// 
// NOTE : 近田君へ
//		  球形コライダーを追加すること
//		  定数などがあればstatic constexpr で追加すること 構造体にまとめてもいい
//		  cppのほうの実装は一度仮組出来たら教えろください
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;

//*********************************************************
// プレイヤーオブジェクトクラスを定義
//*********************************************************
class CPlayer : public CMoveCharactor
{
public:
	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);

	/// <summary>
	/// プレイヤー生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPlayer* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot
	);

	inline CBoxCollider* GetBoxCollider(void) { return m_pBoxCollider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

public:
	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		MOVE,
		MAX
	};

private:
	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー

};