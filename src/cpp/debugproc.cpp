//=========================================================
//
// デバッグプロセス処理 [ debubgproc.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "debugproc.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CDebugproc::CDebugproc() : m_isUse(true),
m_aStr{},
m_pFont(nullptr),
m_nNowIdx(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CDebugproc::~CDebugproc()
{

}
//=========================================================
// デバッグプロセスの初期化処理
//=========================================================
HRESULT CDebugproc::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// デバッグ表示用のフォントを設定
	D3DXCreateFont
	(
		pDevice, 
		FONTSIZE, 
		0,
		0,
		0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont
	);

	// 文字列クリア
	memset(m_aStr, 0, sizeof(m_aStr));

	// 使用状態時
	m_isUse = true;

	return S_OK;
}
//=========================================================
// デバッグプロセスの終了処理
//=========================================================
void CDebugproc::Uninit(void)
{
	// nullチェック
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}
//=========================================================
// デバッグプロセスの更新処理
//=========================================================
void CDebugproc::Update(void)
{
	// キー入力で表示切替
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F1))
	{
		m_isUse = m_isUse ? false : true;
	}
}
//=========================================================
// デバッグプロセスの描画処理
//=========================================================
void CDebugproc::Draw(int PosX,int PosY, const D3DXCOLOR& color)
{
#ifdef _DEBUG
	if (m_isUse)
	{
		// ウィンドウレクト
		RECT rect = { PosX,PosY,SCREEN_WIDTH,SCREEN_HEIGHT };

		// デバッグフォントの生成
		m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT,color);

		// バッファのクリア
		memset(m_aStr, 0, sizeof(m_aStr));

		// 描画インデックスのクリア
		End();
	}
#endif // _DEBUG
}
//=========================================================
// フォント描画関数
//=========================================================
void CDebugproc::Print(const char* fmt, ...)
{
#ifdef NDEBUG
	fmt = nullptr;
#endif // NDEBUG

#ifdef _DEBUG

	// 変数宣言
	va_list list;

	// 描画開始
	va_start(list, fmt);

	// 今の文字の位置が最大数より大きかったら
	if (m_nNowIdx > MAX_WORD)
	{
		// 今の文字の位置をゼロに戻す
		m_nNowIdx = 0;
		return;
	}

	// バッファに積み上げる
	m_nNowIdx += vsprintf(&m_aStr[m_nNowIdx], fmt, list);

	// 描画終了
	va_end(list);
#endif // _DEBUG
}