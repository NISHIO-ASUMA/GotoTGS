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
#include "outline.h"
#include "fade.h"
#include "playerutility.h"
#include "outsidework.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pBoxCollider(nullptr),
m_pSphereCollider(nullptr),
m_pSubItemModels(nullptr),
m_pMachine(nullptr),
m_pEnemyManagerOutSide(nullptr),
m_posOld(VECTOR3_NULL),
m_nCntAfk(NULL),
m_nTimeScore(NULL),
m_nAddScore(NULL),
m_nScoreCnt(NULL),
m_nSmoke(NULL),
m_nTV(NULL),
m_nMagazine(NULL),
m_nGameCenter(NULL),
m_nEating(NULL),
m_nBench(NULL),
m_bMove(false),
m_bAfkSmoke(false),
m_bAfkTV(false),
m_bAfkMagazine(false),
m_bAfkGameCenter(false),
m_bAfkEating(false),
m_isPcWork(false),
m_isCatchEnemy(false),
m_nControlTypes(CONTROLTYPE_NONE),
m_isEnableLazy(false),
m_isSetOutSideTask(false),
m_nDeathTimer(NULL),
m_nInitTaskWorkingTime(NULL),
m_nNoActiveTaskTime(NULL),
m_nTaskClearBonusTime(NULL),
m_isTaskMaxOver(false),
m_isInitTaskTime(false),
m_isCopyMachine(false)
{
	for (int nCnt = 0; nCnt < Player_Bench::BENCH_MAX; nCnt++)
	{
		m_bAfkBench[nCnt] = false;
		m_nCoolTimeBench[nCnt] = NULL;
		m_pCoolTimeUiBench[nCnt] = nullptr;
	}

	for (int nCool = 0; nCool < AFKTYPE_MAX;nCool++) 
	{
		m_nAfkCoolTime[nCool] = NULL;
		m_pCoolTimeUi[nCool] = nullptr;
	}
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

	// 各サボりポイントの位置を取得してUIを生成
	CAfkManager* pAfk = CAfkManager::Instance();
	if (pAfk)
	{
		// 座標指定
		if (pAfk->GetAfkSmoke())	m_pCoolTimeUi[AFKTYPE_SMOKE] = CAfkCoolTimeUi::Create(pAfk->GetAfkSmoke()->GetPos() + player::UI_POS_VALUE);
		if (pAfk->GetAfkTV())		m_pCoolTimeUi[AFKTYPE_TV] = CAfkCoolTimeUi::Create(pAfk->GetAfkTV()->GetPos() +  player::UI_POS_VALUE);
		if (pAfk->GetAfkMagazine())	m_pCoolTimeUi[AFKTYPE_MAGAZINE] = CAfkCoolTimeUi::Create(pAfk->GetAfkMagazine()->GetPos() +  player::UI_POS_VALUE);
		if (pAfk->GetAfkGameCenter())m_pCoolTimeUi[AFKTYPE_GAMECENTER] = CAfkCoolTimeUi::Create(pAfk->GetAfkGameCenter()->GetPos() +  player::UI_POS_VALUE);
		if (pAfk->GetAfkEating())	m_pCoolTimeUi[AFKTYPE_EATING] = CAfkCoolTimeUi::Create(pAfk->GetAfkEating()->GetPos() +  player::UI_POS_VALUE);

		// ベンチ用UI生成
		for (int nBench = 0; nBench < Player_Bench::BENCH_MAX; nBench++)
		{
			if (pAfk->GetAfkBench(nBench))
			{
				m_pCoolTimeUiBench[nBench] = CAfkCoolTimeUi::Create(pAfk->GetAfkBench(nBench)->GetPos() +  player::UI_POS_VALUE);
			}
		}
	}

	// フラグの再初期化
	m_isPcWork = false;
	m_isCatchEnemy = false;
	m_isSetOutSideTask = false;

	// 初期の許容時間を設定 
	// これは最初に「仕事をしていましたよー」の時間分 最初から捕まるといやだから最初だけすぐ捕まらないようにする値
	m_nInitTaskWorkingTime = player::TASK_LIMIT_WORKING;

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
	// 更新だけしてreturn
	if (CManager::GetInstance()->GetCamera()->GetIsAnimTime())
	{
		// ui表示の設定
		UpdateAfkUiState();

		// 親クラスの更新
		CMoveCharactor::Update();
		return;
	}

	// もし上司に捕まってしまったら
	if (m_isCatchEnemy == true)
	{
		// モーションだけ更新
		CMoveCharactor::UpdateMotionOnly();
		return;
	}

	// 初期時間減算関数
	DecleInitTaskTime();

	// NOTE : 後に西尾が担当する
#if 0
	// 指針の取得
	// ゲームシーンのオブジェクトから進捗ゲージを取得
	auto* pProgressGauge = CGameSceneObject::GetInstance()->GetProgressgauge();
	if (pProgressGauge)
	{
		auto* pNeedle = pProgressGauge->GetGaugeneedle();
		if (pNeedle && pNeedle->GetIsFinish())
		{
			// 初めて上限に達した瞬間にタイマーをセット
			if (!m_isTaskMaxOver)
			{
				m_isTaskMaxOver = true;
				m_nDeathTimer = player::DEATH_LIMIT_FRAME;

				// 起動中のタスクを強制終了する

				// 画面を暗くする ( ライトの明るさを落とす )
				
				// uiの描画を開始
			}
		}
	}

	// タスク超過状態の処理
	if (m_isTaskMaxOver)
	{
		// 6秒の間にサボりを起動できたらカウント解除
		if (m_isEnableLazy)
		{
			m_isTaskMaxOver = false;
			m_nDeathTimer = 0;
		}
		else
		{
			// カウントダウン
			m_nDeathTimer--;

			// 6秒間サボれなかったらゲームオーバー
			if (m_nDeathTimer <= 0)
			{// この瞬間だけ"当たり判定をoff"にする
				// モーション変更
				//GetMotion()->SetMotion(CPlayer::MOTION::OVERWORK, true, 2);

				// モーションだけ更新
				CMoveCharactor::UpdateMotionOnly();

				// 画面遷移する
				//CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CWorkOverResult>());
				return;
			}
		}
	}
#endif

	// クールタイムのカウントダウン処理
	for (int nTime = 0; nTime < AFKTYPE_MAX; nTime++)
	{
		if (m_nAfkCoolTime[nTime] > 0)
		{
			m_nAfkCoolTime[nTime]--;	// デクリメント
		}
	}

	// ベンチ用カウントダウン
	for (int nBench = 0; nBench < Player_Bench::BENCH_MAX; nBench++)
	{
		if (m_nCoolTimeBench[nBench] > 0)
		{
			m_nCoolTimeBench[nBench]--;	// デクリメント
		}
	}

	// タスクの情報を取得
	auto* pDesk = CGameSceneObject::GetInstance()->GetDesk();

	// キー入力取得
	const auto& Key = CManager::GetInstance()->GetInputKeyboard();
	const auto& Pad = CManager::GetInstance()->GetJoyPad();

	// タスク中にキーが押されたら、タスクを閉じる
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
			// 更新だけ挟んでreturnする
			CMoveCharactor::Update();
			return;
		}

		// さっき行っていた作業が"デスクワーク"なら
		if (pDesk->GetTaskType() == CWorldUICollision::TYPE_PC)
		{
			// プレイヤーの座標を戻す
			SetPos(Player_Info::DESK_RETURNPOS);

			// 角度を変更
			SetRot(VECTOR3_NULL);

			// フラグをリセットする
			m_isPcWork = false;
		}

		// コピー機なら
		if (pDesk->GetTaskType() == CWorldUICollision::TYPE_COPY)
		{
			// フラグリセット
			m_isCopyMachine = false;
		}

		// 起動したタスクを非アクティブにする [add 髙橋]
		pDesk->SetTaskType(pDesk->GetTaskType());

		// タスク中は移動や他の当たり判定をさせないためにリターン
		return;
	}

	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 oldpos = GetOldPos();

	// 速度の調整値
	float MoveSpeed = m_isTaskMaxOver ? 1.5f : player::fSpeed;
	float PadLStick = m_isTaskMaxOver ? 1.75f : 3.75f;

	if (m_nControlTypes == CONTROLTYPE_KEY)
	{
		// キーボード操作
		MoveKeyboard(MoveSpeed);
	}
	// ジョイパッド操作
	else if (m_nControlTypes == CONTROLTYPE_PAD)
	{
		// 十字キーの入力がある場合は十字キーの移動だけを行う
		if (Pad->GetCrossKeyInput(Pad) == true)
		{
			MoveCrossPadButton(MoveSpeed);
		}
		else
		{
			// 十字キーが押されていない場合は、スティックの判定を行う
			MoveJoypad(PadLStick);
		}
	}
	
	// ステートマシンの更新処理
	if (m_pMachine) m_pMachine->Update();

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
			if (isInputKey && !m_isTaskMaxOver)
			{				
				switch (Colliders->nType)
				{
				case CWorldUICollision::TYPE_NONE: // タスクをしていない状態[add 髙橋]
					break;

				case CWorldUICollision::TYPE_COPY:	// コピー機なら
										// nullじゃない状態
					if (pDesk)
					{
						// デスクワーク時
						if (Colliders->nType == CWorldUICollision::TYPE_COPY)
						{
							m_isCopyMachine = true;
						}

						// タスクを起動する
						pDesk->SetTaskType(CWorldUICollision::TYPE(Colliders->nType), true);
					}
					break;

				case CWorldUICollision::TYPE_DOCUMENT: // 書類タスク[add 髙橋]
				
					// 両方がnullじゃない状態
					if (pDesk && (pDesk->GetDOCUMENTDesk()->GetCOPYTaskNum() > 0))
					{
						pDesk->GetDOCUMENTDesk()->SetDOCUMENTValue();
					}

					break;

				case CWorldUICollision::TYPE_OUTSIDE: // 外出タスク[add 髙橋]

					// 両方がnullじゃない状態
					if (pDesk && (pDesk->GetDOCUMENTDesk()->GetDOCUMENTTaskNum() > 0))
					{
						pDesk->GetOutsideDesk()->SetOutside();
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

	// AFKの2DUI更新処理
	UpdateAfkUiState();

	// ブロックとの判定
	UpdateBlockCollision(UpdatePos);

	// オフィス内のドアとの判定
	UpdateSideDoorCollision(UpdatePos,Key,Pad);

	// 自動ドアとの判定
	UpdateAutoDoorCollision(UpdatePos);

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
// さぼりの起動
//=========================================================
void CPlayer::SetAfk(AFKTYPE AfkType, bool bInput)
{
	// サボり判定
	if (AfkType >= AFKTYPE_SMOKE && AfkType <= AFKTYPE_EATING)
	{
		// サボりマネージャーから該当エリアの判定を取得
		bool bInArea = false;
		switch (AfkType)
		{
		case AFKTYPE_SMOKE:      bInArea = CAfkManager::Instance()->GetAfkSmoke()->GetAfk(); break;
		case AFKTYPE_TV:         bInArea = CAfkManager::Instance()->GetAfkTV()->GetAfk(); break;
		case AFKTYPE_MAGAZINE:   bInArea = CAfkManager::Instance()->GetAfkMagazine()->GetAfk(); break;
		case AFKTYPE_GAMECENTER: bInArea = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk(); break;
		case AFKTYPE_EATING:     bInArea = CAfkManager::Instance()->GetAfkEating()->GetAfk(); break;
		default: break;
		}

		// 操作対象のフラグポインタを取得
		bool* pAfkFlag = nullptr;
		switch (AfkType)
		{
		case AFKTYPE_SMOKE:      pAfkFlag = &m_bAfkSmoke; break;
		case AFKTYPE_TV:         pAfkFlag = &m_bAfkTV; break;
		case AFKTYPE_MAGAZINE:   pAfkFlag = &m_bAfkMagazine; break;
		case AFKTYPE_GAMECENTER: pAfkFlag = &m_bAfkGameCenter; break;
		case AFKTYPE_EATING:     pAfkFlag = &m_bAfkEating; break;
		default: break;
		}

		if (pAfkFlag && bInArea && bInput)
		{
			if (!(*pAfkFlag))
			{
				// クールタイム中でなければサボり開始
				if (m_nAfkCoolTime[AfkType] <= 0)
				{
					*pAfkFlag = true;
				}
			}
			else
			{
				// 終了
				*pAfkFlag = false;
				m_nAfkCoolTime[AfkType] = player::AFK_COOL_TIME;

				// サボり終了コマンドが押されたため、対応するUIを起動
				if (m_pCoolTimeUi[AfkType])
				{
					m_pCoolTimeUi[AfkType]->StartSet();
				}
			}
		}
		else if(!bInArea)
		{
			// 終了
			*pAfkFlag = false;
		}
	}

	// ベンチのサボり処理
	else if (AfkType == AFKTYPE_BENCH)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			bool bInArea = CAfkManager::Instance()->GetAfkBench(nCnt)->GetAfk();

			if (bInArea && bInput)
			{
				if (!m_bAfkBench[nCnt])
				{
					// クールタイム中でなければサボり開始
					if (m_nCoolTimeBench[nCnt] <= 0)
					{
						m_bAfkBench[nCnt] = true;
					}
				}
				else
				{
					// 終了
					m_bAfkBench[nCnt] = false;
					m_nCoolTimeBench[nCnt] = player::AFK_COOL_TIME;

					// サボり終了コマンドが押されたため、対応するベンチのUIを起動
					if (m_pCoolTimeUiBench[nCnt])
					{
						m_pCoolTimeUiBench[nCnt]->StartSet();
					}
				}
			}
			else if (!bInArea)
			{
				// エリア外に出た場合の強制終了
				if (m_bAfkBench[nCnt]) m_nCoolTimeBench[nCnt] = player::AFK_COOL_TIME;
				m_bAfkBench[nCnt] = false;
			}
		}
	}

	// サボり中フラグの更新
	m_isEnableLazy = (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine ||
		m_bAfkGameCenter || m_bAfkEating ||
		m_bAfkBench[0] || m_bAfkBench[1] || m_bAfkBench[2] || m_bAfkBench[3]);
}
//=========================================================
// さぼりの2Dui表示
//=========================================================
void CPlayer::UpdateAfkUiState(void)
{
	// afk2Duiクラス取得
	auto pUI = CGameSceneObject::GetInstance()->GetAfk2DUIPointer();
	if (!pUI) return;

	// サボり可能なエリア内かつクールタイム外かを判定するフラグ
	bool bCanAfkAnywhere = false;

	//サボりのチェック
	for (int nType = AFKTYPE_SMOKE; nType <= AFKTYPE_EATING; nType++)
	{
		bool bInArea = false;
		switch (nType)
		{
		case AFKTYPE_SMOKE:      bInArea = CAfkManager::Instance()->GetAfkSmoke()->GetAfk(); break;
		case AFKTYPE_TV:         bInArea = CAfkManager::Instance()->GetAfkTV()->GetAfk(); break;
		case AFKTYPE_MAGAZINE:   bInArea = CAfkManager::Instance()->GetAfkMagazine()->GetAfk(); break;
		case AFKTYPE_GAMECENTER: bInArea = CAfkManager::Instance()->GetAfkGameCenter()->GetAfk(); break;
		case AFKTYPE_EATING:     bInArea = CAfkManager::Instance()->GetAfkEating()->GetAfk(); break;
		default: break;
		}

		// エリア内にいて、かつクールタイム中でなければ表示可能
		if (bInArea && m_nAfkCoolTime[nType] <= 0)
		{
			bCanAfkAnywhere = true;
			break;
		}
	}

	// ベンチのチェック判定
	if (!bCanAfkAnywhere)
	{
		for (int nCnt = 0; nCnt < Player_Bench::BENCH_MAX; nCnt++)
		{
			bool bInArea = CAfkManager::Instance()->GetAfkBench(nCnt)->GetAfk();

			// ベンチのエリア内にいて、かつクールタイム中でなければ表示可能
			if (bInArea && m_nCoolTimeBench[nCnt] <= 0)
			{
				bCanAfkAnywhere = true;
				break;
			}
		}
	}

	// サボり開始可能な状態の時にUIを表示
	bool bShouldDisplay = (!m_isEnableLazy && bCanAfkAnywhere);

	// UIに表示状態を設定
	pUI->SetisDisplay(bShouldDisplay);
}
//=========================================================
// 初期時間を減らすための関数
//=========================================================
void CPlayer::DecleInitTaskTime(void)
{
	// trueなら
	if (m_isInitTaskTime) return;

	// 0以下なら
	if (m_nInitTaskWorkingTime <= 0)
	{
		m_nInitTaskWorkingTime = 0;
		m_isInitTaskTime = true;
		return;
	}

	// デクリメントする
	m_nInitTaskWorkingTime--;
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
//=========================================================
// プレイヤー移動処理(キーボード編)
//=========================================================
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
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating 
		|| m_bAfkBench[0] || m_bAfkBench[1] || m_bAfkBench[2] || m_bAfkBench[3])
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 60)
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
//=========================================================
// プレイヤー移動処理(ジョイパッド編)
//=========================================================
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
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating
		|| m_bAfkBench[0] || m_bAfkBench[1] || m_bAfkBench[2] || m_bAfkBench[3])
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 60)
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
//=========================================================
// 十字キーでの移動バージョン
//=========================================================
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
	if (m_bAfkSmoke || m_bAfkTV || m_bAfkMagazine || m_bAfkGameCenter || m_bAfkEating
		|| m_bAfkBench[0] || m_bAfkBench[1] || m_bAfkBench[2] || m_bAfkBench[3])
	{
		m_nCntAfk++;
		if (m_nCntAfk >= 60)
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
//=========================================================
// ブロックとのコリジョン判定関数わけ
//=========================================================
void CPlayer::UpdateBlockCollision(D3DXVECTOR3 pos)
{
	if (m_isPcWork) return;

	for (int nCnt = 0; nCnt < Player_Bench::BENCH_MAX; nCnt++)
	{
		// スキップする
		if (m_bAfkTV || m_bAfkBench[nCnt]) return;
	}

	// ブロック管理クラスを取得
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
//=========================================================
// 自動ドアとのコリジョン関数分け
//=========================================================
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
//=========================================================
// サイドに開くドアとのコリジョン関数分け
//=========================================================
void CPlayer::UpdateSideDoorCollision(D3DXVECTOR3 pos, CInputKeyboard* key, CJoyPad* pad)
{
	// もし"外回りタスク"が起動されていないなら
	if (!m_isSetOutSideTask)
	{
		return;
	}

	// ポインタ取得
	auto* pSideDoorCollision = CSideOpenDoorCollision::GetInstance();
	auto* pSideDoorManager = CSideOpenDoorManager::GetInstance();	
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
//=========================================================
// 付近の敵の警戒度を下げる関数
//=========================================================
void CPlayer::LowerLevelToEnemy(void)
{
	// 敵管理クラスnullチェック
	if (!m_pEnemyManagerOutSide) return;

	// 敵の全体を取得
	int nNumAll = m_pEnemyManagerOutSide->GetAllEnemys();
	if (nNumAll <= 0) return;

	for (int nCnt = 0; nCnt < nNumAll; nCnt++)
	{
		// 敵単体クラスを取得
		CEnemy* pEnemy = m_pEnemyManagerOutSide->GetEnemyIdx(nCnt);
		if (!pEnemy) continue;

		// 球形範囲に当たっていたら
		if (this->CollisionSphere(pEnemy->GetSphereCollider()))
		{
			// 敵のレベルポイントを下げ,警戒度によるパラメーターを下げる
			pEnemy->LevelDown(10.0f);
			break;
		}
	}
}
//=========================================================
// テレビを向くための計算関数
//=========================================================
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
//=========================================================
// パソコンを向くための計算関数 
//=========================================================
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
//=========================================================
// ベンチの向きを調整するための計算関数 
//=========================================================
void CPlayer::MathBenchRotation(void)
{
	// 元の位置を保存
	m_posOld = GetPos();
	D3DXVECTOR3 VectorBench;

	// 駅のベンチ
	if (m_bAfkBench[0])
	{
		// 座標をセットする
		SetPos(Player_Bench::STATION_CHARACTORPOS);
		// 対象ベクトルを作成
		VectorBench = Player_Bench::STATION_DESTPOS - GetPos();
	}
	// ゲームセンターのベンチ
	if (m_bAfkBench[1])
	{
		// 座標をセットする
		SetPos(Player_Bench::GAMECENTER_CHARACTORPOS);
		// 対象ベクトルを作成
		VectorBench = Player_Bench::GAMECENTER_DESTPOS - GetPos();
	}
	// 居酒屋のベンチ
	if (m_bAfkBench[2])
	{
		// 座標をセットする
		SetPos(Player_Bench::IZAKAYA_CHARACTORPOS);
		// 対象ベクトルを作成
		VectorBench = Player_Bench::IZAKAYA_DESTPOS - GetPos();
	}
	// オフィスのベンチ
	if (m_bAfkBench[3])
	{
		// 座標をセットする
		SetPos(Player_Bench::OFFICE_CHARACTORPOS);
		// 対象ベクトルを作成
		VectorBench = Player_Bench::OFFICE_DESTPOS - GetPos();
	}


	// 回転角を生成
	float fRotY = atan2f(-VectorBench.x, -VectorBench.z);

	// 角度を設定
	SetRotDest(D3DXVECTOR3(0.0f, fRotY, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, fRotY, 0.0f));
}
//=========================================================
// さぼり時のスコア加算関数
//=========================================================
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
				m_nAddScore = 500;
				break;
			case 2:
				m_nAddScore = 800;
				break;
			case 3:
				m_nAddScore = 1000;
				break;
			case 4:
				m_nAddScore = 300;
				break;
			case 5:
				m_nAddScore = 150;
				break;
			case 6:
				m_nAddScore = 50;
				break;
			case 7:
				m_nAddScore = 1;
				break;
			default:
				break;
			}

			// スコア加算
			CGameSceneObject::GetInstance()->GetScoreDitch()->AddScore(m_nAddScore);

			// スコアのポップ生成
			CScorePop::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 45.0f, GetPos().z), m_nAddScore,5.0f,15.0f);

			// 回数を加算
			m_nScoreCnt++;
		}
	}

	// 外回りのさぼりが有効だったら
	if (m_bAfkGameCenter || m_bAfkBench[0] || m_bAfkBench[1]
		|| m_bAfkBench[2] || m_bAfkBench[3])
	{
		m_nTimeScore++;
		if ((60 * m_nScoreCnt) < m_nTimeScore)
		{
			// スコアの加算値上昇
			switch (m_nScoreCnt)
			{
			case 1:
				m_nAddScore = 800;
				break;
			case 2:
				m_nAddScore = 1400;
				break;
			case 3:
				m_nAddScore = 2000;
				break;
			case 4:
				m_nAddScore = 500;
				break;
			case 5:
				m_nAddScore = 250;
				break;
			case 6:
				m_nAddScore = 100;
				break;
			case 7:
				m_nAddScore = 1;
				break;
			default:
				break;
			}

			// 加算 ( サボり )
			CGameSceneObject::GetInstance()->GetScoreDitch()->AddScore(m_nAddScore);

			// スコアのポップ生成
			CScorePop::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 40.0f, GetPos().z), m_nAddScore, 15.0f, 25.0f);

			// カウント加算
			m_nScoreCnt++;
		}
	}

	// もしさぼり全部が無効だったら
	if (!m_bAfkSmoke && !m_bAfkTV && !m_bAfkMagazine && !m_bAfkGameCenter && !m_bAfkEating 
		&& !m_bAfkBench[0] && !m_bAfkBench[1] && !m_bAfkBench[2] && !m_bAfkBench[3])
	{
		// スコア加算値の上昇カウントとスコア加算タイムをリセット
		m_nScoreCnt = 1;
		m_nTimeScore = NULL;
	}
}
//=========================================================
// 追跡状態からタスクになったかどうか
//=========================================================
bool CPlayer::IsTaskWorking(void) const
{
	// PCデスク作業中
	if (m_isPcWork) return true;
	if (m_isCopyMachine) return true;

	// タスク起動中
	CDeskwork* pDesk = CGameSceneObject::GetInstance()->GetDesk();

	// nullじゃないなら
	if (pDesk)
	{
		// タスクの種類を取得
		auto type = pDesk->GetTaskType();

		// 作業タスク画面に入っているか
		if (type != CWorldUICollision::TYPE_NONE && type != CWorldUICollision::TYPE_DOCUMENT)
		{
			return true;
		}
	}
	return false;
}