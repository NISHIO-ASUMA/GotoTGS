//=========================================================
//
// 回転する2Dオブジェクト処理 [ object2Drotation.cpp ]
// Author: Misaki Takahashi
//
//==========================================================

//**********************************************************
// クラス定義ヘッダーファイル
//**********************************************************
#include "object2Drotation.h"

//**********************************************************
// システムインクルードファイル
//**********************************************************
#include <string>

//**********************************************************
// インクルードファイル
//**********************************************************
#include "manager.h"
#include "renderer.h"
#include "template.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CObject2DRotation::CObject2DRotation(int nPriority) : CObject(nPriority),
m_pTexture(nullptr),
m_pVtxBuff(nullptr),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_fAngle(NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_nIdxTexture(-1)
{

}

//=========================================================
// デストラクタ
//=========================================================
CObject2DRotation::~CObject2DRotation()
{

}

//=========================================================
// 生成処理
//=========================================================
CObject2DRotation* CObject2DRotation::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 pivot,
	const float fAngle,
	const D3DXCOLOR col,
	const D3DXVECTOR2 size,
	const char* pRegisterName)
{
	// ローカル生成
	CObject2DRotation* pObject2DRotation = new CObject2DRotation;
	if (pObject2DRotation == nullptr) return nullptr;

	// 各設定処理
	pObject2DRotation->SetPos(pos);					// 位置
	pObject2DRotation->SetPivot(pivot);				// 回転の基準点
	pObject2DRotation->SetAngle(fAngle);			// 角度
	pObject2DRotation->SetCol(col);					// 色
	pObject2DRotation->SetSize(size);				// サイズ
	pObject2DRotation->SetTexture(pRegisterName);	// テクスチャの名前

	// 初期化処理
	if (FAILED(pObject2DRotation->Init()))
		return nullptr;

	return pObject2DRotation;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CObject2DRotation::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos =
	pVtx[1].pos =
	pVtx[2].pos =
	pVtx[3].pos = VECTOR3_NULL;

	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = COLOR_WHITE;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CObject2DRotation::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}

//=========================================================
// 更新処理
//=========================================================
void CObject2DRotation::Update(void)
{
	// サイン・コサインの計算
	float sinA = sinf(m_fAngle);
	float cosA = cosf(m_fAngle);

	// 横幅と縦幅を基準点を元に計算する
	float fPivotW = m_fWidth * m_pivot.x;
	float fPivotH = m_fHeight * m_pivot.y;

	// 各頂点座標
	float PosX[4] = { -fPivotW, m_fWidth - fPivotW, -fPivotW		   , m_fWidth - fPivotW };
	float PosY[4] = { -fPivotH, -fPivotH		  , m_fHeight - fPivotH, m_fHeight - fPivotH };

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < 4; nCount++)
	{
		// XとYの回転を計算する
		float RotationX = PosX[nCount] * cosA - PosY[nCount] * sinA;
		float RotationY = PosX[nCount] * sinA + PosY[nCount] * cosA;

		// 頂点座標の設定
		pVtx[nCount].pos = D3DXVECTOR3(m_pos.x + RotationX, m_pos.y + RotationY, 0.0f);

	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 描画処理
//=========================================================
void CObject2DRotation::Draw(void)
{
	// ポーズオブジェクトなら
	if (CObject::GetObjType() == TYPE_PAUSE)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// テクスチャを戻す
		pDevice->SetTexture(0, nullptr);
	}
	else
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// nullなら
		if (m_nIdxTexture == -1)
		{
			// テクスチャを戻す
			pDevice->SetTexture(0, nullptr);
		}
		else
		{
			// テクスチャ取得
			CTexture* pTexture = CManager::GetInstance()->GetTexture();
			if (pTexture == nullptr) return;

			// テクスチャセット
			pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));
		}

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// テクスチャを戻す
		pDevice->SetTexture(0, nullptr);
	}
}

//===========================================================
// テクスチャ割り当て
//===========================================================
void CObject2DRotation::SetTexture(const char* pRegisterName)
{
	// テクスチャクラス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// 無かったら
	if (pRegisterName == nullptr) return;

	// パスを短縮する
	std::string TexName = "data/TEXTURE/";
	TexName += pRegisterName;

	// 割り当てる
	m_nIdxTexture = pTexture->Register(TexName.c_str());
}