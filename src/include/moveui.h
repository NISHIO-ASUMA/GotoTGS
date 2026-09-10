//========================================================
//
// 動くUI処理 [ moveui.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// 動きがあるUIクラスの定義
//*********************************************************
class CMoveUi : public CObject2D
{
public:

	//********************
	// 状態の列挙型
	//********************
	enum STATE
	{
		MOVE_DOWN,		// 下方向へ移動
		WAIT,			// 目的座標で一時停止
		MOVE_RIGHT,		// 右方向へ画面外移動
	};

	CMoveUi(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CMoveUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetState(STATE state) { m_State = state; }

	/// <summary>
	/// 有効化処理
	/// </summary>
	/// <param name="isUseFlag">判別フラグ</param>
	void SetIsUse(bool isUseFlag) 
	{
		m_isUse = isUseFlag;
		if (m_isUse)
		{
			ResetState();
		}
	}

	void ResetState(void);

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="nFlashFrame">点滅間隔</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <returns></returns>
	static CMoveUi* Create
	(
		const D3DXVECTOR3& pos,
		const int& nFlashFrame,
		const float& fWidth,
		const float& fHeight,
		const char* Filename
	);

private:
	void SetParameter(const D3DXVECTOR3& pos);

private:
	bool m_isUse;				// 表示するかどうか
	D3DXVECTOR3 m_StartPos;		// 初期座標
	D3DXVECTOR3 m_DestPos;		// 下移動時の目標座標
	STATE m_State;				// 現在の状態
};