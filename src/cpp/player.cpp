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
#include "PCdeskwork.h"				// 髙橋追加
#include "COPYdeskwork.h"			// 髙橋追加
#include "DOCUMENTdeskwork.h"		// 髙橋追加
#include "vigilanceUImanager.h"		// 髙橋追加
#include "vigilanceUImanager.h"		// 髙橋追加
#include "gaugeneedle.h"			// 髙橋追加
#include "worldUIcollision.h"
#include "collisionsphere.h"		// 近田追加
#include "afkmanager.h"				// 近田追加
#include "afktv.h"					// 近田追加
#include "afksmoke.h"				// 近田追加
#include "afkmagazine.h"			// 近田追加
#include "afkeating.h"				// 近田追加
#include "automaticdoormanager.h"	// 西尾追加
#include "automatic_door.h"			// 西尾追加
#include "autodoor_collision.h"		// 西尾追加
#include "outline.h"
#include "afkgamecenter.h"			// 近田追加
#include "titleuimanager.h"			// 近田追加
#include "progressgauge.h"
#include "score.h"					// 近田追加
#include "afkbench.h"				// 近田追加
#include "slideopendoormanager.h"	// 西尾追加
#include "sideopendoor.h"			// 西尾追加
#include "sideopendoorcollision.h"	// 西尾追加
#include "enemymanager.h"			// 西尾追加
#include "enemy.h"					// 西尾追加

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

//*********************************************************
// 定数名前空間 ( タスク関連時 )
//*********************************************************
namespace Player_Info
{
	// テレビ関係
	const D3DXVECTOR3 TV_CHARACTORPOS = { -244.0f, 10.5f,277.0f };
	const D3DXVECTOR3 TV_DESTPOS = { -248.0f, 12.0f, 362.0f };

	// デスクワーク関係
	const D3DXVECTOR3 DESK_DESTPOS = { -63.0f, 16.0f, 185.0f };		// デスクワーク中の座標
	const D3DXVECTOR3 DESK_RETURNPOS = { -100.0f, 0.0f, 175.0f };	// タスク終了時にもどる固定座標

