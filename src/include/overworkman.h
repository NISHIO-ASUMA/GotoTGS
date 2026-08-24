//========================================================
//
// 過労死キャラクター処理 [ overworkman.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルード
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "nomovecharactor.h"

//*********************************************************
// 過労死キャラクタークラスを定義
//*********************************************************
class COverWorkMan : public CNoMoveCharactor
{
public:

	COverWorkMan(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~COverWorkMan();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static COverWorkMan* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,
		DEATH,	// 過労
		MAX
	};
private:
};