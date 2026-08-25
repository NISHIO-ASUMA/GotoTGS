//=========================================================
//
// 疑いゲージマルチオブジェクトクラス [ enemydoubtgauge.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemydoubtgauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "template.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyDoubtGauge::CEnemyDoubtGauge(int nPriority) : CObject2DMulti(nPriority),
m_fRatio(NULL),
m_isDraw(false),
m_isComplete(false),
m_isUpGauge(false),
m_isNormalSet(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemyDoubtGauge::~CEnemyDoubtGauge()
{

}
//=========================================================
// 生成処理
//=========================================================
CEnemyDoubtGauge* CEnemyDoubtGauge::Create
(
	const D3DXVECTOR3& pos,
	const float fWidth,
	const float fHeight,
	const char* pBaseTex,
	const char* pMultitexture
)
{
	// インスタンス生成
	CEnemyDoubtGauge* pGauge = new CEnemyDoubtGauge;
	if (pGauge == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pGauge->Init())) return nullptr;

	// オブジェクト設定
	pGauge->SetPos(pos);
	pGauge->SetSize(fWidth, fHeight);
	pGauge->SetTexture(pBaseTex, 0);
	pGauge->SetTexture(pMultitexture, 1);

	return pGauge;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemyDoubtGauge::Init(void)
{
	// 親クラスの初期化処理
	CObject2DMulti::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemyDoubtGauge::Uninit(void)
{
	// 親クラスの終了処理
	CObject2DMulti::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemyDoubtGauge::Update(void)
{
	// フラグがoffなら
	if (!m_isDraw) return;

	// 比率を増やす
	if (m_isUpGauge)
		m_fRatio += 0.0040f;

	// 最大比率を超えたら
	if (m_fRatio >= 1.0f)
	{
		// 比率を最大にする
		m_fRatio = 1.0f;

		// フラグを設定
		m_isComplete = true;
		return;
	}
	else
	{
		m_isComplete = false;
	}

	// テクスチャのUVを比率分動かす
	CObject2DMulti::SetUV(m_fRatio);

	//-----------------------------------------------------
	// 3Dワールド座標 -> 2Dスクリーン座標変換
	//-----------------------------------------------------
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (pDevice)
	{
		D3DXMATRIX matView, matProj;
		D3DVIEWPORT9 viewport;

		pDevice->GetTransform(D3DTS_VIEW, &matView);
		pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		pDevice->GetViewport(&viewport);

		// m_TargetPos（敵の3D位置）をスクリーン座標へ変換
		D3DXVECTOR3 screenPos;
		D3DXVec3Project(&screenPos, &m_TargetPos, &viewport, &matProj, &matView, nullptr);

		float fCenterX = viewport.Width * 0.5f;
		float fCenterY = viewport.Height * 0.5f;

		D3DXVECTOR2 dir(screenPos.x - fCenterX, screenPos.y - fCenterY);

		if (screenPos.z > 1.0f || screenPos.z < 0.0f)
		{
			dir *= -1.0f;
		}

		float fRange = 60.0f;
		float fMinX = fRange;
		float fMaxX = viewport.Width - fRange;
		float fMinY = fRange;
		float fMaxY = viewport.Height - fRange;

		bool bIsOffScreen = (screenPos.z > 1.0f || screenPos.z < 0.0f ||
			screenPos.x < fMinX || screenPos.x > fMaxX ||
			screenPos.y < fMinY || screenPos.y > fMaxY);

		D3DXVECTOR3 finalGaugePos;

		if (bIsOffScreen)
		{
			float fScaleX = (dir.x != 0.0f) ? (fCenterX - fRange) / fabsf(dir.x) : 1e5f;
			float fScaleY = (dir.y != 0.0f) ? (fCenterY - fRange) / fabsf(dir.y) : 1e5f;

			float fScale = (std::min)(fScaleX, fScaleY);

			finalGaugePos.x = fCenterX + dir.x * fScale;
			finalGaugePos.y = fCenterY + dir.y * fScale;
			finalGaugePos.z = 0.0f;
		}
		else
		{
			finalGaugePos = D3DXVECTOR3(screenPos.x, screenPos.y, 0.0f);
		}

		// 2Dスクリーン座標を自身に設定
		SetPos(finalGaugePos);
	}

	// 更新
	CObject2DMulti::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CEnemyDoubtGauge::Draw(void)
{
	// 描画していないなら
	if (!m_isDraw) return;

	// 親クラスの描画
	CObject2DMulti::Draw();
}
//=========================================================
// 割合値の変更
//=========================================================
void CEnemyDoubtGauge::SetRatio(const float fRatio)
{
	// フラグ変更
	m_isComplete = false;

	// 少しずつ減らす
	m_fRatio -= fRatio;

	// フラグ変更
	if (m_fRatio <= 0.0f)
	{
		m_fRatio = 0.0f;
		m_isDraw = false;
		m_isNormalSet = true;
	}
}