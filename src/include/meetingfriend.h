//=========================================================
//
// 会議室同僚処理 [ meetingfriend.h ]
// Author: Shouya Chikada
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
#include <string>

//*********************************************************
// 会議室同僚オブジェクトクラスを定義
//*********************************************************
class CMeetingFriend : public CNoMoveCharactor
{
public:

	CMeetingFriend(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CMeetingFriend();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetMotionPath(const std::string& MotionPath) { m_MotionPath = MotionPath; }

	/// <summary>
	/// 同僚生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CMeetingFriend* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const std::string& Path
	);

public:

	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		GESTURE1,
		GESTURE2,
		GESTURE3,
		MAX
	};

private:

	int m_nChangeTime;									// モーション切り替えカウント変数
	std::string m_MotionPath;							// モーションパス情報
};
