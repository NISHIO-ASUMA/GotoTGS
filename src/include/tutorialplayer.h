//=========================================================
//
// チュートリアルで動かすプレイヤー処理 [ turorialplayer.h ]
// Author: Asuma Nishio
// 
// TODO : このクラスではチュートリアルのプレイヤーの操作を書いてください
// 管理は任せる
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
class CTutorialPlayerStateBase;
class CStateMachine;
class CInputKeyboard;
class CJoyPad;

//*********************************************************
// チュートリアルプレイヤーオブジェクトクラスを定義
//*********************************************************
class CTutorialPlayer : public CMoveCharactor
{
public:

	CTutorialPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CTutorialPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);
	bool CollisionSphere(CSphereCollider* pOther);
	void ChangeState(CTutorialPlayerStateBase* pState, int nID);
	void MoveKeyboard(float speed);
	void MoveJoypad(float speed);
	void MoveCrossPadButton(float speed);
	void SetDefaultState(void);

	/// <summary>
	/// プレイヤー生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CTutorialPlayer* Create
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
	inline CBoxCollider* GetBoxCollider(void) override { return m_pBoxCollider.get(); }
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
		FOOD,
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
	void AddItemSet(const char* pModelName, CModel::PARTTYPE parttype, const D3DXVECTOR3& rot = VECTOR3_NULL, const D3DXVECTOR3& offpos = VECTOR3_NULL)
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
	void MathDeskRotation(void);

//****************************************************
private:

	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	bool m_bMove;										// 移動したかどうかの判定変数
	bool m_bAfkSmoke;									// たばこさぼりの判定変数
	bool m_bAfkTV;										// TVさぼりの判定変数
	bool m_bAfkMagazine;								// 漫画さぼりの判定変数
	bool m_bAfkGameCenter;								// ゲームセンターさぼりの判定変数
	int m_nCntAfk;										// さぼっているときのゲージの加算

//***********************************
private:
	std::unique_ptr<CModel> m_pSubItemModels; // 特定動作時に持たせるモデル
	D3DXVECTOR3 m_TvPrevPos;				  // テレビサボりの座標
	int m_nControlTypes;					  // 操作種類
	bool m_isPcWork;						  // デスクワークか
};