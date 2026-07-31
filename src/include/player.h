//=========================================================
//
// プレイヤー処理 [ player.h ]
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
class CInput;
class CAfkCoolTimeUi;	// サボりがクールタイム中の時にだすuiクラス

//*********************************************************
// プレイヤーオブジェクトクラスを定義
//*********************************************************
class CPlayer : public CMoveCharactor
{
public:
	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,		// 通常
		MOVE,			// 移動
		SMOKE,			// 煙草
		TV,				// テレビ
		MAGAZINE,		// 雑誌
		GAME,			// ゲーセン
		FOOD,			// 飲食
		BENCH,			// ベンチ
		MAX
	};

	//*************************
	// 操作メニュータイプ
	//*************************
	enum CONTROLTYPE
	{
		CONTROLTYPE_NONE,	// デフォルト
		CONTROLTYPE_KEY,	// キーマウ
		CONTROLTYPE_PAD,	// ゲームパッド
		CONTROLTYPE_MAX
	};

	//*************************
	// さぼりの種類
	//*************************
	enum AFKTYPE
	{
		AFKTYPE_NONE,
		AFKTYPE_SMOKE,		// 煙草
		AFKTYPE_TV,			// テレビ
		AFKTYPE_MAGAZINE,	// 雑誌読み
		AFKTYPE_GAMECENTER,	// ゲームセンター
		AFKTYPE_EATING,		// 食事
		AFKTYPE_BENCH,		// 公園のベンチ
		AFKTYPE_MAX
	};

public:

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void ChangeState(CPlayerStateBase* pState, int nID);
	void MoveKeyboard(float speed);
	void MoveJoypad(float speed);
	void MoveCrossPadButton(float speed);

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
	
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	//************************************************
	// さぼっている時の判定用変数(ゲッター)
	//************************************************
	bool GetMoveCheck(void) { return m_bMove; }
	bool GetAfkSmoke(void) { return m_bAfkSmoke; }
	bool GetAfkTV(void) { return m_bAfkTV; }
	bool GetAfkMagazine(void) { return m_bAfkMagazine; }
	bool GetAfkGameCenter(void) { return m_bAfkGameCenter; }
	bool GetAfkEating(void) { return m_bAfkEating; }
	bool GetAfkBench(int nIdx) { return m_bAfkBench[nIdx]; }

	//************************************************
	// さぼりのカウント用関数
	//************************************************
	void AddSmoke(void) { m_nSmoke++; }
	void AddTV(void) { m_nTV++; }
	void AddMagazine(void) { m_nMagazine++; }
	void AddGameCenter(void) { m_nGameCenter++; }
	void AddEating(void) { m_nEating++; }
	void AddBench(void) { m_nBench++; }

	//************************************************
	// さぼりのカウント用関数(ゲッター)
	//************************************************
	int GetSmoke(void) { return m_nSmoke; }
	int GetTV(void) { return m_nTV; }
	int GetMagazine(void) { return m_nMagazine; }
	int GetGameCenter(void) { return m_nGameCenter; }
	int GetEating(void) { return m_nEating; }
	int GetBench(void) { return m_nBench; }

	void SetAfk(AFKTYPE AfkType, bool bInput);
	void AfkScore(void);

	inline CBoxCollider* GetBoxCollider(void) override { return m_pBoxCollider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

	// 西尾追加
	void SetCatchEnemy(const bool isFlags) { m_isCatchEnemy = isFlags; }
	bool GetIsCatch(void) { return m_isCatchEnemy; }
	bool GetIsLazy(void) { return m_isEnableLazy; }
	bool IsTaskWorking(void) const;

public:

	/// <summary>
	/// 特定動作時にアイテムを追加する関数
	/// </summary>
	/// <param name="pModelName">生成したいモデルの名前</param>
	/// <param name="parttype">どのパーツか</param>
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

		// アウトラインセット
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

	/// <summary>
	/// 矩形のコライダーとの当たり判定
	/// </summary>
	/// <param name="pOther">判定先のコライダー</param>
	/// <param name="OutPos">出力する座標</param>
	/// <returns></returns>
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);

	/// <summary>
	/// 球形のコライダーとの当たり判定
	/// </summary>
	/// <param name="pOther">判定先の球コライダー</param>
	/// <returns></returns>
	bool CollisionSphere(CSphereCollider* pOther);

	/// <summary>
	/// マップに配置されているブロック等の当たり判定
	/// </summary>
	/// <param name="pos">現在座標</param>
	void UpdateBlockCollision(D3DXVECTOR3 pos);

	/// <summary>
	/// 自動ドアとの当たり判定処理
	/// </summary>
	/// <param name="pos">現在座標</param>
	void UpdateAutoDoorCollision(D3DXVECTOR3 pos);

	/// <summary>
	/// 両開きのドアとの当たり判定処理
	/// </summary>
	/// <param name="pos">現在座標</param>
	/// <param name="key">キーボードクラスのポインタ</param>
	/// <param name="pad">ゲームパッドクラスのポインタ</param>
	void UpdateSideDoorCollision(D3DXVECTOR3 pos, CInputKeyboard* key, CJoyPad* pad);

	/// <summary>
	/// テレビの方を向く関数
	/// </summary>
	/// <param name=""></param>
	void MathTVRotation(void);

	/// <summary>
	/// PC作業の方向に向く関数
	/// </summary>
	/// <param name=""></param>
	void MathDeskRotation(void);

	/// <summary>
	/// ベンチで寝るときの向き調整関数
	/// </summary>
	/// <param name=""></param>
	void MathBenchRotation(void);

	/// <summary>
	/// afkのui関連回りの起動関数
	/// </summary>
	/// <param name=""></param>
	void UpdateAfkUiState(void);

