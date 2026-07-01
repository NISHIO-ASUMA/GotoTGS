//=========================================================
//
// 書類タスク処理 [ DOCUMENTdeskwork.h ]
// Author: Takahashi Misaki
//
// NOTE : 書類タスクに関しての追記 Author : 西尾
//		かごの置いてある位置 → { 40.0f,36.0f,280.0f }を参考に。
//		当たり判定のサイズ → 30.0fくらいかなと思います。
//		上記2つを[ WolrdSphrer.json ]に追加してください。
//		対象のモデルはあとで配置するので気にしないで。とりあえず判定取って起動できるように。
//		そして発生できる条件を[ コピーした書類があるとき ] のみに限定したいと思います。
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルード
//*********************************************************
#include "deskworkUImanager.h"

//*********************************************************
// 書類タスククラスを定義
//*********************************************************
class CDOCUMENTDeskwork :public CDeskworkUIManager
{
public:

	CDOCUMENTDeskwork();
	~CDOCUMENTDeskwork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetDOCUMENTValue(void);

	// 生成処理
	static CDOCUMENTDeskwork* Create(const bool& bUse = false);

private:
	// メンバ変数

};