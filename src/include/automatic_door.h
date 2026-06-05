//=========================================================
//
// 自動ドアの処理 [ automatic_door.h ]
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
// 自動ドアクラスを定義
//*********************************************************
class CAutoMaticDoor : public CObjectX
{
public:

	//************************
	// 移動方向の列挙型
	//************************
	enum MOVETYPE
	{
		MOVETYPE_LEFT, // 左方向
		MOVETYPE_RIGHT,// 右方向
		MOVETYPE_MAX
	};

	//**************************
	// ドアの状態管理用の列挙型
	//**************************
	enum STATE
	{
		STATE_CLOSE_WAIT,	// 閉じ状態
		STATE_OPENING,		// 開き中
		STATE_OPEN_WAIT,	// 開ききった状態
		STATE_CLOSING,		// 閉じ中
		STATE_MAX
	};

	CAutoMaticDoor(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CAutoMaticDoor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);

	/// <summary>
	/// 矩形コライダー取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline CBoxCollider* GetCollider(void) { return m_pCollider.get(); }

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
	static CAutoMaticDoor* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName,
		const MOVETYPE& nType
	);

public:

	inline void SetType(const MOVETYPE& nType) { m_nMoveType = nType; }
	inline void SetZEneble(const bool& isenable) { m_isZTestEneble = isenable; }

	void OpenDoorFlag(void);

private:

	std::unique_ptr<CBoxCollider> m_pCollider;	// 矩形のコライダー
	MOVETYPE m_nMoveType;						// 移動方向の種類インデックス
	float m_fMoveSpeed;							// 移動速度
	bool m_isZTestEneble;						// 透明化判定を受けるかどうか
	D3DXVECTOR3 m_Size;							// サイズ
private:

	STATE m_nState;				// 現在の状態
	D3DXVECTOR3 m_vBasePos;		// 初期位置
	int m_nOpenTimer;			// 開きっぱなしにする時間
};