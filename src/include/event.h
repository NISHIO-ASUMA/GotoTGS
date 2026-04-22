//=========================================================
//
// イベント処理 [ event.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// イベント一個に対するクラスを定義
//*********************************************************
class CEvent
{
public:

	CEvent();
	~CEvent();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline D3DXVECTOR3 GetPos(void) const { return  m_pos; }

	/// <summary>
	/// イベント生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <returns></returns>
	static CEvent* Create(const D3DXVECTOR3& pos);

private:

	D3DXVECTOR3 m_pos; // イベントを起こす座標
};