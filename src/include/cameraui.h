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
	enum MOVEMENT
	{
		MOVEMENT_NONE,	// 初期化状態
		MOVEMENT_START,	// 開始状態
		MOVEMENT_STOP,	// 停止状態(継続状態)
		MOVEMENT_END,	// 終了に向かう状態
		MOVEMENT_MAX
	};

public:

	CCameraUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CCameraUI();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <param name="nMaxInterval">最大生存時間</param>
	/// <param name="nMoveStartSetNumber">初期の移動方向の種類</param>
	/// <returns></returns>
	static CCameraUI* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth,
		const float& fHeight,
		const char* Filename,
		const int& nMaxInterval,
		const int& nMoveStartSetNumber
	);

private:
	int m_nLifeInterval;	// 生存時間
	int m_Movement;			// 移動の種類
};