	// ベンチ関係
	const D3DXVECTOR3 BENCH_CHARACTORPOS = { 792.4f, 14.0f, 1303.6f };
};

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pSubItemModels(nullptr),
m_pMachine(nullptr),
m_nCntAfk(NULL),
m_nTimeScore(NULL),
m_nAddScore(NULL),
m_nScoreCnt(NULL),
m_bMove(false),
m_bAfkSmoke(false),
m_bAfkTV(false),
m_bAfkMagazine(false),
m_bAfkGameCenter(false),
m_bAfkEating(false),
m_bAfkBench(false),
m_isPcWork(false),
m_nControlTypes(CONTROLTYPE_NONE)
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

	// 操作の種類を設定する(パッドかキーマウかどうか)
	m_nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();

	// フラグの再初期化
	m_isPcWork = false;

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
	//		  西尾追記 : 2026/06/16 モデルが出ないバグを修正 ステート追加

	// タスクの情報を取得
	auto* pDesk = CGameSceneObject::GetInstance()->GetDesk();

	// キー入力取得
	const auto& Key = CManager::GetInstance()->GetInputKeyboard();
	const auto& Pad = CManager::GetInstance()->GetJoyPad();

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
		case CPlayer::CONTROLTYPE_NONE:
			break;

		case CPlayer::CONTROLTYPE_KEY:

			if (Key->GetTrigger(DIK_F))
			{
				isInputKey = true;
			}
			break;

		case CPlayer::CONTROLTYPE_PAD:

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
			SetPos(Player_Info::DESK_RETURNPOS);

			// 角度を変更
			SetRot(VECTOR3_NULL);

			// フラグをリセットする
			m_isPcWork = false;
		}

		// 起動したタスクを非アクティブにする [add 髙橋]
		pDesk->SetTaskType(pDesk->GetTaskType());

		// タスク中は移動や他の当たり判定をさせないためにリターン
		return;
	}

	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 oldpos = GetOldPos();

	if (m_nControlTypes == CONTROLTYPE_KEY)
	{
		// キーボード操作
		MoveKeyboard(player::fSpeed);
	}
	// ジョイパッド操作
	else if (m_nControlTypes == CONTROLTYPE_PAD)
	{
		// 十字キーの入力がある場合は十字キーの移動だけを行う
		if (Pad->GetCrossKeyInput(Pad) == true)
		{
			MoveCrossPadButton(player::fSpeed);
		}
		else
		{
			// 十字キーが押されていない場合は、スティックの判定を行う
			MoveJoypad(3.75f);
		}
	}
	
	// ステートマシンの更新処理
	m_pMachine->Update();

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
			case CPlayer::CONTROLTYPE_NONE:
				break;

			case CPlayer::CONTROLTYPE_KEY:
				if (Key->GetTrigger(DIK_F))
				{
					isInputKey = true;
				}
				break;

			case CPlayer::CONTROLTYPE_PAD:
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
				case CWorldUICollision::TYPE_NONE: // タスクをしていない状態[add 髙橋]
					break;

				case CWorldUICollision::TYPE_DOCUMENT: // 書類タスク[add 髙橋]
				
					// 両方がnullじゃない状態
					if (pDesk && pDesk->GetDOCUMENTDesk() && (pDesk->GetDOCUMENTDesk()->GetCOPYTaskNum() > 0))
					{
						pDesk->GetDOCUMENTDesk()->SetDOCUMENTValue();
					}

					break;

				default:

					// nullじゃない状態
					if (pDesk)
					{
						// デスクワーク時
						if (Colliders->nType == CWorldUICollision::TYPE_PC)
						{
							// PCタスクの時の動作関数
							MathDeskRotation();
						}

						// タスクを起動する
						pDesk->SetTaskType(CWorldUICollision::TYPE(Colliders->nType), true);
					}

					break;

				}

				// タスク起動したら抜ける
				break;
			}
		}
	}

	// ブロックとの判定
	UpdateBlockCollision(UpdatePos);

	// 自動ドアとの判定
	UpdateAutoDoorCollision(UpdatePos);

	// 敵の視界との当たり判定
	CollisionEnemyEyesite(UpdatePos);

	// オフィス内のドアとの判定
	UpdateSideDoorCollision(UpdatePos,Key,Pad);

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

	// たばこさぼり
	SetAfk(AFKTYPE_SMOKE, pKeyboard->GetTrigger(DIK_F));
	
	// TVさぼり
	SetAfk(AFKTYPE_TV, pKeyboard->GetTrigger(DIK_F));

	// 雑誌さぼり
	SetAfk(AFKTYPE_MAGAZINE, pKeyboard->GetTrigger(DIK_F));

	// ゲームセンターさぼり
	SetAfk(AFKTYPE_GAMECENTER, pKeyboard->GetTrigger(DIK_F));

	// 飲食さぼり
	SetAfk(AFKTYPE_EATING, pKeyboard->GetTrigger(DIK_F));

	// ベンチさぼり
	SetAfk(AFKTYPE_BENCH, pKeyboard->GetTrigger(DIK_F));

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
	
	// さぼり時のスコア加算
	AfkScore();

	// サボり判定が有効な物があったら
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating || m_bAfkBench)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			// さぼった回数を加算
			CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle()->AddAFK();
			m_nCntAfk = 0;
		}
	}

	// キーが押されていなかったら
	else if (!pKeyboard->GetPress(DIK_W) &&
			 !pKeyboard->GetPress(DIK_S) &&
			 !pKeyboard->GetPress(DIK_D) &&
			 !pKeyboard->GetPress(DIK_A))
	{
		GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL,true,5);
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
		GetMotion()->SetMotion(CPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// プレイヤー移動処理(ジョイパッド編)
//=================================================
void CPlayer::MoveJoypad(float speed)
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

	// たばこさぼり
	SetAfk(AFKTYPE_SMOKE, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

	// TVさぼり
	SetAfk(AFKTYPE_TV, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

	// 雑誌さぼり
	SetAfk(AFKTYPE_MAGAZINE, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

	// ゲームセンターさぼり
	SetAfk(AFKTYPE_GAMECENTER, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

	// 飲食さぼり
	SetAfk(AFKTYPE_EATING, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

	// ベンチさぼり
	SetAfk(AFKTYPE_BENCH, pJoyPad->GetTrigger(CJoyPad::JOYKEY_START));

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

	// さぼり時のスコア加算
	AfkScore();

	// モーションチェンジ
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating || m_bAfkBench)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle()->AddAFK();
			m_nCntAfk = 0;
		}
	}
	// 移動していなかったら
	else if (!pJoyPad->GetLeftStick())
	{
		GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL, true, 5);
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
		GetMotion()->SetMotion(CPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// 十字キーでの移動バージョン
//=================================================
void CPlayer::MoveCrossPadButton(float speed)
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


	// たばこさぼり
	SetAfk(AFKTYPE_SMOKE, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

	// TVさぼり
	SetAfk(AFKTYPE_TV, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

	// 雑誌さぼり
	SetAfk(AFKTYPE_MAGAZINE, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

	// ゲームセンターさぼり
	SetAfk(AFKTYPE_GAMECENTER, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

	// 飲食さぼり
	SetAfk(AFKTYPE_EATING, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

	// ベンチさぼり
	SetAfk(AFKTYPE_BENCH, pGamePad->GetTrigger(CJoyPad::JOYKEY_START));

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

	// さぼり時のスコア加算
	AfkScore();

	// サボり判定が有効な物があったら
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating || m_bAfkBench)
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 120)
		{
			CGameSceneObject::GetInstance()->GetProgressgauge()->GetGaugeneedle()->AddAFK();
			m_nCntAfk = 0;
		}
	}

	// 十字キーが押されていなかったら
	else if (!pGamePad->GetPress(CJoyPad::JOYKEY_UP) &&
			 !pGamePad->GetPress(CJoyPad::JOYKEY_DOWN) &&
			 !pGamePad->GetPress(CJoyPad::JOYKEY_RIGHT) &&
			 !pGamePad->GetPress(CJoyPad::JOYKEY_LEFT))
	{
		GetMotion()->SetMotion(CPlayer::MOTION::NEUTRAL, true, 5);
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
		GetMotion()->SetMotion(CPlayer::MOTION::MOVE);

		m_nCntAfk = 0;
	}
}
//=================================================
// ブロックとのコリジョン判定関数わけ
//=================================================
void CPlayer::UpdateBlockCollision(D3DXVECTOR3 pos)
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
// 自動ドアとのコリジョン関数分け
//=================================================
void CPlayer::UpdateAutoDoorCollision(D3DXVECTOR3 pos)
{
	auto* pDoorCollision = CAutoMaticDoorCollision::GetInstance(); // コライダークラス
	auto* pDoorManager = CAutoMaticDoorManager::GetInstance();	   // ドア管理クラス
	if (!pDoorCollision || !pDoorManager) return;

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
				m_pSphereCollider->SetPos(pos);
			}

			// 当たったコライダーのインデックスを渡して、特定のペアを開ける
			pDoorManager->StartOpen(ColliderData->nIdx);
			break;
		}
	}
}
//=================================================
// サイドに開くドアとのコリジョン関数分け
//=================================================
void CPlayer::UpdateSideDoorCollision(D3DXVECTOR3 pos, CInputKeyboard* key, CJoyPad* pad)
{
	auto* pSideDoorCollision = CSideOpenDoorCollision::GetInstance();	// コライダークラス
	auto* pSideDoorManager = CSideOpenDoorManager::GetInstance();		// ドア管理クラス
	if (!pSideDoorCollision || !pSideDoorManager) return;

	// 判定チェック
	if (m_nControlTypes == CONTROLTYPE_KEY)
	{
		if (!key->GetTrigger(DIK_F)) return;
	}
	else if (m_nControlTypes == CONTROLTYPE_PAD)
	{
		if (!pad->GetTrigger(CJoyPad::JOYKEY_START)) return;
	}

	//自動ドア判定用コライダーを取得
	const auto& DoorColliders = pSideDoorCollision->GetColliders();

	for (const auto& ColliderData : DoorColliders)
	{
		// nullチェック
		if (ColliderData == nullptr || ColliderData->pCollider == nullptr) continue;

		// プレイヤーの球と、サイドドアのセンサー球との当たり判定
		if (CollisionSphere(ColliderData->pCollider.get()))
		{
			// 球コライダー座標を更新
			if (m_pSphereCollider)
			{
				m_pSphereCollider->SetPos(pos);
				
			}
			
			// 当たったコライダーのインデックスを渡して指定数のドアを開ける
			pSideDoorManager->OpenSideDoor(ColliderData->targetDoorIndices);
			break;
		}
	}
}
//=================================================
// 敵の視界との当たり判定
//=================================================
void CPlayer::CollisionEnemyEyesite(const D3DXVECTOR3& UpdatePos)
{
	// 敵管理クラスを取得する
	const auto& Enemy = CEnemyManager::GetInstance()->GetEnemyData();

	// 警戒度ゲージを取得 [add 髙橋]
	const auto& Gauge = CGameSceneObject::GetInstance()->GetVigilanceUIManager()->GetGauge();

	// 敵の中での判定取得
	for (auto& IdxEnemy : Enemy)
	{
		// 見つかる扇方の範囲内だったら
		if (IdxEnemy->CheckEyesight(UpdatePos))
		{
			// 対象の敵の動きを変更する(プレイヤーを追従するかどうかのフラグを変更する )
			//IdxEnemy->SetTargetChaseFlag(true);

			// 現在のゲージの量を取得 [add 髙橋]
			float fNowRatio = Gauge->GetRatio();

			// 警戒度ゲージを増やす [add 髙橋]
			Gauge->SetRatio(fNowRatio + 0.002f);

			break;
		}
	}
}
//=================================================
// テレビを向くための計算関数
//=================================================
void CPlayer::MathTVRotation(void)
{
	// 座標をセットする
	SetPos(Player_Info::TV_CHARACTORPOS);

	// 対象ベクトルを作成
	auto VecToTV = Player_Info::TV_DESTPOS - GetPos();

	// 回転角を生成
	float fRotY = atan2f(-VecToTV.x, -VecToTV.z);

	// 角度を設定
	SetRotDest(D3DXVECTOR3(0.0f, fRotY, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, fRotY, 0.0f));
}
//=================================================
// パソコンを向くための計算関数 
//=================================================
void CPlayer::MathDeskRotation(void)
{
	// フラグを有効化する
	m_isPcWork = true;

	// モーション情報を変更
	GetMotion()->SetMotion(CPlayer::MOTION::TV, true, 3);

	// プレイヤーの位置をデスク用の固定座標にセット
	SetPos(Player_Info::DESK_DESTPOS);

	// 向きたい角度
	float fRotY = D3DX_PI * 0.5f;

	// 目的の角度を設定
	SetRotDest(D3DXVECTOR3(0.0f, -fRotY, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, -fRotY, 0.0f));
}
//=================================================
// ベンチの向きを調整するための計算関数 
//=================================================
void CPlayer::MathBenchRotation(void)
{
	// 座標をセットする
	SetPos(Player_Info::BENCH_CHARACTORPOS);
}

//=================================================
// さぼりの起動
//=================================================
void CPlayer::SetAfk(AFKTYPE AfkType, bool bInput)
{
	// さぼっているかの判定
	auto bAfkSmoke = CAfkManager::Instance()->GetAfkSmoke()->GetAfk();
	auto bAfkTV = CAfkManager::Instance()->GetAfkTV()->GetAfk();
	auto bAfkMagazine = CAfkManager::Instance()->GetAfkMagazine()->GetAfk();
	auto bAfkGameCenter = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk();
	auto bAfkEating = CAfkManager::Instance()->GetAfkEating()->GetAfk();
	auto bAfkBench = CAfkManager::Instance()->GetAfkBench()->GetAfk();

	switch (AfkType)
	{
	case AFKTYPE_SMOKE:
		// サボりキー入力判定
		if (bAfkSmoke && bInput) m_bAfkSmoke = m_bAfkSmoke ? false : true;
		else if (!bAfkSmoke) m_bAfkSmoke = false;
		break;
	case AFKTYPE_TV:
		if (bAfkTV && bInput) m_bAfkTV = m_bAfkTV ? false : true;
		else if (!bAfkTV) m_bAfkTV = false;
		break;
	case AFKTYPE_MAGAZINE:
		if (bAfkMagazine && bInput) m_bAfkMagazine = m_bAfkMagazine ? false : true;
		else if (!bAfkMagazine) m_bAfkMagazine = false;
		break;
	case AFKTYPE_GAMECENTER:
		if (bAfkGameCenter && bInput) m_bAfkGameCenter = m_bAfkGameCenter ? false : true;
		else if (!bAfkGameCenter) m_bAfkGameCenter = false;
		break;
	case AFKTYPE_EATING:
		if (bAfkEating && bInput) m_bAfkEating = m_bAfkEating ? false : true;
		else if (!bAfkEating) m_bAfkEating = false;
		break;
	case AFKTYPE_BENCH:
		if (bAfkBench && bInput) m_bAfkBench = m_bAfkBench ? false : true;
		else if (!bAfkBench) m_bAfkBench = false;
		break;
	default:
		break;
	}
}
//=================================================
// さぼり時のスコア加算関数
//=================================================
void CPlayer::AfkScore(void)
{
	// オフィス内のサボり判定が有効な物があったら
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkEating)
	{
		m_nTimeScore++;
		if ((60 * m_nScoreCnt) < m_nTimeScore)
		{

			// スコアの加算値上昇
			switch (m_nScoreCnt)
			{
			case 1:
				m_nAddScore = 1000;
				break;
			case 2:
				m_nAddScore = 1500;
				break;
			case 3:
				m_nAddScore = 2000;
				break;
			case 4:
				m_nAddScore = 1000;
				break;
			case 5:
				m_nAddScore = 500;
				break;
			case 6:
				m_nAddScore = 100;
				break;
			default:
				break;
			}

			// スコア加算
			CGameSceneObject::GetInstance()->GetScoreDitch()->AddScore(m_nAddScore);
			m_nScoreCnt++;

		}
	}

	// 外回りのさぼりが有効だったら
	if (m_bAfkGameCenter || m_bAfkBench)
	{
		m_nTimeScore++;
		if (60 < m_nTimeScore)
		{
			CGameSceneObject::GetInstance()->GetScore()->AddScore(2000);
			m_nTimeScore = NULL;
		}
	}

	// もしさぼり全部が無効だったら
	if (!m_bAfkSmoke && !m_bAfkTV && !m_bAfkMagazine && !m_bAfkGameCenter && !m_bAfkEating && !m_bAfkBench)
	{
		// スコア加算値の上昇カウントとスコア加算タイムをリセット
		m_nScoreCnt = 1;
		m_nTimeScore = NULL;
	}
}