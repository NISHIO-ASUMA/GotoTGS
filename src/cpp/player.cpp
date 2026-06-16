//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "player.h"

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
#include "PCdeskwork.h"			// Misaki
#include "COPYdeskwork.h"		// Misaki
#include "DOCUMENTdeskwork.h"	// Misaki
#include "worldUIcollision.h"
#include "collisionsphere.h"
#include "afkmanager.h"
#include "afktv.h"
#include "afksmoke.h"
#include "afkmagazine.h"
#include "automaticdoormanager.h" // 西尾追加
#include "automatic_door.h"		 // 西尾追加
#include "autodoor_collision.h"		 // 西尾追加
#include "outline.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace player
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
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pSubItemModels(nullptr),
m_pMachine(nullptr),
m_bMove(false),
m_bAfkSmoke(false),
m_bAfkTV(false),
m_bAfkMagazine(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CPlayer::~CPlayer()
{

}
//=========================================================
// 生成処理
//=========================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CPlayer* pPlayer = new CPlayer;
	if(pPlayer == nullptr)return nullptr;

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
HRESULT CPlayer::Init(void)
{
	// 親クラスの初期化処理
	CMoveCharactor::Init();

	// モーション読み込み
	MotionLoad(player::SCRIPT,MAX,false);

	// インスタンス生成
	m_pMachine = new CStateMachine;

	// 初期ステートをセット
	ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot(); // オブジェクトの回転角度を取得

	// X, Y, Zの回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// ボックスコライダーの生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetOldPos(), D3DXVECTOR3(player::BoxSize, player::BoxSize, player::BoxSize),matRot);

	// スフィアコライダーの生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), player::SphereSize);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPlayer::Uninit(void)
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
void CPlayer::Update(void)
{
	//******************************************************
	// NOTE : 西尾追記 2026/05/18
	//        今はPCの作業のみだけどこれから複数のタスクの判定も組んであげる形に変更になる
	//		　髙橋追記 2026/05/19
	//		　コピー機用の処理を追加しました
	//        西尾追記 : カメラの固定化する処理を追加したよ
	//		  西尾追記 : 2026/06/05 自動ドアの処理を追加
	//        西尾追記 : 2026/06/08 自動ドアの判別設定と判定の修正

	// タスクの情報を取得
	auto* pDesk = CGameSceneObject::GetInstance()->GetDesk();

	// 各タスクの使用中かどうか
	bool isPcDeskWork = (pDesk && pDesk->GetPCDeskUI()) ? pDesk->GetPCDeskUI()->GetUse() : false;			// PCタスク
	bool isCopyDeskWork = (pDesk && pDesk->GetCOPYDeskUI()) ? pDesk->GetCOPYDeskUI()->GetUse() : false;		// コピー機タスク [add Misaki]

	// キー入力取得
	const auto& Key = CManager::GetInstance()->GetInputKeyboard();

	//*********************************************************
	// ADD: 西尾 タスク中にキーが押されたら、タスクを閉じる
	//*********************************************************
	if (isPcDeskWork || isCopyDeskWork)
	{
		if (!Key->GetTrigger(DIK_F))
		{// 終了キーを押していない場合
			return;
		}

		// 起動したタスクを非アクティブにする [add Misaki]
		if (isPcDeskWork)
		{// PCタスクの場合
			pDesk->GetPCDeskUI()->SetAlphaUI();
			// カメラ固定フラグ無効化
			CManager::GetInstance()->GetCamera()->SetCameraMove(false);
		}
		else if (isCopyDeskWork)
		{// コピー機タスクの場合
			pDesk->GetCOPYDeskUI()->SetAlphaUI();
			// カメラ固定フラグ無効化
			CManager::GetInstance()->GetCamera()->SetCameraMove(false);
		}

		// タスク中は移動や他の当たり判定をさせないためにリターン
		return;
	}

	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 oldpos = GetOldPos();

	// キーボード操作
	MoveKeyboard(player::fSpeed);

	// ジョイパッド操作
	//MoveJoypad(player::fSpeed);

	// ステートマシンの更新処理
	m_pMachine->Update();

	// ボックスコライダー座標の更新
	if (m_pBoxCollider)
	{
		m_pBoxCollider->SetPos(pos);
		m_pBoxCollider->SetPosOld(oldpos);
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
	
//*************************************************
// ADD : 西尾追加 タスクを起こせる球との当たり判定をとり、その時のキー入力でタスク発生(指定したもの)

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
			// 当たっている かつ Fキー入力
			if (Key->GetTrigger(DIK_F))
			{
				// カメラ固定フラグ有効化
				CManager::GetInstance()->GetCamera()->SetCameraMove(true);
				
				switch (Colliders->nType)
				{
				case CWorldUICollision::TYPE_PC: // パソコンタスク
				{
					// 両方がnullじゃない状態
					if (pDesk && pDesk->GetPCDeskUI())
					{
						pDesk->SetTexBG(CWorldUICollision::TYPE_PC);
						pDesk->GetPCDeskUI()->SetAlphaUI(true);
					}
				}
					break;

				case CWorldUICollision::TYPE_COPY: // コピー機作業 [add Misaki]

					// 両方がnullじゃない状態
					if (pDesk && pDesk->GetCOPYDeskUI())
					{
						pDesk->SetTexBG(CWorldUICollision::TYPE_COPY);
						pDesk->GetCOPYDeskUI()->SetAlphaUI(true);
					}

					break;

				case CWorldUICollision::TYPE_DOCUMENT: // 書類タスク[add Misaki]
				{
					// 両方がnullじゃない状態
					if (pDesk && pDesk->GetDOCUMENTDesk() && (pDesk->GetDOCUMENTDesk()->GetCOPYTaskNum() > 0))
					{
						pDesk->GetDOCUMENTDesk()->SetDOCUMENTValue();
					}
				}

					break;

				default:
					break;
				}

				// タスク起動したら抜ける
				break;
			}
		}
	}

//*************************************************
	// jsonmanagerからブロックを取得
	const auto& BlockManager = CManager::GetInstance()->GetJsonManager()->GetBlockManager();
	if (BlockManager == nullptr) return;

	// 最大ブロックを取得する
	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		// 各ブロックを取得し判定を生成
		auto IdxBlock = BlockManager->GetBlock(nCnt);

		// コライダー取得とnullチェック
		CBoxCollider* Collider = IdxBlock->GetCollider();
		if (Collider == nullptr) continue;

		// 当たり判定の実行
		if (Collision(Collider, &UpdatePos))
		{
			// 当たった点の座標セット
			SetPos(UpdatePos);

			// コライダーと現在座標の更新をする
			m_pBoxCollider->SetPos(UpdatePos);
			m_pBoxCollider->SetPosOld(UpdatePos);
		}
	}

//*************************************************
// ADD : 西尾 自動開閉ドア関係
//*************************************************
	auto* pDoorCollision = CAutoMaticDoorCollision::GetInstance(); // コライダークラス
	auto* pDoorManager = CAutoMaticDoorManager::GetInstance();	   // ドア管理クラス

	if (pDoorCollision && pDoorManager)
	{
		//自動ドア判定用コライダーを取得
		const auto& DoorColliders = pDoorCollision->GetColliders();

		for (const auto& ColliderData : DoorColliders)
		{
			// nullチェック
			if (ColliderData == nullptr || ColliderData->pCollider == nullptr) continue;

			// プレイヤーの球と、自動ドアのセンサー球との当たり判定
			if (CollisionSphere(ColliderData->pCollider.get()))
			{
				// 球コライダー座標を更新
				if (m_pSphereCollider)
				{
					m_pSphereCollider->SetPos(UpdatePos);
				}

				// 当たったコライダーのインデックスを渡して、特定のペアを開ける
				pDoorManager->StartOpen(ColliderData->nIdx);
				break;
			}
		}
	}

	// 親クラスの更新処理
	CMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CPlayer::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();

	// 特定モデルがあるなら
	if (m_pSubItemModels)
	{
		// モデル描画
		m_pSubItemModels->Draw();

		// マトリックス取得
		auto mtxworld = GetMtx();

		// ワールドマトリックスの設定
		CManager::GetInstance()->GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxworld);

		// カリングを切る
		CManager::GetInstance()->GetRenderer()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// シェーダー開始
		COutLine::GetInstance()->Begin();
		COutLine::GetInstance()->BeginPass();

		// アウトラインセット
		m_pSubItemModels->DrawOutLine();

		// シェーダー終了
		COutLine::GetInstance()->EndPass();
		COutLine::GetInstance()->End();

		// カリングを戻す
		CManager::GetInstance()->GetRenderer()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	// プレイヤー座標のデバッグ表示
	CDebugproc::Print("[プレイヤーの位置] : { %.2f,%.2f,%.2f }", GetPos().x, GetPos().y, GetPos().z);
	CDebugproc::Draw(0, 180);

#ifdef _DEBUG
	// モーションのデバッグ表示
	GetMotion()->Debug();
#endif
}
//=========================================================
// 当たり判定
//=========================================================
bool CPlayer::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::CollisionEx(m_pBoxCollider.get(), pOther, OutPos);
}
//=========================================================
// 球形当たり判定
//=========================================================
bool CPlayer::CollisionSphere(CSphereCollider* pOther)
{
	// nullチェック
	if (m_pSphereCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(), pOther);
}
//=========================================================
// ステート変更処理
//=========================================================
void CPlayer::ChangeState(CPlayerStateBase* pState, int nID)
{
	// 自分自身のポインタを設定
	pState->SetOwner(this);

	// IDの設定
	pState->SetID(nID);

	// ステート変更
	m_pMachine->ChangeState(pState);
}
//=================================================
// プレイヤー移動処理(キーボード編)
//=================================================
void CPlayer::MoveKeyboard(float speed)
{
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

	if (bAfkSmoke && pKeyboard->GetTrigger(DIK_F)) m_bAfkSmoke = m_bAfkSmoke ? false : true;
	else if (!bAfkSmoke) m_bAfkSmoke = false;
	
	if (bAfkTV && pKeyboard->GetTrigger(DIK_F)) m_bAfkTV = m_bAfkTV ? false : true;
	else if (!bAfkTV) m_bAfkTV = false;
	
	if (bAfkMagazine && pKeyboard->GetTrigger(DIK_F)) m_bAfkMagazine = m_bAfkMagazine ? false : true;
	else if (!bAfkMagazine) m_bAfkMagazine = false;

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

//***********************************************
// TODO :  特定のモーションの時に特定のモデルを持たせる
//***********************************************

	// テレビを見るモーションに切り替え
	if (m_bAfkTV)
	{
		// プレイヤー座標を椅子の上にセットする
		
		// 角度を変更し、テレビに向ける
		// SetRot();

		// tvモーションに変更する
		GetMotion()->SetMotion(CPlayer::MOTION::TV);
	}

	// モーションチェンジ
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine) return;

	// キーが押されていなかったら
	else if (!pKeyboard->GetPress(DIK_W) &&
			 !pKeyboard->GetPress(DIK_S) &&
			 !pKeyboard->GetPress(DIK_D) &&
			 !pKeyboard->GetPress(DIK_A))
	{
		GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL,true,5);
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
		GetMotion()->SetMotion(CPlayer::MOTION::MOVE);
	}
}
//=================================================
// プレイヤー移動処理(ジョイパッド編)
//=================================================
void CPlayer::MoveJoypad(float speed)
{
	// ジョイパッドのポインタ
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();

	if (!pJoyPad) return;

	XINPUT_STATE* pState = pJoyPad->GetStickAngle();

	// コントローラーが接続されていなかったら
	//if (!pJoyPad->GetConnectGamePad()) return;

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

	if (bAfkSmoke && pJoyPad->GetTrigger(CJoyPad::JOYKEY_A))m_bAfkSmoke = m_bAfkSmoke ? false : true;
	else if (!bAfkSmoke) m_bAfkSmoke = false;

	if (bAfkTV && pJoyPad->GetTrigger(CJoyPad::JOYKEY_A)) m_bAfkTV = m_bAfkTV ? false : true;
	else if (!bAfkTV) m_bAfkTV = false;

	if (bAfkMagazine && pJoyPad->GetTrigger(CJoyPad::JOYKEY_A)) m_bAfkMagazine = m_bAfkMagazine ? false : true;
	else if (!bAfkMagazine) m_bAfkMagazine = false;

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

	if (pJoyPad->GetLeftStick())
	{
		if (pState->Gamepad.sThumbLY > player::fJoyInput)
		{
			moveDir += camForward;
			RotDest.y = rot.y + D3DX_PI;

			// 移動判定をtrueに
			m_bMove = true;
		}
		if (pState->Gamepad.sThumbLY < -player::fJoyInput)
		{
			moveDir -= camForward;
			RotDest.y = rot.y;

			// 移動判定をtrueに
			m_bMove = true;
		}
		if (pState->Gamepad.sThumbLX > player::fJoyInput)
		{
			moveDir += camRight;
			RotDest.y = rot.y - D3DX_PI * HALF;

			// 移動判定をtrueに
			m_bMove = true;
		}
		if (pState->Gamepad.sThumbLX < -player::fJoyInput)
		{
			moveDir -= camRight;
			RotDest.y = rot.y + D3DX_PI * HALF;

			// 移動判定をtrueに
			m_bMove = true;
		}
	}

	// テレビを見るモーションに切り替え
	else if (m_bAfkTV) GetMotion()->SetMotion(CPlayer::MOTION::TV);

	// モーションチェンジ
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine) return;

	// 移動していなかったら
	else if (!pJoyPad->GetLeftStick())GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL, true, 5);
	
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
		GetMotion()->SetMotion(CPlayer::MOTION::MOVE);
	}
}