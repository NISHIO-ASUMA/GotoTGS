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

	// uvの設定
	pVtx[0].tex_u = 0.0f; pVtx[0].tex_v = 0.0f;
	pVtx[1].tex_u = 1.0f; pVtx[1].tex_v = 0.0f;
	pVtx[2].tex_u = 0.0f; pVtx[2].tex_v = 1.0f;
	pVtx[3].tex_u = 1.0f; pVtx[3].tex_v = 1.0f;

	pVtx[0].texMulti_u = 0.0f; pVtx[0].texMulti_v = 0.0f;
	pVtx[1].texMulti_u = 1.0f; pVtx[1].texMulti_v = 0.0f;
	pVtx[2].texMulti_u = 0.0f; pVtx[2].texMulti_v = 0.5f;
	pVtx[3].texMulti_u = 1.0f; pVtx[3].texMulti_v = 0.5f;

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
	SetAnchorPoint(pVtx);

	// 頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = m_col;

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
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG1, D3DTA_TEXTURE); // 2枚目の赤
			pDevice->SetTextureStageState(static_cast<DWORD>(nCnt), D3DTSS_COLORARG2, D3DTA_CURRENT); // 1枚目の円

			// 透明にする
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

	// テクスチャ解除
	for (int i = 0; i < static_cast<int>(m_apTexture.size()); i++)
	{
		// null初期化
		pDevice->SetTexture(i, nullptr);
	}

	// 通常状態へ戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 2枚目のテクスチャステージチャートを戻す
	for (int i = 1; i < static_cast<int>(m_apTexture.size()); i++)
	{
		pDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}
}

//==========================================================
// アンカーポイントに合わせた座標設定
//==========================================================
void CObject2DMulti::SetAnchorPoint(VERTEX_2D_MULTI* pVtx)
{
	switch (m_nAnchorType)
	{
	// 中心
	case ANCHORTYPE_CENTER:

		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	// 左寄り
	case ANCHORTYPE_LEFTSIDE:

		pVtx[0].pos = D3DXVECTOR3(m_pos.x				, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth * 2.0f, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x				, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth * 2.0f, m_pos.y + m_fHeight, 0.0f);

		break;

	// 右寄り
	case ANCHORTYPE_RIGHTSIDE:

		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth * 2.0f, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x				, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth * 2.0f, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x				, m_pos.y + m_fHeight, 0.0f);

		break;

	// 上寄り
	case ANCHORTYPE_TOPSIDE:

		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y				 , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y				 , 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight * 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight * 2.0f, 0.0f);

		break;

	// 下寄り
	case ANCHORTYPE_BOTTOMSIDE:

		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight * 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight * 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y				 , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y				 , 0.0f);

		break;
	}

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
// UV設定
//==========================================================
void CObject2DMulti::SetUV(const float fRatio)
{
	// クランプ処理
	Clump(fRatio, 0.0f, 1.0f);

	// 頂点情報のポインタ
	VERTEX_2D_MULTI* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定 ( ベース )
	pVtx[0].tex_u = 0.0f; pVtx[0].tex_v = 0.0f;
	pVtx[1].tex_u = 1.0f; pVtx[1].tex_v = 0.0f;
	pVtx[2].tex_u = 0.0f; pVtx[2].tex_v = 1.0f;
	pVtx[3].tex_u = 1.0f; pVtx[3].tex_v = 1.0f;

	// uv座標のオフセット
	float vOffset = fRatio * 0.5f;

	// 2枚目の設定 ( マルチ側 )
	// 上端の頂点
	pVtx[0].texMulti_u = 0.0f; pVtx[0].texMulti_v = 0.0f + vOffset;
	pVtx[1].texMulti_u = 1.0f; pVtx[1].texMulti_v = 0.0f + vOffset;

	// 下端の頂点
	pVtx[2].texMulti_u = 0.0f; pVtx[2].texMulti_v = 0.5f + vOffset;
	pVtx[3].texMulti_u = 1.0f; pVtx[3].texMulti_v = 0.5f + vOffset;

	// アンロック
	m_pVtxBuff->Unlock();
}