//=========================================================
//
// イベント処理 [ event.h ]
// Author: Misaki Takahashi
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

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CEvent* Create(const D3DXVECTOR3& pos);

private:

};