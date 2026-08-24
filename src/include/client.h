//=========================================================
//
// 外のクライアントキャラクター処理 [ client.h ]
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
#include "nomovecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// 外のクライアントキャラクターオブジェクトクラスを定義
//*********************************************************
class CClient : public CNoMoveCharactor
{
public:

	CClient(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CClient();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void ActionSet(void);
	void Reset(void);

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="pOther">相手のコライダー</param>
	/// <returns></returns>
	bool Collision(CSphereCollider* pOther);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CClient* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot
	);

	/// <summary>
	/// コライダー取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

public:

	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		ACTION,
		MAX
	};

private:

	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
	int m_nChangeTime;									// モーション切り替えカウント変数
	bool m_isChange;									// モーション切り替えフラグ
};