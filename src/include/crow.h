//=========================================================
//
// カラス処理 [ crow.h ]
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
// カラスのオブジェクトクラスを定義
//*********************************************************
class CCrowBird : public CNoMoveCharactor
{
public:

	CCrowBird(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CCrowBird();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// カラス生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CCrowBird* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const int &nMotionType
	);

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
	int m_nMotionType;	// モーションの種類
};