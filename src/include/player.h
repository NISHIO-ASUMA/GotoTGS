//=========================================================
//
// プレイヤー処理 [ player.cpp ]
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
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;
class CPlayerStateBase;
class CStateMachine;
class CInputKeyboard;
class CJoyPad;

//*********************************************************
// プレイヤーオブジェクトクラスを定義
//*********************************************************
class CPlayer : public CMoveCharactor
{
public:

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);
	bool CollisionSphere(CSphereCollider* pOther);
	void ChangeState(CPlayerStateBase* pState, int nID);
	void MoveKeyboard(float speed);
	void MoveJoypad(float speed);

	/// <summary>
	/// プレイヤー生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPlayer* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot
	);
	
	bool GetMoveCheck(void) { return m_bMove; }
	bool GetAfkSmoke(void) { return m_bAfkSmoke; }
	bool GetAfkTV(void) { return m_bAfkTV; }
	bool GetAfkMagazine(void) { return m_bAfkMagazine; }
	bool GetAfkGameCenter(void) { return m_bAfkGameCenter; }

	inline D3DXVECTOR3 GetPrevPos(void) const { return m_TvPrevPos; }
	inline CBoxCollider* GetBoxCollider(void) { return m_pBoxCollider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

	void SetPrevPos(const D3DXVECTOR3& pos) { m_TvPrevPos = pos; }
public:

	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		MOVE,
		SMOKE,
		TV,
		MAGAZINE,
		GAME,
		MAX
	};

	//*************************
	// 操作メニュータイプ
	//*************************
	enum CONTROLTYPE
	{
		CONTROLTYPE_NONE,
		CONTROLTYPE_KEY,
		CONTROLTYPE_PAD,
		CONTROLTYPE_MAX
	};

//***********************************
// 西尾追加
public:

	/// <summary>
	/// 特定動作時にアイテムを追加する関数
	/// </summary>
	/// <param name="pModelName"></param>
	/// <param name="parttype"></param>
	void AddItemSet(const char* pModelName, CModel::PARTTYPE parttype,const D3DXVECTOR3& rot = VECTOR3_NULL,const D3DXVECTOR3& offpos = VECTOR3_NULL)
	{
		// nullじゃなかったら破棄する
		if (m_pSubItemModels) DeleteItem();

		// 親モデルを取得する
		CModel* pParent = GetPartModel(parttype);
		if (!pParent) return;

		// アイテムモデル生成
		m_pSubItemModels.reset(CModel::Create(VECTOR3_NULL, VECTOR3_NULL, pModelName, false));
		if (m_pSubItemModels)
		{
			// 親にくっつける
			m_pSubItemModels->SetParent(pParent);
		}

		// オフセットを設定する
		m_pSubItemModels->OffSetPos(offpos);
		m_pSubItemModels->OffSetRot(rot);

		// アウトラインせってい
		m_pSubItemModels->SetUseOutLine(true);
	}

	/// <summary>
	/// 手に持っている物を破棄する
	/// </summary>
	/// <param name=""></param>
	void DeleteItem(void)
	{
		if (m_pSubItemModels)
		{
			// 終了処理
			m_pSubItemModels->Uninit();

			// ポインタのクリア
			m_pSubItemModels.reset();
		}
	}

	void UpdateBlockCollision(D3DXVECTOR3 pos);
	void UpdateAutoDoorCollision(D3DXVECTOR3 pos);
	void UpdateSideDoorCollision(D3DXVECTOR3 pos, CInputKeyboard* key, CJoyPad* pad);

	void MathTVRotation(void);

private:

	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	bool m_bMove;										// 移動したかどうかの判定変数
	bool m_bAfkSmoke;									// たばこさぼりの判定変数
	bool m_bAfkTV;										// TVさぼりの判定変数
	bool m_bAfkMagazine;								// 漫画さぼりの判定変数
	bool m_bAfkGameCenter;								// ゲームセンターさぼりの判定変数

//***********************************
// 西尾追加
private:
	std::unique_ptr<CModel> m_pSubItemModels; // 特定動作時に持たせるモデル
	D3DXVECTOR3 m_TvPrevPos;				  // テレビサボりの座標
	int m_nControlTypes;					  // 操作種類
};