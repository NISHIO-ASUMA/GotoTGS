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
// 前方宣言
//*********************************************************
class CBoxCollider;

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

	bool Collision(CBoxCollider* pOther,D3DXVECTOR3* PushPos);

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
		PLAY,		// ゲーセン遊び
		MAX
	};

	//***************************
	// 移動タイプ列挙型
	//***************************
	enum MOVING
	{
		NONE,			// 移動しない
		MOVE_LEFT,		// 左方向
		MOVE_RIGHT,		// 右方向
		MOVE_FRONT_Z,	// -Z(手前)
		MOVE_BACK_Z,	// +Z(奥行き)
		MOVE_MAX
	};

private:

	void SetFileName(const char* pFileName) { m_pFileName = pFileName; }

private:

	std::unique_ptr<CBoxCollider> m_pBoxCollider;	// 矩形のコライダー

private:

	bool m_isSet;			 // セットポイントについたか
	const char* m_pFileName; // ファイルパス

private:
	const D3DXVECTOR3 BoxSize = { 50.0f,50.0f,50.0f }; // 固定の矩形サイズ
};