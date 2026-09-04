//=========================================================
//
// デバッグプロセス処理 [ debubgproc.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// デバッグプロセスクラスを定義
//*********************************************************
class CDebugproc
{
public:

	~CDebugproc();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(int PosX, int PosY,const D3DXCOLOR& color = COLOR_WHITE);
	void Print(const char* fmt, ...);
	void End(void) { m_nNowIdx = 0; }

	/// <summary>
	/// シングルトン取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CDebugproc* GetInstance(void)
	{
		static CDebugproc Instance;
		return &Instance;
	}

private:
	CDebugproc();	// シングルトン

private:
	static constexpr int FONTSIZE = 20;		// フォントサイズ

private:
	LPD3DXFONT m_pFont;				// デバッグフォント
	int m_nNowIdx;					// 現在のインデックス
	char m_aStr[MAX_WORD];			// 文字列格納バッファ
	bool m_isUse;					// 使用しているかフラグ
};