//=========================================================
//
// 捕まった時のキャスト達 [ resultlazycast.h ]
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
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "nomovecharactor.h"

//*********************************************************
// キャラクターオブジェクトクラスを定義
//*********************************************************
class CResultCast : public CNoMoveCharactor
{
public:

	CResultCast(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CResultCast();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="nMotionType">モーション番号</param>
	/// <param name="Path">ファイルパス</param>
	/// <returns></returns>
	static CResultCast* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const int& nMotionType,
		const std::string& Path
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
	int m_nMotionType;			// モーションの種類
	std::string m_MotionPath;	// モーションパス
};