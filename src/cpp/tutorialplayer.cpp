//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialplayer.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "boxcollider.h"
#include "spherecollider.h"
#include "collisionbox.h"
#include "playerstatebase.h"
#include "playerstateneutral.h"
#include "statemachine.h"
#include "input.h"
#include "camera.h"
#include "blockmanager.h"
#include "jsonmanager.h"
#include "deskwork.h"
#include "gamesceneobject.h"
#include "debugproc.h"
#include "PCdeskwork.h"			
#include "COPYdeskwork.h"		
#include "DOCUMENTdeskwork.h"	
#include "worldUIcollision.h"
#include "collisionsphere.h"
#include "afkmanager.h"
#include "afktv.h"
#include "afksmoke.h"
#include "afkmagazine.h"
#include "automaticdoormanager.h" 
#include "automatic_door.h"		  
#include "autodoor_collision.h"	  
#include "outline.h"
#include "afkgamecenter.h"
#include "titleuimanager.h"
#include "progressgauge.h"
#include "slideopendoormanager.h"	
#include "sideopendoor.h"			
#include "sideopendoorcollision.h"	
#include "enemymanager.h"			
#include "enemy.h"					

//*********************************************************
// 名前空間
//*********************************************************
namespace TutorilaPlayer
{
	constexpr float fSpeed = 5.0f;			// プレイヤーの移動スピード
	constexpr float fInput = 0.0001f;		// 移動処理に使うキーが入力されてるか比較する用の変数
	constexpr float fJoyInput = 2000.0f;	// ジョイパッドのスティック入力の値

