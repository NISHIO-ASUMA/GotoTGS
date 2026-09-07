//=========================================================
//
// カメラアニメーション時に出すUI [ cameraui.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "cameraui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "camera.h"

//========================================================
// コンストラクタ
//========================================================
CCameraUI::CCameraUI(int nPriority) : CObject2D(nPriority),
m_Movement(MOVEMENT_NONE),
m_nLifeInterval(NULL),
m_nDirType(NULL),
m_TargetPos(VECTOR3_NULL)
{

}
//========================================================
// デストラクタ
//========================================================
CCameraUI::~CCameraUI()
{

}
//========================================================
// 生成処理
//========================================================
CCameraUI* CCameraUI::Create
(
	const D3DXVECTOR3& pos,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	const int& nMaxInterval,
	const int& nMoveStartSetNumber,
	const int& nDirType
)
{
	// インスタンス生成
	CCameraUI* pUi = new CCameraUI;
	if (pUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init())) return nullptr;

	// オブジェクト基本設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);
	pUi->SetTexture(Filename);
	pUi->SetAnchor();
	pUi->SetInterval(nMaxInterval);
	pUi->SetTypeDir(nDirType);
	pUi->SetMoveType(nMoveStartSetNumber);

	// 座標設定
	D3DXVECTOR3 targetPos = pos;
	float fCenterY = 45.0f;

	if (nDirType == 0)
	{
		// 下から上方向へ進む場合の目標座標設定
		targetPos.y = targetPos.y - fCenterY;
	}
	else
	{
		// 上から下方向へ進む場合の目標座標設定
		targetPos.y = targetPos.y + fCenterY;
	}

	// 進行座標の設定
	pUi->m_TargetPos = targetPos;

	
	return pUi;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CCameraUI::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CCameraUI::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CCameraUI::Update(void)
{
	// 現在座標を取得
	D3DXVECTOR3 NowPos = GetPos();

	// 画面中央のY座標（退場時の上下判定基準）
	float fCenterY = SCREEN_HEIGHT * 0.5f;

	// 移動アニメーション分岐
	switch (m_Movement)
	{
	case MOVEMENT_NONE:
		break;

	case MOVEMENT_START: // 目標座標へ移動
	{
		if (m_nDirType == 1) // 上から下へ移動
		{
			if (NowPos.y < m_TargetPos.y)
			{
				NowPos.y += 2.0f;

				if (NowPos.y >= m_TargetPos.y)
				{
					NowPos.y = m_TargetPos.y;
					m_Movement = MOVEMENT_STOP; // 到達したら停止状態へ
				}
				SetPos(NowPos);
			}
		}
		else // 下から上へ移動
		{
			if (NowPos.y > m_TargetPos.y)
			{
				NowPos.y -= 2.0f;

				if (NowPos.y <= m_TargetPos.y)
				{
					NowPos.y = m_TargetPos.y;
					m_Movement = MOVEMENT_STOP; // 到達したら停止状態へ
				}
				SetPos(NowPos);
			}
		}
	}
	break;

	case MOVEMENT_STOP: // 目標位置で待機
	{
		NowPos = m_TargetPos;
		SetPos(NowPos);

		// インターバル（タイマー）の減算処理
		if (m_nLifeInterval > 0)
		{
			m_nLifeInterval--;
			if (m_nLifeInterval <= 0)
			{
				m_Movement = MOVEMENT_END; // 0になったら退場へ
			}
		}
	}
	break;

	case MOVEMENT_END: // 画面外へ退場
	{
		// 画面中心より上なら上へ、下なら下へ退場
		if (NowPos.y < fCenterY)
		{
			NowPos.y -= 2.0f;
		}
		else
		{
			NowPos.y += 2.0f;
		}

		SetPos(NowPos);

		// 画面外に出たら破棄処理
		if (NowPos.y < -50.0f || NowPos.y > SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
			return;
		}
	}
	break;

	default:
		break;
	}

	// カメラアニメーション終了判定による強制退場トリガー
	// （待機中や移動中でもカメラアニメが終了したら MOVEMENT_END へ移行）
	if (CManager::GetInstance()->GetCamera()->GetIsAnimTime() == false && 
		m_Movement > MOVEMENT_NONE)
	{
		m_Movement = MOVEMENT_END;
	}

	// 親クラス更新処理
	CObject2D::Update();
}
//========================================================
// 描画処理
//========================================================
void CCameraUI::Draw(void)
{
	// 親クラスの描画処理
	CObject2D::Draw();
}