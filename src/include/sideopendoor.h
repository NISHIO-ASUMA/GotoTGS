//=========================================================
//
// キー入力で開く両開きドアの処理 [ sideopendoor.h ]
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
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "objectX.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// キー入力で開く両開きドアクラスを定義
//*********************************************************
class CSideOpenDoor : public CObjectX
{
public:

	//************************
	// 移動方向の列挙型
	//************************
	enum OPENTYPE
	{
		OPENTYPE_LEFT, // 左方向
		OPENTYPE_RIGHT,// 右方向
		OPENTYPE_MAX
	};

	//**************************
	// ドアの状態管理用の列挙型
	//**************************
	enum STATE
	{
		STATE_CLOSE_WAIT,	// 閉じ状態
		STATE_OPENING,		// 開き中
		STATE_OPENWAIT,		// 開いている時間の状態
		STATE_RETURN,		// 初期の状態にもどる
		STATE_CLOSE,
		STATE_MAX
	};

	CSideOpenDoor(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CSideOpenDoor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <param name="">現在のドアの状態</param>
	/// <returns></returns>
	STATE GetState(void) const { return m_nState; }

	/// <summary>
	/// 矩形コライダー取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline CBoxCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// 大きさ取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline D3DXVECTOR3 GetSize(void) const { return m_Size; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <param name="nType">移動方向タイプ</param>
	/// <returns></returns>
	static CSideOpenDoor* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName,
		const OPENTYPE& nType
	);
	
public:

	inline void SetType(const OPENTYPE& nType) { m_nOpenType = nType; }
	inline void SetZEneble(const bool& isenable) { m_isZTestEneble = isenable; }

	void RotationDoorFlag(void);

private:

	std::unique_ptr<CBoxCollider> m_pCollider;	// 矩形のコライダー
	OPENTYPE m_nOpenType;						// 移動方向の種類インデックス
	D3DXVECTOR3 m_Size;							// サイズ
	STATE m_nState;		// 現在の状態

private:

	float m_fAngle;		// 現在の開閉角度 (ラジアン)
	float m_fOffsetX;	// 回転軸のオフセット量（ドアの幅の半分）
	bool m_isZTestEneble;	// 透明化判定を受けるかどうか
	int m_nCloseTime;		// 開いていたら閉じるカウント用変数
	int m_nReturnCount;
};