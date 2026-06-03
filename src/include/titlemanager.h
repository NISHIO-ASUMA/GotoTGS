//=========================================================
//
// タイトル管理処理 [ titlemanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// タイトル管理クラスを定義
//*********************************************************
class CTitleManager
{
public:

	~CTitleManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	static CTitleManager* GetInstance(void);

public:

	inline bool GetIsKeyInput(void) const { return isKeyinputSet; }

private:

	CTitleManager();	// 格納コンストラクタ

private:
	bool isKeyinputSet; // キー入力の判定をもらったか
};