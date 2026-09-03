//=========================================================
//
// カメラ処理 [ camera.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "camera.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "template.h"
#include "player.h"
#include "block.h"
#include "automatic_door.h"
#include "sideopendoor.h"
#include "titleuimanager.h"
#include "boss.h"

//*********************************************************
// 定数名前空間宣言
//*********************************************************
namespace CAMERAINFO
{
	constexpr float MAX_VIEWUP = 3.0f;			// カメラの角度制限値
	constexpr float MAX_VIEWDOWN = 0.1f;		// カメラの角度制限値
	constexpr float NorRot = D3DX_PI * 2.0f;	// 正規化値

	const D3DXVECTOR3 InitPos = { 0.0f, 1000.0f, -800.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitRot = { D3DX_PI * 0.6f, 0.0f, 0.0f }; // カメラ初期角度
	const D3DXVECTOR3 InitVecU = { 0.0f, 1.0f, 0.0f };			// 初期ベクトル
}

//*********************************************************
// タイトル用の定数名前空間宣言
//*********************************************************
namespace TITLECAMERAINFO
{
	const D3DXVECTOR3 InitPos = { -178.0f, 166.0f, -63.0f };	// カメラ初期座標
	const D3DXVECTOR3 InitPosR = { -2.3f, 0.0f, -230.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitRot = { 2.02f, 0.54f, 0.0f };			// カメラ初期角度
	const D3DXVECTOR3 InitVecU = { 0.0f, 1.0f, 0.0f };			// 初期ベクトル
	constexpr float Distance = 380.0f;							// 初期の距離
}

//*********************************************************
// ランキング用の定数名前空間宣言
//*********************************************************
namespace RANKINGCAMERAINFO
{
	const D3DXVECTOR3 InitPosV = { 405.0f,40.0f, -127.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitPosR = { 415.0f,40.0f, -127.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitRot = { 1.57f, 1.57f, 0.0f };			// カメラ初期角度
	const D3DXVECTOR3 InitVecU = { 0.0f, 1.0f, 0.0f };			// 初期ベクトル
	constexpr float Distance = 10.0f;							// 初期の距離
}

//*********************************************************
// 失敗リザルト用の定数名前空間宣言
//*********************************************************
namespace CATCHINFO
{
	const D3DXVECTOR3 InitPosV = { -4.0f,106.0f, 145.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitPosR = { 0.8f,-143.0f, 620.0f };		// カメラ初期座標
	const D3DXVECTOR3 InitRot = { VECTOR3_NULL };			// カメラ初期角度
	const D3DXVECTOR3 InitVecU = { 0.0f, 1.0f, 0.0f };			// 初期ベクトル
	constexpr float Distance = 540.0f;							// 初期の距離
}

//=========================================================
// コンストラクタ
//=========================================================
CCamera::CCamera() : m_pCamera(ClearDefault()),
m_pThirdPersonPos(VECTOR3_NULL),
m_nControlTypes(CONTROLTYPE_NONE),
m_isMove(false),
m_pCharactor(nullptr),
m_currentAnim{},
m_isAnimating(false),
m_nCurrentFrame(NULL),
m_nTotalFrames(NULL),
m_TargetPosV(VECTOR3_NULL),
m_TargetPosR(VECTOR3_NULL),
m_TargetRot(VECTOR3_NULL),
m_fLerpRate(NULL),
m_nBossCamWaitCount(NULL),
m_pBoss(nullptr),
m_isFinishBossMovie(false)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CCamera::~CCamera()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CCamera::Init(void)
{
	m_pCamera.posV = CAMERAINFO::InitPos;	// カメラの位置
	m_pCamera.posR = VECTOR3_NULL;			// カメラの見ている位置
	m_pCamera.vecU = CAMERAINFO::InitVecU;	// 上方向ベクトル
	m_pCamera.rot = CAMERAINFO::InitRot;	// 角度

	// 距離を計算
	float fRotx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fRoty = m_pCamera.posV.y - m_pCamera.posR.y;
	float fRotz = m_pCamera.posV.z - m_pCamera.posR.z;

	// 視点から注視点までの距離
	m_pCamera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));

	// 移動フラグ
	m_currentAnim.AnimData.clear();
	m_isMove = false;
	m_isFinishBossMovie = false;

	// 操作の種類を設定する (パッドかキーマウかどうか)
	m_nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CCamera::Uninit(void)
{
	m_currentAnim.AnimData.clear();
	m_pCharactor = nullptr;
}
//=========================================================
// 更新処理
//=========================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	// マウスでのカメラ更新
	MouseView(CManager::GetInstance()->GetMouse());
#endif

	// ボスムービーなら
	if (m_pCamera.nMode == MODE_BOSS_SYSTEM)
	{
		// 有効なら
		if (m_isFinishBossMovie == true)
		{
			m_pCamera.nMode = MODE_THIRD;
			return;
		}

		// 追従変更
		UpdateBossCamera();

		// 追従関係の更新処理
		if (m_pBoss->GetActiveFlags())
		{
			D3DXVECTOR3 B_pos = m_pBoss->GetPos();
			UpdateFollowBoss({ B_pos.x,B_pos .y + 50.0f,B_pos .z});
		}
		return;
	}

	// もしアニメーションモードだったら
	if (m_pCamera.nMode == CCamera::MODE_ANIM && m_isAnimating)
	{
		// アニメーション更新だけしてreturnする
		UpdateAnim();
		return;
	}

	// 追従カメラ設定
	if (CManager::GetInstance()->GetScene() == CScene::MODE_GAME || 
		CManager::GetInstance()->GetScene() == CScene::MODE_TUTORIAL)
	{
		// 追従モードならそのカメラに設定
		if (m_pCamera.nMode == MODE_THIRD)
		{
			// 追従視点
			ThirdPersonView();

			// 移動フラグがfalseならフリック対応
			if (m_nControlTypes == CONTROLTYPE_KEY && !m_isMove) FollowMouse();
			if (m_nControlTypes == CONTROLTYPE_PAD && !m_isMove) FollowJoyPad();
		}
	}
	else if (CManager::GetInstance()->GetScene() == CScene::MODE_TITLE)
	{
		// 固定カメラに設定
		TitleCamera();
	}
	else if (CManager::GetInstance()->GetScene() == CScene::MODE_RESULT || 
			CManager::GetInstance()->GetScene() == CScene::MODE_OVERWORK)
	{
		// 固定カメラに設定
		ResultCamera();
	}
	else if (CManager::GetInstance()->GetScene() == CScene::MODE_RANKING)
	{
		// 固定カメラに設定
		RankingCamera();
	}
	else if (CManager::GetInstance()->GetScene() == CScene::MODE_LOSELAZY ||
			m_pCamera.nMode == MODE_LAZYMISS)
	{
		// 固定カメラに設定
		CastCamera();
	}

	// 角度の正規化
	if (m_pCamera.rot.y > D3DX_PI)
	{// D3DX_PIより大きくなったら
		m_pCamera.rot.y -= CAMERAINFO::NorRot;
	}

	// 角度の正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{// D3DX_PIより小さくなったら
		m_pCamera.rot.y += CAMERAINFO::NorRot;
	}

#ifdef _DEBUG
	// デバッグ表示
	CDebugproc::GetInstance()->Print("Camera : PosV [ %.2f, %.2f, %.2f ]\n", m_pCamera.posV.x, m_pCamera.posV.y, m_pCamera.posV.z);
	CDebugproc::GetInstance()->Print("Camera : PosR [ %.2f, %.2f, %.2f ]\n", m_pCamera.posR.x, m_pCamera.posR.y, m_pCamera.posR.z);
	CDebugproc::GetInstance()->Print("Camera : Rot [ %.2f, %.2f, %.2f ]\n", m_pCamera.rot.x, m_pCamera.rot.y, m_pCamera.rot.z);
#endif // _DEBUG
}
//=========================================================
// カメラをセット
//=========================================================
void CCamera::SetCamera(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_pCamera.mtxView,
		&m_pCamera.posV,
		&m_pCamera.posR,
		&m_pCamera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxprojection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_pCamera.mtxprojection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		6000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera.mtxprojection);

}
//==============================================================
// マウス操作の視点移動
//==============================================================
void CCamera::MouseView(CInputMouse * pMouse)
{
	// 左のALT押しているときのみ
	if (CManager::GetInstance()->GetInputKeyboard()->GetPress(DIK_LALT))
	{
		// 左クリック
		if (pMouse->GetPress(CInputMouse::MOUSE_LEFT))
		{
			// マウスの移動量取得
			D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
			D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

			// 現在の角度を計算
			D3DXVECTOR2 fAngle = Move - MoveOld;

			// 回転量を更新
			m_pCamera.rot.y += fAngle.x * 0.01f;
			m_pCamera.rot.x += fAngle.y * 0.01f;

			// 回転量を制限
			if (m_pCamera.rot.x > CAMERAINFO::MAX_VIEWUP)
			{
				m_pCamera.rot.x -= fAngle.y * 0.01f;
			}
			else if (m_pCamera.rot.x < CAMERAINFO::MAX_VIEWDOWN)
			{
				m_pCamera.rot.x -= fAngle.y * 0.01f;
			}

			// カメラの視点の情報
			m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
			m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
			m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
		}
		// 右クリック
		else if (pMouse->GetPress(CInputMouse::MOUSE_RIGHT))
		{
			D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
			D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

			D3DXVECTOR2 fAngle = Move - MoveOld;

			// 回転量を更新
			m_pCamera.rot.y += fAngle.x * 0.01f;
			m_pCamera.rot.x += fAngle.y * 0.01f;

			// 回転量を制限
			if (m_pCamera.rot.x > CAMERAINFO::MAX_VIEWUP)
			{
				m_pCamera.rot.x -= fAngle.y * 0.01f;
			}
			else if (m_pCamera.rot.x < CAMERAINFO::MAX_VIEWDOWN)
			{
				m_pCamera.rot.x -= fAngle.y * 0.01f;
			}

			// カメラ座標を更新
			m_pCamera.posR.x = m_pCamera.posV.x + sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
			m_pCamera.posR.y = m_pCamera.posV.y + cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
			m_pCamera.posR.z = m_pCamera.posV.z + sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
		}
	}

	// 正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{
		m_pCamera.rot.y += CAMERAINFO::NorRot;
	}
	else if (m_pCamera.rot.y > D3DX_PI)
	{
		m_pCamera.rot.y += -CAMERAINFO::NorRot;
	}
	if (m_pCamera.rot.x < -D3DX_PI)
	{
		m_pCamera.rot.x += CAMERAINFO::NorRot;
	}
	else if (m_pCamera.rot.x > D3DX_PI)
	{
		m_pCamera.rot.x += -CAMERAINFO::NorRot;
	}
}
//==============================================================
// マウスのフリック移動更新
//==============================================================
void CCamera::FollowMouse(void)
{
	// マウス状態を設定
	DIMOUSESTATE mouseState;

	// マウスの軸移動設定
	if (CManager::GetInstance()->GetMouse()->GetState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00045f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		m_pCamera.rot.y += X;
		m_pCamera.rot.x += Y;

		if (m_pCamera.rot.y < -D3DX_PI)
		{
			m_pCamera.rot.y += D3DX_PI * 2.0f;
		}
		else if (m_pCamera.rot.y > D3DX_PI)
		{
			m_pCamera.rot.y += -D3DX_PI * 2.0f;
		}

		if (m_pCamera.rot.x < -D3DX_PI)
		{
			m_pCamera.rot.x += D3DX_PI * 2.0f;
		}
		else if (m_pCamera.rot.x > D3DX_PI)
		{
			m_pCamera.rot.x += -D3DX_PI * 2.0f;
		}

		if (m_pCamera.rot.x > 3.00f)
		{
			m_pCamera.rot.x -= Y;
		}
		else if (m_pCamera.rot.x < 0.1f)
		{
			m_pCamera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);
	}
}
//==============================================================
// マウスホイール処理
//==============================================================
void CCamera::WheelMouse(int nDelta)
{
	if (nDelta > 0)
	{// マウスの値が正
		m_pCamera.fDistance -= 20.0f;
	}
	else if (nDelta < 0)
	{// マウスの値が負
		m_pCamera.fDistance += 20.0f;
	}

	if (m_pCamera.fDistance <= 50.0f)
	{// 50.0f以下なら
		m_pCamera.fDistance = 50.0f;
	}

	// カメラの視点の情報
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
}
//==============================================================
// 三人称視点カメラ
//==============================================================
void CCamera::ThirdPersonView(void)
{
	// カメラの注視点座標を設定
	m_pCamera.posR = m_pThirdPersonPos;

	//カメラの設定
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
}
//==============================================================
// リザルトのカメラ処理
//==============================================================
void CCamera::ResultCamera(void)
{
	m_pCamera.posV = TITLECAMERAINFO::InitPos;			// カメラの位置
	m_pCamera.posR = VECTOR3_NULL;						// カメラの見ている位置
	m_pCamera.vecU = TITLECAMERAINFO::InitVecU;			// 上方向ベクトル
	m_pCamera.rot = TITLECAMERAINFO::InitRot;			// 角度
	m_pCamera.fDistance = TITLECAMERAINFO::Distance;	// 距離
}
//==============================================================
// タイトルのカメラ処理
//==============================================================
void CCamera::TitleCamera(void)
{
	m_pCamera.posV = TITLECAMERAINFO::InitPos;			// カメラの位置
	m_pCamera.posR = VECTOR3_NULL;						// カメラの見ている位置
	m_pCamera.vecU = TITLECAMERAINFO::InitVecU;			// 上方向ベクトル
	m_pCamera.rot = TITLECAMERAINFO::InitRot;			// 角度
	m_pCamera.fDistance = TITLECAMERAINFO::Distance;	// 距離
}
//==============================================================
// ランキングのカメラ処理
//==============================================================
void CCamera::RankingCamera(void)
{
	m_pCamera.posV = RANKINGCAMERAINFO::InitPosV;			// カメラの位置
	m_pCamera.posR = RANKINGCAMERAINFO::InitPosR;			// カメラの見ている位置
	m_pCamera.vecU = RANKINGCAMERAINFO::InitVecU;			// 上方向ベクトル
	m_pCamera.rot = RANKINGCAMERAINFO::InitRot;				// 角度
	m_pCamera.fDistance = RANKINGCAMERAINFO::Distance;		// 距離
}
//==============================================================
// カメラから見て透過させる時の当たり判定関数
//==============================================================
bool CCamera::CollisionTorayBlock(CBlock* pBlock)
{
	// null値チェック
	if (!m_pCharactor || !pBlock) return false;

	// 判定用変数
	bool isCollision = false;

	// プレイヤー情報取得
	const auto& PlayerPos = m_pCharactor->GetPos();

	// 判定ブロック情報
	const auto& BlockPos = pBlock->GetPos();
	const auto& BlockSize = pBlock->GetSize();

	// ベクトル線分情報
	D3DXVECTOR3 VecDir = PlayerPos - m_pCamera.posV;

	// 長さを取得
	float fLength = D3DXVec3Length(&VecDir);
	if (fLength < 1e-6f) return false;

	// ベクトルの正規化
	D3DXVec3Normalize(&VecDir, &VecDir);

	// モデルのサイズ判定
	float radius = max(BlockSize.x, max(BlockSize.y, BlockSize.z)) * 0.5f;

	// 線分と球の最短距離を判定する
	D3DXVECTOR3 MathLength = BlockPos - m_pCamera.posV;
	float MathDot = D3DXVec3Dot(&MathLength, &VecDir);
	MathDot = max(0.0f, min(fLength, MathDot));

	// ベクトル分の計算
	D3DXVECTOR3 close = m_pCamera.posV + VecDir * MathDot;

	// 差分を計算
	D3DXVECTOR3 Diff = close - BlockPos;

	// 長さの2剰計算
	float distSq = D3DXVec3LengthSq(&Diff);

	// 長さが以下だったら
	if (distSq <= (radius * radius))
	{
		isCollision = true;
	}
	else
	{
		isCollision = false;
	}

	return isCollision;
}
//==============================================================
// 別クラス判別バージョン
//==============================================================
bool CCamera::CollisionTorayDoor(CAutoMaticDoor* pDoor)
{
	// null値チェック
	if (!m_pCharactor || !pDoor) return false;

	// 判定用変数
	bool isCollision = false;

	// プレイヤー情報取得
	const auto& PlayerPos = m_pCharactor->GetPos();

	// 判定ブロック情報
	const auto& BlockPos = pDoor->GetPos();
	const auto& BlockSize = pDoor->GetSize();

	// ベクトル線分情報
	D3DXVECTOR3 VecDir = PlayerPos - m_pCamera.posV;

	// 長さを取得
	float fLength = D3DXVec3Length(&VecDir);
	if (fLength < 1e-6f) return false;

	// ベクトルの正規化
	D3DXVec3Normalize(&VecDir, &VecDir);

	// モデルのサイズ判定
	float radius = max(BlockSize.x, max(BlockSize.y, BlockSize.z)) * 0.5f;

	// 線分と球の最短距離を判定する
	D3DXVECTOR3 MathLength = BlockPos - m_pCamera.posV;
	float MathDot = D3DXVec3Dot(&MathLength, &VecDir);
	MathDot = max(0.0f, min(fLength, MathDot));

	// ベクトル分の計算
	D3DXVECTOR3 close = m_pCamera.posV + VecDir * MathDot;

	// 差分を計算
	D3DXVECTOR3 Diff = close - BlockPos;

	// 長さの2剰計算
	float distSq = D3DXVec3LengthSq(&Diff);

	// 長さが以下だったら
	if (distSq <= (radius * radius))
	{
		isCollision = true;
	}
	else
	{
		isCollision = false;
	}

	return isCollision;
}
//==============================================================
// 別クラス判別バージョン
//==============================================================
bool CCamera::CollisionToraySide(CSideOpenDoor* pDoor)
{
	// null値チェック
	if (!m_pCharactor || !pDoor) return false;

	// 判定用変数
	bool isCollision = false;

	// プレイヤー情報取得
	const auto& PlayerPos = m_pCharactor->GetPos();

	// 判定ブロック情報
	const auto& BlockPos = pDoor->GetPos();
	const auto& BlockSize = pDoor->GetSize();

	// ベクトル線分情報
	D3DXVECTOR3 VecDir = PlayerPos - m_pCamera.posV;

	// 長さを取得
	float fLength = D3DXVec3Length(&VecDir);
	if (fLength < 1e-6f) return false;

	// ベクトルの正規化
	D3DXVec3Normalize(&VecDir, &VecDir);

	// モデルのサイズ判定
	float radius = max(BlockSize.x, max(BlockSize.y, BlockSize.z)) * 0.5f;

	// 線分と球の最短距離を判定する
	D3DXVECTOR3 MathLength = BlockPos - m_pCamera.posV;
	float MathDot = D3DXVec3Dot(&MathLength, &VecDir);
	MathDot = max(0.0f, min(fLength, MathDot));

	// ベクトル分の計算
	D3DXVECTOR3 close = m_pCamera.posV + VecDir * MathDot;

	// 差分を計算
	D3DXVECTOR3 Diff = close - BlockPos;

	// 長さの2剰計算
	float distSq = D3DXVec3LengthSq(&Diff);

	// 長さが以下だったら
	if (distSq <= (radius * radius))
	{
		isCollision = true;
	}
	else
	{
		isCollision = false;
	}

	return isCollision;
}
//==============================================================
// アニメーション起動
//==============================================================
void CCamera::PlayAnimation(const AnimData& data)
{
	// データがないなら
	if (data.AnimData.empty()) return;

	// 各情報を初期化、代入
	m_currentAnim = data;
	m_isAnimating = true;
	m_nCurrentFrame = 0;
	m_pCamera.nCntAnim = 0;
}
//==============================================================
// アニメーション更新
//==============================================================
void CCamera::UpdateAnim(void)
{
	// データが空、またはキーが1つしかない場合は処理しない
	if (m_currentAnim.AnimData.empty() || m_currentAnim.AnimData.size() < 2)
	{
		// フラグリセットする
		m_isAnimating = false;

		// カメラモードを"追従"に変更する
		m_pCamera.nMode = MODE_THIRD;
		return;
	}

	// キー数を取得
	int numKeys = (int)m_currentAnim.AnimData.size();

	// 次のキ―
	int nextKey = m_nCurrentFrame + 1;

	// ループ時のインデックス巡回処理
	if (m_currentAnim.isLoop)
	{
		nextKey %= numKeys;
	}

	const auto& curKeyData = m_currentAnim.AnimData[m_nCurrentFrame];
	const auto& nxtKeyData = m_currentAnim.AnimData[nextKey];

	int targetFrame = (curKeyData.nAnimFrame > 0) ? curKeyData.nAnimFrame : 1;
	float fRateFrame = (float)m_pCamera.nCntAnim / (float)targetFrame;
	if (fRateFrame > 1.0f) fRateFrame = 1.0f;

	// 各種差分算出
	D3DXVECTOR3 diffPosV = nxtKeyData.posV - curKeyData.posV;
	D3DXVECTOR3 diffPosR = nxtKeyData.posR - curKeyData.posR;
	D3DXVECTOR3 diffRot = nxtKeyData.rot - curKeyData.rot;

	// 角度の正規化
	auto NormalizeAngle = [](float& angle)
	{
		if (angle > D3DX_PI)  angle -= CAMERAINFO::NorRot;
		if (angle < -D3DX_PI) angle += CAMERAINFO::NorRot;
	};

	// 正規化ラムダ式
	NormalizeAngle(diffRot.x);
	NormalizeAngle(diffRot.y);
	NormalizeAngle(diffRot.z);

	// カメラパラメータの線形補間適用
	m_pCamera.posV = curKeyData.posV + diffPosV * fRateFrame;
	m_pCamera.posR = curKeyData.posR + diffPosR * fRateFrame;
	m_pCamera.rot = curKeyData.rot + diffRot * fRateFrame;
	m_pCamera.fDistance = curKeyData.fDistance + (nxtKeyData.fDistance - curKeyData.fDistance) * fRateFrame;

	// フレームカウンタ進行管理
	if (m_pCamera.nCntAnim >= targetFrame)
	{
		m_nCurrentFrame++;
		m_pCamera.nCntAnim = 0;

		if (m_nCurrentFrame >= numKeys - 1)
		{
			if (m_currentAnim.isLoop)
			{
				// 最初に戻す
				m_nCurrentFrame = 0;
			}
			else
			{
				// ループじゃないとき
				const auto& lastKey = m_currentAnim.AnimData.back();
				m_pCamera.posV = lastKey.posV;
				m_pCamera.posR = lastKey.posR;
				m_pCamera.rot = lastKey.rot;
				m_pCamera.fDistance = lastKey.fDistance;

				// カウンタ初期化＆停止
				m_nCurrentFrame = 0;
				m_pCamera.nCntAnim = 0;
				m_isAnimating = false;
			}
		}
	}
	else
	{
		m_pCamera.nCntAnim++;
	}
}
//==============================================================
// アニメーション読み込み
//==============================================================
HRESULT CCamera::LoadAnimation(const std::string& path)
{
	// ファイル読み込み
	std::ifstream file(path);

	// 例外チェック
	if (!file.is_open()) return E_FAIL;

	// ローカル保存データ
	AnimData tempAnim;
	tempAnim.isLoop = false;

	// 読み込むラインと現在データの変数設定
	std::string line;
	bool inKeySet = false;
	AnimDataKey currentKey = {};

	// 読み込むラインがなくなるまで回す
	while (std::getline(file, line))
	{
		// 無かったらスキップ
		if (line.empty()) continue;

		// "//"と"#"を探してコメントアウトを除去
		size_t commentPos = line.find("//");
		if (commentPos != std::string::npos) line = line.substr(0, commentPos);
		commentPos = line.find("#");
		if (commentPos != std::string::npos) line = line.substr(0, commentPos);

		// 読んだ部分とトークンを設定する
		std::stringstream ss(line);
		std::string token;
		ss >> token;

		// トークンが無かったらスキップ
		if (token.empty()) continue;

		if (token == "LOOP")
		{
			char equal;
			int loopVal;
			ss >> equal >> loopVal;
			tempAnim.isLoop = (loopVal != 0);
		}
		else if (token == "NUM_ALLKEY")
		{
			char equal;
			int numKeys;
			ss >> equal >> numKeys;

			// サイズの確保
			tempAnim.AnimData.reserve(numKeys);
		}
		else if (token == "KEYSET")
		{
			inKeySet = true;

			// キー情報を初期化
			currentKey = AnimDataKey{};
		}
		else if (token == "END_KEYSET")
		{
			// 動的配列に追加
			tempAnim.AnimData.push_back(currentKey);

			// セットフラグ無効化
			inKeySet = false;
		}
		else if (inKeySet)
		{
			// 各トークンごとの処理
			if (token == "FRAME")
			{
				char equal; 
				ss >> equal >> currentKey.nAnimFrame;
			}
			else if (token == "POSV")
			{
				char equal; 
				ss >> equal >> currentKey.posV.x >> currentKey.posV.y >> currentKey.posV.z;
			}
			else if (token == "POSR")
			{
				char equal;
				ss >> equal >> currentKey.posR.x >> currentKey.posR.y >> currentKey.posR.z;
			}
			else if (token == "ROT")
			{
				char equal; 
				ss >> equal >> currentKey.rot.x >> currentKey.rot.y >> currentKey.rot.z;
			}
			else if (token == "DISTANCE")
			{
				char equal; 
				ss >> equal >> currentKey.fDistance;
			}
		}

		// 最終行ならwhile終了
		if (token == "END_ANIMCAMERA") break;
	}

	// ファイル閉じる
	file.close();

	// アニメーションの再生をする
	if (!tempAnim.AnimData.empty())
	{
		// 現在のデータに入れる
		m_currentAnim = tempAnim;
		m_isAnimating = true;
		m_pCamera.nMode = CCamera::MODE_ANIM;

		return S_OK;
	}

	return E_FAIL;
}
//==============================================================
// 値のクリア関数
//==============================================================
CCamera::Camera CCamera::ClearDefault(void)
{
	// 要素のクリア
	m_pCamera.fDistance = NULL;
	m_pCamera.mtxprojection = {};
	m_pCamera.mtxView = {};
	m_pCamera.nMode = MODE_NONE;
	m_pCamera.posR = VECTOR3_NULL;
	m_pCamera.posRDest = VECTOR3_NULL;
	m_pCamera.posV = VECTOR3_NULL;
	m_pCamera.rot = VECTOR3_NULL;
	m_pCamera.vecU = VECTOR3_NULL;

	return m_pCamera;
}
//==============================================================
// パッド操作関数
//==============================================================
void CCamera::FollowJoyPad(void)
{
	// スティック
	auto pad = CManager::GetInstance()->GetJoyPad();
	XINPUT_STATE* pStick = pad->GetStickAngle();

	if (pad->GetRightStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag >= DeadZone)
		{
			float NormalizeX = RStickAngleX / fMag;
			float NormalizeY = RStickAngleY / fMag;

			float fAngle = fMag * 0.000003f;
			m_pCamera.rot.y += NormalizeX * 0.5f * fAngle;
			m_pCamera.rot.x -= NormalizeY * 0.5f * fAngle;
		}
	}

	// Y軸（水平回転）の正規化
	if (m_pCamera.rot.y > D3DX_PI)  m_pCamera.rot.y -= D3DX_PI * 2.0f;
	if (m_pCamera.rot.y < -D3DX_PI) m_pCamera.rot.y += D3DX_PI * 2.0f;

	// X軸（上下回転）の制限
	if (m_pCamera.rot.x <= 1.0f)  m_pCamera.rot.x = 1.0f;
	if (m_pCamera.rot.x >= 2.6f)  m_pCamera.rot.x = 2.6f;
}
//==============================================================
// ボス関連のシステム設定 ( 基本派この関数の中で設定する)
//==============================================================
void CCamera::SetBossSysytem(const D3DXVECTOR3& targetPosV, const D3DXVECTOR3& targetPosR, const D3DXVECTOR3& targetRot)
{
	// モードを一時的に変更
	m_pCamera.nMode = MODE_BOSS_SYSTEM;

	// カメラの値設定
	m_TargetPosV = targetPosV;
	m_TargetPosR = targetPosR;
	m_TargetRot = targetRot;
	m_fLerpRate = 0.0f;
}
//==============================================================
// 捕まった時のカメ設定
//==============================================================
void CCamera::CastCamera(void)
{
	m_pCamera.posV = CATCHINFO::InitPosV;			// カメラの位置
	m_pCamera.posR = CATCHINFO::InitPosR;			// カメラの見ている位置
	m_pCamera.vecU = CATCHINFO::InitVecU;			// 上方向ベクトル
	m_pCamera.rot = CATCHINFO::InitRot;				// 角度
	m_pCamera.fDistance = CATCHINFO::Distance;		// 距離
}
//==============================================================
// ボスの時のカメラ更新
//==============================================================
void CCamera::UpdateBossCamera(void)
{
	const float fLerpFactor = 0.05f;

	D3DXVec3Lerp(&m_pCamera.posV, &m_pCamera.posV, &m_TargetPosV, fLerpFactor);
	D3DXVec3Lerp(&m_pCamera.posR, &m_pCamera.posR, &m_TargetPosR, fLerpFactor);
	D3DXVec3Lerp(&m_pCamera.rot, &m_pCamera.rot, &m_TargetRot, fLerpFactor);

	// 目標座標への到着判定
	D3DXVECTOR3 diff = m_TargetPosV - m_pCamera.posV;
	float fDistanceSq = D3DXVec3LengthSq(&diff);

	// 距離の2乗が一定値以下かつ、まだボスが未起動の場合
	if (fDistanceSq < 1.0f)
	{
		if (m_pBoss && !m_pBoss->GetActiveFlags())
		{
			// ボス行動開始
			m_pBoss->SetActiveFlags(true);

			// 追従対象を一時的にボスにする場合
			UpdateFollowBoss(m_pBoss->GetPos());
		}
	}

	// 通常追従に戻す
	if (m_pBoss && m_pBoss->GetOutSideIn())
	{
		// 軽い停止時間
		const int MAXTIME = 10;

		// インクリメント
		if (m_nBossCamWaitCount < MAXTIME)
		{
			m_nBossCamWaitCount++;
		}
		else
		{
			// カウント初期化
			m_nBossCamWaitCount = 0;

			// 三人称追従モードに戻す
			m_pCamera.nMode = MODE_THIRD;

			// 終わりのフラグ
			m_isFinishBossMovie = true;
			return;
		}
	}
}
//==============================================================
// 単純にボスを追従するカメラ
//==============================================================
void CCamera::UpdateFollowBoss(const D3DXVECTOR3& FollowtargetPos)
{
	// 注視点設定
	m_pCamera.posR = FollowtargetPos;

	//カメラの設定
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
}