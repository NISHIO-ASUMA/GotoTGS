//=========================================================
//
// カメラ処理 [ camera.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 前方宣言
//*********************************************************
class CInputMouse;
class CPlayer;
class CBlock;
class CAutoMaticDoor;
class CSideOpenDoor;

//*********************************************************
// カメラクラスを定義
//*********************************************************
class CCamera
{
public:

	//*************************
	// カメラモード列挙型
	//*************************
	enum MODE
	{
		MODE_NONE,
		MODE_THIRD,
		MODE_MOUSE,
		MODE_MAX
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

	//*************************
	// カメラ構造体を定義
	//*************************
	struct Camera
	{
		D3DXMATRIX mtxprojection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;			// ビューマトリックス
		D3DXVECTOR3 posV, posR;		// 視点,注視点座標
		D3DXVECTOR3 rot;			// 角度
		D3DXVECTOR3 vecU;			// 法線ベクトル
		D3DXVECTOR3 posRDest;		// 目的座標
		float fDistance;			// カメラの距離
		int nMode;					// カメラのモード
	};

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void MouseView(CInputMouse* pMouse);
	void FollowMouse(void);
	void FollowJoyPad(void);
	void WheelMouse(int nDelta);

	/// <summary>
	/// 
	/// </summary>
	/// <param name=""></param>
	void ThirdPersonView(void);

	/// <summary>
	/// 西尾追加 : リザルトのカメラ処理
	/// </summary>
	/// <param name=""></param>
	void ResultCamera(void);

	/// <summary>
	/// 西尾追加 : タイトルのカメラ
	/// </summary>
	/// <param name=""></param>
	void TitleCamera(void);

	/// <summary>
	/// 西尾追加 : 三人称カメラに関する設定処理
	/// </summary>
	/// <param name="target">追従対象の座標</param>
	void SetTargetPersonPos(const D3DXVECTOR3& target = VECTOR3_NULL) 
	{
		m_pThirdPersonPos = target;  // ターゲットの値を設定
		m_pThirdPersonPos.y = 50.0f; // 高さの値を少し上に
		m_pCamera.nMode = MODE_THIRD;// カメラモードを切り替え
		m_pCamera.fDistance = 130.0f;// 距離を設定
	}

	/// <summary>
	/// 西尾追加 : カメラから見たプレイヤーとの透過オブジェクト判定
	/// </summary>
	/// <param name="pPlayer">プレイヤーの外部ポインタ</param>
	/// <param name="pBlock">判定するブロックの外部ポインタ</param>
	/// <returns></returns>
	bool CollisionTorayBlock(CPlayer* pPlayer = nullptr,CBlock * pBlock = nullptr);

	/// <summary>
	/// 西尾追加 : カメラから見たプレイヤーとの透過オブジェクト判定
	/// </summary>
	/// <param name="pPlayer">プレイヤーの外部ポインタ</param>
	/// <param name="pBlock">判定するクラスの外部ポインタ</param>
	/// <returns></returns>
	bool CollisionTorayDoor(CPlayer* pPlayer = nullptr, CAutoMaticDoor* pDoor = nullptr);

	/// <summary>
	/// 西尾追加 : カメラから見たプレイヤーとの透過判定
	/// </summary>
	/// <param name="pPlayer">プレイヤーの外部ポインタ</param>
	/// <param name="pDoor">判定するクラスのポインタ</param>
	/// <returns></returns>
	bool CollisionToraySide(CPlayer* pPlayer = nullptr, CSideOpenDoor* pDoor = nullptr);

public:

	void SetMode(const int& nMode) { m_pCamera.nMode = nMode; }
	void SetRot(const D3DXVECTOR3 &rot) { m_pCamera.rot = rot; }
	void SetCameraMove(const bool& isMove) { m_isMove = isMove; }

	inline D3DXVECTOR3 GetRot(void) const { return m_pCamera.rot; }
	inline D3DXVECTOR3 GetPos(void) const { return m_pCamera.posV; }
	inline D3DXVECTOR3 GetPosR(void) const { return m_pCamera.posR; }
	inline D3DXMATRIX GetMtxProjection(void) const { return m_pCamera.mtxprojection; }
	inline D3DXMATRIX GetView(void) const { return m_pCamera.mtxView; }

	int GetMode(void) const { return m_pCamera.nMode; }

private:

	Camera ClearDefault(void);	// クリア用関数

private:
	Camera m_pCamera;				// カメラ構造体変数
	D3DXVECTOR3 m_pThirdPersonPos;	// 三人称座標
	int m_nControlTypes;			// 操作種類
	bool m_isMove;					// カメラ動かせるかどうかのフラグ
};