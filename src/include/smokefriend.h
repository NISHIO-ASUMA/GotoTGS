//========================================================
//
// 煙草吸っている同僚の処理 [ smokefriend.h ]
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
// 煙草吸いの同僚キャラクタークラスを定義
//*********************************************************
class CSmokeFriend : public CNoMoveCharactor
{
public:

	CSmokeFriend(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CSmokeFriend();

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
	static CSmokeFriend* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		ACTION,		// 煙草吸い
		MAX
	};

private:

	void RandomChange(void);

private:
	int m_nChangeCount; // モーション切り替えカウント
	int m_nNextChangeCount; // 次の変更までのカウント
};