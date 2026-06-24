//=========================================================
//
// マルチテクスチャ適用2Dオブジェクトクラス [ object2Dmulti.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//**********************************************************
// クラス定義ヘッダーファイル
//**********************************************************
#include "object2Dmulti.h"

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

//==========================================================
// コンストラクタ
//==========================================================
CObject2DMulti::CObject2DMulti(int nPriority) : CObject(nPriority),
m_pVtxBuff(nullptr),
m_rot(VECTOR3_NULL),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_fHeight(NULL),
m_fWidth(NULL),
m_nColorCount(NULL),
m_nDrawType(NULL),
m_nIdxTexture(-1),
m_apTexture{},
m_nAnchorType(ANCHORTYPE_NONE)
{

}
//==========================================================
// デストラクタ
//==========================================================
CObject2DMulti::~CObject2DMulti()
{
	m_apTexture.clear();
}
//==========================================================
// 生成処理
//==========================================================
CObject2DMulti* CObject2DMulti::Create
(
	const D3DXVECTOR3& pos,
	float fWidth,
	float fHeight,
	const char* pTexName0,
	const char* pTexName1
)
{
	// インスタンス生成
	CObject2DMulti* pMulti = new CObject2DMulti;
	if (pMulti == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pMulti->Init())) return nullptr;

	// オブジェクト設定
	pMulti->SetPos(pos);
	pMulti->SetHeight(fHeight);
	pMulti->SetWidth(fWidth);
	pMulti->SetTexture(pTexName0, 0);
	pMulti->SetTexture(pTexName1, 1);

	return pMulti;
}
//==========================================================
// 初期化処理
//==========================================================
HRESULT CObject2DMulti::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_MULTI) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D_MULTI,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D_MULTI* pVtx;

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

	//--------------------------------------------
	// テクスチャ座標（UV）の設定 ( float型にしてます )
	//--------------------------------------------
	pVtx[0].tex_u = 0.0f; pVtx[0].tex_v = 0.0f;
	pVtx[1].tex_u = 1.0f; pVtx[1].tex_v = 0.0f;
	pVtx[2].tex_u = 0.0f; pVtx[2].tex_v = 1.0f;
	pVtx[3].tex_u = 1.0f; pVtx[3].tex_v = 1.0f;

	pVtx[0].texMulti_u = 0.0f; pVtx[0].texMulti_v = 0.0f;
	pVtx[1].texMulti_u = 1.0f; pVtx[1].texMulti_v = 0.0f;
	pVtx[2].texMulti_u = 0.0f; pVtx[2].texMulti_v = 1.0f;
	pVtx[3].texMulti_u = 1.0f; pVtx[3].texMulti_v = 1.0f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//==========================================================
// 終了処理
//==========================================================
void CObject2DMulti::Uninit(void)
{
	// テクスチャ配列の破棄
	m_apTexture.clear();
	
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//==========================================================
// 更新処理
//==========================================================
void CObject2DMulti::Update(void)
{// 一旦中心座標

	// 頂点情報のポインタ
	VERTEX_2D_MULTI* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex_u = 0.0f; pVtx[0].tex_v = 0.0f;
	pVtx[1].tex_u = 1.0f; pVtx[1].tex_v = 0.0f;
	pVtx[2].tex_u = 0.0f; pVtx[2].tex_v = 1.0f;
	pVtx[3].tex_u = 1.0f; pVtx[3].tex_v = 1.0f;

	pVtx[0].texMulti_u = 0.0f; pVtx[0].texMulti_v = 0.0f;
	pVtx[1].texMulti_u = 1.0f; pVtx[1].texMulti_v = 0.0f;
	pVtx[2].texMulti_u = 0.0f; pVtx[2].texMulti_v = 1.0f;
	pVtx[3].texMulti_u = 1.0f; pVtx[3].texMulti_v = 1.0f;

	// アンロック
	m_pVtxBuff->Unlock();
}
//==========================================================
// 描画処理
//==========================================================
void CObject2DMulti::Draw(void)
{
	// デバイスとテクスチャ取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	//配列の要素分描画する
	for (int nCnt = 0; nCnt < static_cast<int>(m_apTexture.size()); nCnt++)
	{
		// テクスチャごとのインデックスを取得
		int nIdx = m_apTexture[nCnt];
		if (nIdx < 0) continue;

		// アドレス取得
		LPDIRECT3DTEXTURE9 pTexData = pTexture->GetAddress(nIdx);
		if (pTexData == nullptr) continue;

		// デバイスにセット
		pDevice->SetTexture(static_cast<DWORD>(nCnt), pTexData);

		if (nCnt == 0)
		{
			// 1枚目の設定
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		}
		else
		{
			// 2枚目の設定
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG2, D3DTA_TEXTURE);

			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
		}
	}

	// 頂点バッファの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D_MULTI));

	// FVF設定
	pDevice->SetFVF(FVF_VERTEX_2D_MULTI);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャ破棄
	for (int i = 0; i < static_cast<int>(m_apTexture.size()); ++i)
	{
		// テクスチャクラスにnull設定
		pDevice->SetTexture(static_cast<DWORD>(i), nullptr);

		// テクスチャ設定を元に戻す
		if (i > 0)
		{
			pDevice->SetTextureStageState(static_cast<DWORD>(i), D3DTSS_COLOROP, D3DTOP_DISABLE);
			pDevice->SetTextureStageState(static_cast<DWORD>(i), D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		}
	}
}
//==========================================================
// 中心基準生成
//==========================================================
void CObject2DMulti::SetCenter(void)
{

}
//==========================================================
// 左寄り
//==========================================================
void CObject2DMulti::SetLeft(void)
{

}
//==========================================================
// 右寄り
//==========================================================
void CObject2DMulti::SetRight(void)
{

}
//==========================================================
// テクスチャ設定
//==========================================================
void CObject2DMulti::SetTexture(const char* pRegisterName, const int& nIdx)
{
	// 例外チェック
	if (nIdx < 0) return;
	if (pRegisterName == nullptr) return;

	// テクスチャクラス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パスを短縮する
	std::string TexName = "data/TEXTURE/";
	TexName += pRegisterName;

	// テクスチャ登録
	int nIdxTexture = pTexture->Register(TexName.c_str());
	if (nIdxTexture == -1) return;

	// 配列サイズを設定
	if (nIdx >= m_apTexture.size())
	{
		m_apTexture.resize(nIdx + 1, -1);
	}

	// 指定したステージにインデックスを格納
	m_apTexture[nIdx] = nIdxTexture;
}
//==========================================================
// 点滅関数
//==========================================================
void CObject2DMulti::SetFlash(const int nFirstcount, const int nEndcount, const D3DXCOLOR col)
{

}
