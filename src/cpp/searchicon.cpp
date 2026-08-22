//========================================================
//
// 敵のアイコン処理 [ searchicon.cpp ]
// Author: Asuma Nishio
// 参考例 : これはよくある敵がこっちにいますよーっていうアイコンを出す2duiクラス 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "searchicon.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "renderer.h"

//========================================================
// コンストラクタ
//========================================================
CSearchIcon::CSearchIcon(int nPriority) : CObject2D(nPriority),
m_TargetPos(VECTOR3_NULL),
m_fRange(NULL),
m_isDraw(false)
{

}
//========================================================
// デストラクタ
//========================================================
CSearchIcon::~CSearchIcon()
{

}
//========================================================
// 生成処理
//========================================================
CSearchIcon* CSearchIcon::Create
(
	const D3DXVECTOR3& pos,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	const D3DXVECTOR3& TargetPos
)
{
	// インスタンス生成
	CSearchIcon* pIcon = new CSearchIcon;
	if (pIcon == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pIcon->Init())) return nullptr;
	
	// オブジェクト設定
	pIcon->SetPos(pos);
	pIcon->SetSize(fWidth, fHeight);
	pIcon->SetTexture(Filename);
	pIcon->SetTargetPos(TargetPos);

	return pIcon;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CSearchIcon::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	// 境界線の距離を設定
	m_fRange = 60.0f;

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CSearchIcon::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CSearchIcon::Update(void)
{
	if (!m_isDraw) return;

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

	// カメラの後方にいる場合、負の設定
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
	CObject2D::Update();
}
//========================================================
// 描画処理
//========================================================
void CSearchIcon::Draw(void)
{
	if (!m_isDraw) return;

	// 親クラスの描画処理
	CObject2D::Draw();
}