	/// <summary>
	/// 以下、西尾追加
	/// </summary>
	constexpr float BoxSize = 50.0f;									  // 矩形サイズ
	constexpr float SphereSize = 60.0f;									  // 球形サイズ
	constexpr const char* SCRIPT = "data/MOTION/Player/PlayerMotion.txt"; // テキストファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pSubItemModels(nullptr),
m_pMachine(nullptr),
m_bMove(false),
m_bAfkSmoke(false),
m_bAfkTV(false),
m_bAfkMagazine(false),
m_bAfkGameCenter(false),
m_isPcWork(false),
m_TvPrevPos(VECTOR3_NULL),
m_nControlTypes(CONTROLTYPE_NONE),
m_nCntAfk(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CTutorialPlayer::~CTutorialPlayer()
{

}
//=========================================================
// 生成処理
//=========================================================
CTutorialPlayer* CTutorialPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CTutorialPlayer* pPlayer = new CTutorialPlayer;
	if (pPlayer == nullptr)return nullptr;

	// オブジェクト設定
	pPlayer->SetPos(pos);
	pPlayer->SetRot(rot);
	pPlayer->SetUseOutLine(true);
	pPlayer->SetOutlineColor(D3DXVECTOR4(0.46f, 0.81f, 1.0f, 1.0f));

	// 初期化失敗時
	if (FAILED(pPlayer->Init()))return nullptr;

	return pPlayer;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialPlayer::Init(void)
{
	// 親クラスの初期化処理
	CMoveCharactor::Init();

	// モーション読み込み
	MotionLoad(TutorilaPlayer::SCRIPT, MAX, false);

	//// インスタンス生成
	//m_pMachine = new CStateMachine;

	//// 初期ステートをセット
	//ChangeState(new CTutorialPlayerStateNeutral(), CTutorialPlayerStateBase::ID_NEUTRAL);

	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot(); // オブジェクトの回転角度を取得

	// X, Y, Zの回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetOldPos(), D3DXVECTOR3(TutorilaPlayer::BoxSize, TutorilaPlayer::BoxSize, TutorilaPlayer::BoxSize), matRot);

	// スフィアコライダーの生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), TutorilaPlayer::SphereSize);

	// 操作の種類を設定する(パッドかキーマウかどうか)
	m_nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();

	// フラグの再初期化
	m_isPcWork = false;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialPlayer::Uninit(void)
{
	// ステートマシンの破棄
	if (m_pMachine)
	{
		m_pMachine->OnExit();
		delete m_pMachine;
		m_pMachine = nullptr;
	}

	// ボックスコライダーの破棄
	m_pBoxCollider.reset();

	// スフィアコライダーの破棄
	m_pSphereCollider.reset();

	// サブモデルの破棄
	m_pSubItemModels.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CTutorialPlayer::Update(void)
{
#if 1
	// タスクの情報を取得
	auto* pDesk = CGameSceneObject::GetInstance()->GetDesk();
#else

	//*********************************************************
	// ADD: 西尾 タスク中にキーが押されたら、タスクを閉じる
	//*********************************************************
	if (pDesk->GetTaskType() != CWorldUICollision::TYPE_NONE && pDesk->GetTaskType() != CWorldUICollision::TYPE_DOCUMENT)
	{
		// 入力フラグ
		bool isInputKey = false;

		// 操作の種類によって使える物を変化させる
		switch (m_nControlTypes)
		{
		case CTutorialPlayer::CONTROLTYPE_NONE:
			break;

		case CTutorialPlayer::CONTROLTYPE_KEY:

			if (Key->GetTrigger(DIK_F))
			{
				isInputKey = true;
			}
			break;

		case CTutorialPlayer::CONTROLTYPE_PAD:

			if (Pad->GetTrigger(CJoyPad::JOYKEY_START))
			{
				isInputKey = true;
			}
			break;

		default:
			break;
		}

		if (!isInputKey)
		{// 終了キーを押していない場合
			// モーション更新だけ挟んでreturnする
			CMoveCharactor::Update();
			return;
		}

		// ADD : 西尾 さっき行っていた作業が"デスクワーク"なら
		if (pDesk->GetTaskType() == CWorldUICollision::TYPE_PC)
		{
			// プレイヤーの座標を戻す
			SetPos(VECTOR3_NULL);

			// 角度を変更
			SetRot(VECTOR3_NULL);

			// フラグをリセットする
			m_isPcWork = false;
		}

		// 起動したタスクを非アクティブにする [add Misaki]
		pDesk->SetTaskType(pDesk->GetTaskType());

		// タスク中は移動や他の当たり判定をさせないためにリターン
		return;
	}

#endif

	// キー入力取得
	const auto& Key = CManager::GetInstance()->GetInputKeyboard();
	const auto& Pad = CManager::GetInstance()->GetJoyPad();

	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 oldpos = GetOldPos();

	if (m_nControlTypes == CONTROLTYPE_KEY)
	{
		// キーボード操作
		//MoveKeyboard(TutorilaPlayer::fSpeed);
	}
	// ジョイパッド操作
	else if (m_nControlTypes == CONTROLTYPE_PAD)
	{
		// 十字キーの入力がある場合は十字キーの移動だけを行う
		if (Pad->GetCrossKeyInput(Pad) == true)
		{
			//MoveCrossPadButton(TutorilaPlayer::fSpeed);
		}
		else
		{
			// 十字キーが押されていない場合は、スティックの判定を行う
			//MoveJoypad(3.75f);
		}
	}

	if (m_pMachine)
	{
		// ステートマシンの更新処理
		m_pMachine->Update();
	}

	// スフィアコライダー座標の更新
	if (m_pSphereCollider)
	{
		m_pSphereCollider->SetPos(pos);
	}

	// 座標の更新処理
	CMoveCharactor::UpdatePosition();

	// 更新後の座標取得
	auto UpdatePos = GetPos();

	// ボックスコライダー座標の更新
	if (m_pBoxCollider)
	{
		m_pBoxCollider->SetPos(UpdatePos);
		m_pBoxCollider->SetPosOld(GetOldPos());
	}

	// インスタンス取得
	const auto& InteractPoint = CWorldUICollision::GetInstance()->GetInteractPoints();
	if (!&InteractPoint) return;

	// 最大数と判定をする
	for (const auto& Colliders : InteractPoint)
	{
		// nullチェック
		if (Colliders == nullptr || Colliders->pCollider == nullptr) continue;

		// プレイヤーの球と判定し、有効なら
		if (CollisionSphere(Colliders->pCollider.get()))
		{
			// 入力フラグ
			bool isInputKey = false;

			// 操作の種類によって使える物を変化させる
			switch (m_nControlTypes)
			{
			case CTutorialPlayer::CONTROLTYPE_NONE:
				break;

			case CTutorialPlayer::CONTROLTYPE_KEY:
				if (Key->GetTrigger(DIK_F))
				{
					isInputKey = true;
				}
				break;

			case CTutorialPlayer::CONTROLTYPE_PAD:
				if (Pad->GetTrigger(CJoyPad::JOYKEY_START))
				{
					isInputKey = true;
				}
				break;

			default:
				break;
			}

			// タスクの起動処理
			if (isInputKey)
			{
				switch (Colliders->nType)
				{
				case CWorldUICollision::TYPE_NONE: // タスクをしていない状態[add Misaki]
					break;

				case CWorldUICollision::TYPE_DOCUMENT: // 書類タスク[add Misaki]

					//// 両方がnullじゃない状態
					//if (pDesk && pDesk->GetDOCUMENTDesk() && (pDesk->GetDOCUMENTDesk()->GetCOPYTaskNum() > 0))
					//{
					//	pDesk->GetDOCUMENTDesk()->SetDOCUMENTValue();
					//}

					break;

				default:

					//// nullじゃない状態
					//if (pDesk)
					//{
					//	// デスクワーク時
					//	if (Colliders->nType == CWorldUICollision::TYPE_PC)
					//	{
					//		// PCタスクの時の動作関数
					//		MathDeskRotation();
					//	}

					//	// タスクを起動する
					//	pDesk->SetTaskType(CWorldUICollision::TYPE(Colliders->nType), true);
					//}
					break;
				}

				// タスク起動したら抜ける
				break;
			}
		}
	}

	// ブロックとの判定
	UpdateBlockCollision(UpdatePos);

	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutorialPlayer::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();

	// プレイヤー座標のデバッグ表示
	CDebugproc::Print("[プレイヤーの位置] : { %.2f,%.2f,%.2f }", GetPos().x, GetPos().y, GetPos().z);
	CDebugproc::Draw(0, 180);

#ifdef _DEBUG
	// モーションのデバッグ表示
	//GetMotion()->Debug();
#endif
}
//=========================================================
// 当たり判定
//=========================================================
bool CTutorialPlayer::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::CollisionEx(m_pBoxCollider.get(), pOther, OutPos);
}
//=========================================================
// 球形当たり判定
//=========================================================
bool CTutorialPlayer::CollisionSphere(CSphereCollider* pOther)
{
	// nullチェック
	if (m_pSphereCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(), pOther);
}
//=========================================================
// ステート変更処理
//=========================================================
void CTutorialPlayer::ChangeState(CTutorialPlayerStateBase* pState, int nID)
{
#if 0
	// 自分自身のポインタを設定
	pState->SetOwner(this);

	// IDの設定
	pState->SetID(nID);

	// ステート変更
	m_pMachine->ChangeState(pState);
#endif
}
//=================================================
// プレイヤー移動処理(キーボード編)
//=================================================
void CTutorialPlayer::MoveKeyboard(float speed)
{
	// キー操作タイプじゃないなら
	if (m_nControlTypes != CONTROLTYPE_KEY) return;

	// キーボードのポインタ
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// カメラのポインタ
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 向きの取得
	D3DXVECTOR3 rot = pCamera->GetRot();

	// ビューマトリックスの取得
	auto ViewMatrix = pCamera->GetView();

	// さぼっているかの判定
	auto bAfkSmoke = CAfkManager::Instance()->GetAfkSmoke()->GetAfk();
	auto bAfkTV = CAfkManager::Instance()->GetAfkTV()->GetAfk();
	auto bAfkMagazine = CAfkManager::Instance()->GetAfkMagazine()->GetAfk();
	auto bAfkGameCenter = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk();

	if (bAfkSmoke && pKeyboard->GetTrigger(DIK_F)) m_bAfkSmoke = m_bAfkSmoke ? false : true;
	else if (!bAfkSmoke) m_bAfkSmoke = false;

	if (bAfkTV && pKeyboard->GetTrigger(DIK_F)) m_bAfkTV = m_bAfkTV ? false : true;
	else if (!bAfkTV) m_bAfkTV = false;

	if (bAfkMagazine && pKeyboard->GetTrigger(DIK_F)) m_bAfkMagazine = m_bAfkMagazine ? false : true;
	else if (!bAfkMagazine) m_bAfkMagazine = false;

	if (bAfkGameCenter && pKeyboard->GetTrigger(DIK_F)) m_bAfkGameCenter = m_bAfkGameCenter ? false : true;
	else if (!bAfkGameCenter) m_bAfkGameCenter = false;

	// ビュー行列の逆行列を計算
	D3DXMATRIX invViewMat;
	D3DXMatrixInverse(&invViewMat, NULL, &ViewMatrix);

	// 逆行列からカメラの方向ベクトルを抽出
	D3DXVECTOR3 camForward = D3DXVECTOR3(invViewMat._31, invViewMat._32, invViewMat._33);
	D3DXVECTOR3 camRight = D3DXVECTOR3(invViewMat._11, invViewMat._12, invViewMat._13);

	// XZ平面の移動にするため、Y成分を0にする
	camForward.y = NULL;
	camRight.y = NULL;

	// 方向ベクトルの正規化
	D3DXVec3Normalize(&camForward, &camForward);
	D3DXVec3Normalize(&camRight, &camRight);

	// 移動方向の計算
	D3DXVECTOR3 moveDir = VECTOR3_NULL;

	// 目的の向き
	D3DXVECTOR3 RotDest = GetRotDest();

	if (pKeyboard->GetPress(DIK_W))
	{
		moveDir += camForward;
		RotDest.y = rot.y + D3DX_PI;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pKeyboard->GetPress(DIK_S))
	{
		moveDir -= camForward;
		RotDest.y = rot.y;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pKeyboard->GetPress(DIK_D))
	{
		moveDir += camRight;
		RotDest.y = rot.y - D3DX_PI * HALF;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pKeyboard->GetPress(DIK_A))
	{
		moveDir -= camRight;
		RotDest.y = rot.y + D3DX_PI * HALF;

		// 移動判定をtrueに
		m_bMove = true;
	}

	// サボり判定が有効な物があったら
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			//CGameSceneObject::GetInstance()->GetProgressgauge()->AddAFK();
			m_nCntAfk = 0;
		}
	}

	// キーが押されていなかったら
	else if (!pKeyboard->GetPress(DIK_W) &&
		!pKeyboard->GetPress(DIK_S) &&
		!pKeyboard->GetPress(DIK_D) &&
		!pKeyboard->GetPress(DIK_A))
	{
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::NEUTRAL, true, 5);
		m_nCntAfk = 0;
	}
	// 移動入力がある場合
	else if (m_bMove)
	{
		// 移動の正規化
		D3DXVec3Normalize(&moveDir, &moveDir);

		// 位置の更新
		SetMove(moveDir * speed);

		// 移動方向から向きを計算
		RotDest.y = atan2f(-moveDir.x, -moveDir.z);

		// 目的の向きを設定
		SetRotDest(RotDest);

		// 移動モーション設定
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// プレイヤー移動処理(ジョイパッド編)
//=================================================
void CTutorialPlayer::MoveJoypad(float speed)
{
	// パッド操作タイプじゃないなら
	if (m_nControlTypes != CONTROLTYPE_PAD) return;

	// ジョイパッドのポインタ
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();
	if (!pJoyPad) return;

	XINPUT_STATE* pState = pJoyPad->GetStickAngle();

	// カメラのポインタ
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 向きの取得
	D3DXVECTOR3 rot = pCamera->GetRot();

	// ビューマトリックスの取得
	auto ViewMatrix = pCamera->GetView();

	// さぼっているかの判定
	auto bAfkSmoke = CAfkManager::Instance()->GetAfkSmoke()->GetAfk();
	auto bAfkTV = CAfkManager::Instance()->GetAfkTV()->GetAfk();
	auto bAfkMagazine = CAfkManager::Instance()->GetAfkMagazine()->GetAfk();
	auto bAfkGameCenter = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk();

	if (bAfkSmoke && pJoyPad->GetTrigger(CJoyPad::JOYKEY_START))m_bAfkSmoke = m_bAfkSmoke ? false : true;
	else if (!bAfkSmoke) m_bAfkSmoke = false;

	if (bAfkTV && pJoyPad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkTV = m_bAfkTV ? false : true;
	else if (!bAfkTV) m_bAfkTV = false;

	if (bAfkMagazine && pJoyPad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkMagazine = m_bAfkMagazine ? false : true;
	else if (!bAfkMagazine) m_bAfkMagazine = false;

	if (bAfkGameCenter && pJoyPad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkGameCenter = m_bAfkGameCenter ? false : true;
	else if (!bAfkGameCenter) m_bAfkGameCenter = false;

	// ビュー行列の逆行列を計算
	D3DXMATRIX invViewMat;
	D3DXMatrixInverse(&invViewMat, NULL, &ViewMatrix);

	// 逆行列からカメラの方向ベクトルを抽出
	D3DXVECTOR3 camForward = D3DXVECTOR3(invViewMat._31, invViewMat._32, invViewMat._33);
	D3DXVECTOR3 camRight = D3DXVECTOR3(invViewMat._11, invViewMat._12, invViewMat._13);

	// XZ平面の移動にするため、Y成分を0にする
	camForward.y = NULL;
	camRight.y = NULL;

	// 方向ベクトルの正規化
	D3DXVec3Normalize(&camForward, &camForward);
	D3DXVec3Normalize(&camRight, &camRight);

	// 移動方向の計算
	D3DXVECTOR3 moveDir = VECTOR3_NULL;

	// 目的の向き
	D3DXVECTOR3 RotDest = GetRotDest();
	auto move = GetMove();

	if (pJoyPad->GetLeftStick())
	{
		// 左スティックの角度
		float LStickAngleY = pState->Gamepad.sThumbLY;
		float LStickAngleX = pState->Gamepad.sThumbLX;

		// デッドゾーンを設定
		float DeadZone = 0.5f;
		float fMag = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

		if (fMag < DeadZone)
		{
			m_bMove = false;
			SetMove(VECTOR3_NULL);
		}
		else if (fMag > DeadZone)
		{
			// 正規化
			float normalizeX = (LStickAngleX / fMag);
			float normalizeY = (LStickAngleY / fMag);

			// 移動量を計算する
			float MoveX = normalizeX * cosf(-pCamera->GetRot().y) - normalizeY * sinf(-pCamera->GetRot().y);
			float MoveZ = normalizeX * sinf(-pCamera->GetRot().y) + normalizeY * cosf(-pCamera->GetRot().y);

			// 移動量を設定
			move.x += MoveX * speed;
			move.z += MoveZ * speed;
			RotDest.y = atan2f(-MoveX, -MoveZ);
			m_bMove = true;
		}
		else
		{
			m_bMove = false;
		}
	}

	// モーションチェンジ
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			//CGameSceneObject::GetInstance()->GetProgressgauge()->AddAFK();
			m_nCntAfk = 0;
		}
	}
	// 移動していなかったら
	else if (!pJoyPad->GetLeftStick())
	{
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::NEUTRAL, true, 5);
		m_nCntAfk = 0;
	}
	// 移動入力がある場合
	else if (m_bMove)
	{
		// 位置の更新
		SetMove(move);

		// 目的の向きを設定
		SetRotDest(RotDest);

		// 移動モーション設定
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// 十字キーでの移動バージョン
//=================================================
void CTutorialPlayer::MoveCrossPadButton(float speed)
{
	// パッド操作タイプじゃないなら
	if (m_nControlTypes != CONTROLTYPE_PAD) return;

	// パッドのポインタ
	CJoyPad* pGamePad = CManager::GetInstance()->GetJoyPad();

	// カメラのポインタ
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 向きの取得
	D3DXVECTOR3 rot = pCamera->GetRot();

	// ビューマトリックスの取得
	auto ViewMatrix = pCamera->GetView();

	// さぼっているかの判定
	auto bAfkSmoke = CAfkManager::Instance()->GetAfkSmoke()->GetAfk();
	auto bAfkTV = CAfkManager::Instance()->GetAfkTV()->GetAfk();
	auto bAfkMagazine = CAfkManager::Instance()->GetAfkMagazine()->GetAfk();
	auto bAfkGameCenter = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk();

	// サボりキー入力判定
	if (bAfkSmoke && pGamePad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkSmoke = m_bAfkSmoke ? false : true;
	else if (!bAfkSmoke) m_bAfkSmoke = false;

	if (bAfkTV && pGamePad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkTV = m_bAfkTV ? false : true;
	else if (!bAfkTV) m_bAfkTV = false;

	if (bAfkMagazine && pGamePad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkMagazine = m_bAfkMagazine ? false : true;
	else if (!bAfkMagazine) m_bAfkMagazine = false;

	if (bAfkGameCenter && pGamePad->GetTrigger(CJoyPad::JOYKEY_START)) m_bAfkGameCenter = m_bAfkGameCenter ? false : true;
	else if (!bAfkGameCenter) m_bAfkGameCenter = false;

	// ビュー行列の逆行列を計算
	D3DXMATRIX invViewMat;
	D3DXMatrixInverse(&invViewMat, NULL, &ViewMatrix);

	// 逆行列からカメラの方向ベクトルを抽出
	D3DXVECTOR3 camForward = D3DXVECTOR3(invViewMat._31, invViewMat._32, invViewMat._33);
	D3DXVECTOR3 camRight = D3DXVECTOR3(invViewMat._11, invViewMat._12, invViewMat._13);

	// XZ平面の移動にするため、Y成分を0にする
	camForward.y = NULL;
	camRight.y = NULL;

	// 方向ベクトルの正規化
	D3DXVec3Normalize(&camForward, &camForward);
	D3DXVec3Normalize(&camRight, &camRight);

	// 移動方向の計算
	D3DXVECTOR3 moveDir = VECTOR3_NULL;

	// 目的の向き
	D3DXVECTOR3 RotDest = GetRotDest();

	// フラグ初期化
	m_bMove = false;

	//**********************************
	// 移動計算ブロック
	//**********************************
	if (pGamePad->GetPress(CJoyPad::JOYKEY_UP))
	{
		moveDir += camForward;
		RotDest.y = rot.y + D3DX_PI;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pGamePad->GetPress(CJoyPad::JOYKEY_DOWN))
	{
		moveDir -= camForward;
		RotDest.y = rot.y;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pGamePad->GetPress(CJoyPad::JOYKEY_RIGHT))
	{
		moveDir += camRight;
		RotDest.y = rot.y - D3DX_PI * HALF;

		// 移動判定をtrueに
		m_bMove = true;
	}
	if (pGamePad->GetPress(CJoyPad::JOYKEY_LEFT))
	{
		moveDir -= camRight;
		RotDest.y = rot.y + D3DX_PI * HALF;

		// 移動判定をtrueに
		m_bMove = true;
	}

	// サボり判定が有効な物があったら
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			//CGameSceneObject::GetInstance()->GetProgressgauge()->AddAFK();
			m_nCntAfk = 0;
		}
	}

	// 十字キーが押されていなかったら
	else if (!pGamePad->GetPress(CJoyPad::JOYKEY_UP) &&
		!pGamePad->GetPress(CJoyPad::JOYKEY_DOWN) &&
		!pGamePad->GetPress(CJoyPad::JOYKEY_RIGHT) &&
		!pGamePad->GetPress(CJoyPad::JOYKEY_LEFT))
	{
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::NEUTRAL, true, 5);
		m_nCntAfk = 0;
	}
	// 移動入力がある場合
	else if (m_bMove)
	{
		// 移動の正規化
		D3DXVec3Normalize(&moveDir, &moveDir);

		// 位置の更新
		SetMove(moveDir * speed);

		// 移動方向から向きを計算
		RotDest.y = atan2f(-moveDir.x, -moveDir.z);

		// 目的の向きを設定
		SetRotDest(RotDest);

		// 移動モーション設定
		GetMotion()->SetMotion(CTutorialPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// ブロックとのコリジョン判定関数わけ
//=================================================
void CTutorialPlayer::UpdateBlockCollision(D3DXVECTOR3 pos)
{
	// スキップする
	if (m_bAfkTV) return;
	if (m_isPcWork)
	{
		return;
	}

	const auto& BlockManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (!BlockManager) return;

	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		auto IdxBlock = BlockManager->GetBlock(nCnt);
		if (!IdxBlock) continue;

		// コライダー取得
		CBoxCollider* Collider = IdxBlock->GetCollider();
		if (!Collider) continue;

		if (Collision(Collider, &pos))
		{
			// 現在座標をセット
			SetPos(pos);

			// コライダー更新
			m_pBoxCollider->SetPos(pos);
			m_pBoxCollider->SetPosOld(pos);
		}
	}

	// サイドに開くドアの当たり判定
	const auto& SideDoorManager = CSideOpenDoorManager::GetInstance();
	if (!SideDoorManager) return;

	for (int nCnt = 0; nCnt < SideDoorManager->GetAll(); nCnt++)
	{
		// インデックスごとの取得
		auto IdxSide = SideDoorManager->GetSideOpenDoor(nCnt);
		if (!IdxSide) continue;

		// もし状態が"開き"ならスキップ
		auto State = IdxSide->GetState();
		if (State != CSideOpenDoor::STATE_CLOSE) continue;

		// 矩形のコライダー取得
		CBoxCollider* Collider = IdxSide->GetCollider();
		if (!Collider) continue;

		// ヒットしたら
		if (Collision(Collider, &pos))
		{
			// 現在座標をセット
			SetPos(pos);

			// コライダー更新
			m_pBoxCollider->SetPos(pos);
			m_pBoxCollider->SetPosOld(pos);
		}
	}
}
//=================================================
// パソコンを向くための計算関数 
//=================================================
void CTutorialPlayer::MathDeskRotation(void)
{
	// フラグを有効化する
	m_isPcWork = true;

	// モーション情報を変更
	GetMotion()->SetMotion(CTutorialPlayer::MOTION::TV, true, 3);

	// プレイヤーの位置をデスク用の固定座標にセット
	SetPos(VECTOR3_NULL);

	// 向きたい角度
	float fRotY = D3DX_PI * 0.5f;

	// 目的の角度を設定
	SetRotDest(D3DXVECTOR3(0.0f, -fRotY, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, -fRotY, 0.0f));
}