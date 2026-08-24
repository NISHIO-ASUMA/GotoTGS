//========================================================
//
// 敵のアイコン処理 [ enemymarkicon.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemymarkicon.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "renderer.h"

//========================================================
// コンストラクタ
//========================================================
CEnemyMarkIcon::CEnemyMarkIcon(int nPriority) : CObject2DMulti(nPriority),
m_TargetPos(VECTOR3_NULL),
m_fRange(NULL),
m_isDraw(false)
{

}
//========================================================
// デストラクタ
//========================================================
CEnemyMarkIcon::~CEnemyMarkIcon()
{

}
//========================================================
// 生成処理
//========================================================
CEnemyMarkIcon* CEnemyMarkIcon::Create
(
	const D3DXVECTOR3& pos,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	const char* FileMultiTex,
	const D3DXVECTOR3& TargetPos
)
{
	// インスタンス生成
	CEnemyMarkIcon* pIcon = new CEnemyMarkIcon;
	if (pIcon == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pIcon->Init())) return nullptr;

	// オブジェクト設定
	pIcon->SetPos(pos);
	pIcon->SetSize(fWidth, fHeight);
	pIcon->SetTexture(Filename,0);
	pIcon->SetTexture(FileMultiTex,1);
	pIcon->SetTargetPos(TargetPos);

	return pIcon;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CEnemyMarkIcon::Init(void)
{
	// 親クラスの初期化
	CObject2DMulti::Init();

	// 境界線の距離を設定
	m_fRange = 60.0f;

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CEnemyMarkIcon::Uninit(void)
{
	// 親クラスの終了処理
	CObject2DMulti::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CEnemyMarkIcon::Update(void)
{
	// フラグ設定
	if (!m_isDraw) return;

	// 値のアップ設定






	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return;

	// 行列計算変数
	D3DXMATRIX matView, matProj;
	D3DVIEWPORT9 viewport;

	// デバイスから取得
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	pDevice->GetViewport(&viewport);

	// ワールド座標をスクリーン座標へ変換
	D3DXVECTOR3 screenPos;
	D3DXVec3Project(&screenPos, &m_TargetPos, &viewport, &matProj, &matView, nullptr);

	// 画面の中心座標
	float fCenterX = viewport.Width * 0.5f;
	float fCenterY = viewport.Height * 0.5f;

	// 画面中心からターゲットへの方向ベクトル
	D3DXVECTOR2 dir(screenPos.x - fCenterX, screenPos.y - fCenterY);

	// カメラの後方に敵がいる場合、負の設定
	if (screenPos.z > 1.0f || screenPos.z < 0.0f)
	{
		dir *= -1.0f;
	}

	// 画面枠の境界線を計算
	float fMinX = m_fRange;
	float fMaxX = viewport.Width - m_fRange;
	float fMinY = m_fRange;
	float fMaxY = viewport.Height - m_fRange;

	// スクリーンの座標を設定
	D3DXVECTOR3 finalIconPos;
	bool bIsOffScreen = (screenPos.z > 1.0f || screenPos.z < 0.0f ||
		screenPos.x < fMinX || screenPos.x > fMaxX ||
		screenPos.y < fMinY || screenPos.y > fMaxY);

	if (bIsOffScreen)
	{
		// 画面端の長方形枠上にクランプ
		float fScaleX = (dir.x != 0.0f) ? (fCenterX - m_fRange) / fabsf(dir.x) : 1e5f;
		float fScaleY = (dir.y != 0.0f) ? (fCenterY - m_fRange) / fabsf(dir.y) : 1e5f;

		float fScale = (std::min)(fScaleX, fScaleY);

		finalIconPos.x = fCenterX + dir.x * fScale;
		finalIconPos.y = fCenterY + dir.y * fScale;
		finalIconPos.z = 0.0f;
	}
	else
	{
		// 画面内なら変換された座標にそのまま表示
		finalIconPos = D3DXVECTOR3(screenPos.x, screenPos.y, 0.0f);
	}

	// 計算した位置を設定
	SetPos(finalIconPos);

	// 親クラスの更新処理
	CObject2DMulti::Update();
}
//========================================================
// 描画処理
//========================================================
void CEnemyMarkIcon::Draw(void)
{
	if (!m_isDraw) return;

	// 親クラスの描画処理
	CObject2DMulti::Draw();
}