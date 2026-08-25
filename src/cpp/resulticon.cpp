//========================================================
//
// リザルト成果アイコン処理 [ resulticon.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resulticon.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "easing.h"

//========================================================
// コンストラクタ
//========================================================
CResultIcon::CResultIcon(int nPriority) : CObject2D(nPriority),
m_isAnimating(false),
m_fStartScale(NULL),
m_nFrameCount(NULL),
m_nMaxFrame(NULL),
m_targetPos(VECTOR3_NULL),
m_targetSize(VECTOR3_NULL)
{

}
//========================================================
// デストラクタ
//========================================================
CResultIcon::~CResultIcon()
{

}
//========================================================
// 生成処理
//========================================================
CResultIcon* CResultIcon::Create
(
	const D3DXVECTOR3& pos,
	const float fWidth,
	const float fHeight,
	const int nScore
)
{
	// インスタンス生成
	CResultIcon* pIcon = new CResultIcon;
	if (pIcon == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pIcon->Init())) return nullptr;

	// 目的地の座標と本来のサイズを保持
	pIcon->m_targetPos = pos;
	pIcon->m_targetSize = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// オブジェクト設定
	pIcon->SetPos(pos);
	pIcon->SetSize(fWidth, fHeight);
	pIcon->MathScoreTexture(nScore);

	return pIcon;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CResultIcon::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	// 変数の初期値を設定
	m_isAnimating = true;
	m_nFrameCount = 0;
	m_fStartScale = 6.0f;
	m_nMaxFrame = 40;

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CResultIcon::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CResultIcon::Update(void)
{
	if (m_isAnimating)
	{
		// カウント加算
		m_nFrameCount++;

		// 差分計算
		float t = static_cast<float>(m_nFrameCount) / static_cast<float>(m_nMaxFrame);
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_isAnimating = false; 
		}

		// イージングの適用
		float fProgress = CEasing::EaseOutBack(t);

		// スケール計算
		float fCurrentScale = m_fStartScale + (1.0f - m_fStartScale) * fProgress;

		// サイズの設定
		SetSize(m_targetSize.x * fCurrentScale, m_targetSize.y * fCurrentScale);

		// 座標の更新
		SetPos(m_targetPos);
	}

	// 親クラスの更新処理
	CObject2D::Update();
}
//========================================================
// 描画処理
//========================================================
void CResultIcon::Draw(void)
{
	// 親クラスの描画処理
	CObject2D::Draw();
}
//========================================================
// 使うテクスチャを設定する関数
//========================================================
void CResultIcon::MathScoreTexture(const int& nScore)
{// ここは主に称号のテクスチャを設定する

	if (nScore <= 3000)
		SetTexture("ResultIcon/yesman.png");
	else if (nScore <= 8000)
		SetTexture("ResultIcon/workman.png");
	else if (nScore <= 20000)
		SetTexture("ResultIcon/Saboriman.png");
	else if (nScore <= 50000)
		SetTexture("ResultIcon/Dalarryman.png");
	else
		SetTexture("ResultIcon/bucklehman.png");
}