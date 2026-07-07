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
	/// <param name="MotionName">ファイル名</param>
	/// <param name="nMoveType">移動方向の種類</param>
	/// <param name="fMoveValue">移動距離</param>
	/// <returns></returns>
	static CWalkFriend* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot,
		const char * MotionName,
		const int& nMoveType,
		const float& fMoveValue = 30.0f,
		const float& fSpeed = 1.5f
	);

public:

	int GetMoveType(void) const { return m_MoveType; }

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		WALK,		// 歩く
		PLAY,		// ゲーセン遊び
		MAX
	};

	//***************************
	// 移動タイプ列挙型
	//***************************
	enum MOVING
	{
		NONE,			// 移動しない
		MOVE_FRONT_Z,	// -Z(手前)
		MOVE_BACK_Z,	// +Z(奥行き)
		MOVE_LEFT,		// 左方向
		MOVE_RIGHT,		// 右方向
		MOVE_MAX
	};

private:

	void SetFileName(const char* pFileName) { m_pFileName = pFileName; }
	void SetMoveType(const int& nType) { m_MoveType = nType; }
	void SetMoveValue(const float& fDistance) { m_fMoveDistance = fDistance; }
	void SetMoveSpeed(const float& fMoveSpeed) { m_fSpeed = fMoveSpeed; }
	void SetSavePos(const D3DXVECTOR3& pos) { m_SavePos = pos; }

	void SetMathTargetPos(const D3DXVECTOR3& startpos);
	void UpdateMovingType(void);

	void UpdateNormal(void);
	void UpdateMj(void);

private:

	std::unique_ptr<CBoxCollider> m_pBoxCollider;	// 矩形のコライダー

private:
	
	D3DXVECTOR3 m_TargetPos;	// 目的地
	D3DXVECTOR3 m_SavePos;		// 保存座標
	int m_MoveType;				// 移動方向の種類
	int m_nStopCount;			// 待機時間
	int m_nMoveTypeIndex;		// 移動のインデックス
	const char* m_pFileName;	// ファイルパス
	float m_fMoveDistance;		// 移動する距離
	float m_fSpeed;				// 移動速度
	bool m_isSet;				// セットポイントについたか
	bool m_isReturning;			// 戻りフラグ

private:

	const D3DXVECTOR3 BoxSize = { 50.0f,50.0f,50.0f };	// 固定の矩形サイズ
	static constexpr int MaxStop = 180;					// 移動停止カウント
};