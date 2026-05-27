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

//*********************************************************
// 定数宣言
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

//=========================================================
// コンストラクタ
//=========================================================
CCamera::CCamera() : m_pCamera(ClearDefault()),
m_pThirdPersonPos(VECTOR3_NULL),
m_isMove(false)
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
	m_isMove = false;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CCamera::Uninit(void)
{

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

	// ゲームのみ追従カメラ設定
	if (CManager::GetInstance()->GetScene() == CScene::MODE_GAME)
	{
		// 追従モードならそのカメラに設定
		if (m_pCamera.nMode == MODE_THIRD)
		{
			ThirdPersonView();

			// 移動フラグがfalseならフリック対応
			if (!m_isMove) FollowMouse();
		}
	}
	else if (CManager::GetInstance()->GetScene() == CScene::MODE_RESULT)
	{
		// 固定カメラに設定
		ResultCamera();
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

#ifdef _DEBUG
	// デバッグ表示
	CDebugproc::Print("Camera : PosV [ %.2f, %.2f, %.2f ]\n", m_pCamera.posV.x, m_pCamera.posV.y, m_pCamera.posV.z);
	CDebugproc::Draw(0, 20);

	CDebugproc::Print("Camera : PosR [ %.2f, %.2f, %.2f ]\n", m_pCamera.posR.x, m_pCamera.posR.y, m_pCamera.posR.z);
	CDebugproc::Draw(0, 40);

	CDebugproc::Print("Camera : Rot [ %.2f, %.2f, %.2f ]\n", m_pCamera.rot.x, m_pCamera.rot.y, m_pCamera.rot.z);
	CDebugproc::Draw(0, 80);
#endif // _DEBUG
}
//==============================================================
// マウス操作の視点移動
//==============================================================
void CCamera::MouseView(CInputMouse * pMouse)
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
}
//==============================================================
// カメラから見て透過させる時の当たり判定関数
//==============================================================
bool CCamera::CollisionTorayBlock(CPlayer* pPlayer, CBlock* pBlock)
{
	// null値チェック
	if (!pPlayer || !pBlock) return false;

	// 判定用変数
	bool isCollision = false;

	// プレイヤー情報取得
	const auto& PlayerPos = pPlayer->GetPos();

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