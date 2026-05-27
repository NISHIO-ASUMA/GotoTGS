//========================================================
//
// 歩いている同僚の処理 [ walkingfriend.h ]
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
#include "movecharactor.h"

//*********************************************************
// 歩く同僚キャラクタークラスを定義
//*********************************************************
class CWalkFriend : public CMoveCharactor
{
public:

	CWalkFriend(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CWalkFriend();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name= "MotionName">スクリプト名</param>
	/// <returns></returns>
	static CWalkFriend* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const char * MotionName);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		ACTION,		// 歩く
		MAX
	};

private:

	void SetFileName(const char* pFileName) { m_pFileName = pFileName; }

private:

	bool m_isSet;			 // セットポイントについたか
	const char* m_pFileName; // ファイルパス
};