private:

	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	D3DXVECTOR3 m_posOld;								// 過去の位置
	int m_nCntAfk;										// さぼっているときのゲージの加算
	int m_nTimeScore;									// スコアタイム用変数
	int m_nAddScore;									// 加算スコア
	int m_nScoreCnt;									// スコア用カウント
	bool m_bMove;										// 移動したかどうかの判定変数
	bool m_bAfkSmoke;									// たばこさぼりの判定変数
	bool m_bAfkTV;										// TVさぼりの判定変数
	bool m_bAfkMagazine;								// 漫画さぼりの判定変数
	bool m_bAfkGameCenter;								// ゲームセンターさぼりの判定変数
	bool m_bAfkEating;									// 飲食スペースでさぼっているときの判定変数
	bool m_bAfkBench[4];								// ベンチでさぼっている時の判定変数

	//************************************************
	// さぼりのカウント用変数
	//************************************************
	int m_nSmoke;										// たばこを吸った回数カウント用変数
	int m_nTV;											// TVを見た回数カウント用変数
	int m_nMagazine;									// 雑誌を読んだ回数カウント用変数
	int m_nGameCenter;									// ゲームセンターで遊んだ回数カウント用変数
	int m_nEating;										// 飲食スペースでお菓子を食べた回数カウント用変数
	int m_nBench;										// ベンチで寝た回数カウント用変数

	//************************************************
	// さぼりのクールタイム用
	//************************************************
	int m_nAfkCoolTime[AFKTYPE_MAX];	// 通常のサボり用クールタイムタイマー
	int m_nCoolTimeBench[4];			// ベンチ専用のクールタイムタイマー

private:
	std::unique_ptr<CModel> m_pSubItemModels; // 特定動作時に持たせるモデル
	int m_nControlTypes;					  // 操作種類
	bool m_isPcWork;						  // デスクワークかどうか

	// 西尾追加
	bool m_isCatchEnemy;					  // 上司に捕まってしまった
	bool m_isEnableLazy;					  // サボり中の判定

	CAfkCoolTimeUi* m_pCoolTimeUi[AFKTYPE_MAX];					 // 通常サボり用
	CAfkCoolTimeUi* m_pCoolTimeUiBench[4]; // ベンチ4箇所用
};