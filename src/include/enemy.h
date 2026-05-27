//========================================================
//
// 敵の処理 [ enemy.h ]
// Author: Asuma Nishio
// 
// NOTE : プレイヤーを疑っているモーションを追加する
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
class CSphereCollider;
class CBoxCollider;

//*********************************************************
// 敵キャラクタークラスを定義
//*********************************************************
class CEnemy : public CMoveCharactor
{
public:

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawEyeSight(void);
	bool CheckEyesight(const D3DXVECTOR3& TargetPos);
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CEnemy* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動
		MAX
	};

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 80.0f; // 球形範囲
		static constexpr float BOX_RANGE = 60.0f;	 // 矩形範囲
		static constexpr int DIVIDE = 16;			 // メッシュの分割数
	};

	//***************************
	// 扇形の当たり判定
	//***************************
	struct Eyesight
	{
		static constexpr float EYE_RADIUS = 100.0f;		// 視界の届く距離
		static constexpr float EYE_ANGLE = 45.0f;		// 視野角
		static constexpr float EYE_HEIGHT = 50.0f;		// 視界の高さ制限
	};

private:

	void UpdateMoveViewPoint(void);

private:

	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

	bool m_isCheckPoint;								// ポイントに到着したかどうか
	int m_nStopTime;									// 停止しているカウント
	int m_nTargetIdx;									// 向かう目的地のインデックス
};