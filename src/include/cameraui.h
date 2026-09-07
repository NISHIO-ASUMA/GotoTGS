//=========================================================
//
// カメラアニメーション時に出すUI [ cameraui.h ]
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
#include "object2D.h"

//*********************************************************
// カメラアニメーション時に出すUIクラスを定義
//*********************************************************
class CCameraUI : public CObject2D
{
public:

	//****************************
	// 移動の種類列挙型
	//****************************
	enum MOVEMENT_STATE
	{
		MOVEMENT_NONE,
		MOVEMENT_START, // 中央へ移動中
		MOVEMENT_STOP,  // 画面内で停止待機中
		MOVEMENT_END	// 画面外へ退場中
	};

public:

	CCameraUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CCameraUI();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	void SetMoveType(const int nType) { m_Movement = static_cast<MOVEMENT_STATE>(nType); }
	void SetTypeDir(const int nDirType) { m_nDirType = nDirType; }
	void SetInterval(const int nInterval) { m_nLifeInterval = nInterval; }

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <param name="nMaxInterval">最大生存時間</param>
	/// <param name="nMoveStartSetNumber">初期の移動方向の種類</param>
	/// <param name="nDirType">方向の種類</param>
	/// <returns></returns>
	static CCameraUI* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth,
		const float& fHeight,
		const char* Filename,
		const int& nMaxInterval,
		const int& nMoveStartSetNumber,
		const int& nDirType
	);

private:
	MOVEMENT_STATE m_Movement;	// アニメーション状態
	D3DXVECTOR3 m_TargetPos;	// 目標停止座標
	int m_nLifeInterval;		// 停止待機時間
	int m_nDirType;				// 移動方向タイプ[0: 下から上へ, 1: 上から下へ]
};