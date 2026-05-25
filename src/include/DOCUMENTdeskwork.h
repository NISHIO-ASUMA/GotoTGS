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

	// 設定処理
	void SetAlphaUI(void);

	// 情報取得処理
	CDeskworkUI* GetDeskUI(const int nIdx) const { return m_pDeskUI[nIdx]; }
	inline int GetNowIdx(void) const { return m_nNowIdx; }
	inline int GetNumUI(void) const { return Config::UI_NUM; }

	// 生成処理
	static CDOCUMENTDeskwork* Create(const D3DXVECTOR3& pos);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 300.0f;	// 横幅の増加分
		static constexpr float VALUE_TEXU = 0.333f;		// テクスチャのUV座標
		static constexpr int UI_NUM = 3;				// UIの数
		static constexpr float UI_WIDTH = 120.0f;		// UIの横幅
		static constexpr float UI_HEIGHT = 150.0f;		// UIの縦幅
		static constexpr int TIME_COOL = 60;			// クールタイム
	};

	// メンバ変数
	CDeskworkUI* m_pDeskUI[Config::UI_NUM];		// UIのポインタ
	int m_nNowIdx;								// 現在選択している番号
	int m_nCountTime;							// タイマーカウント
	bool m_bTime;								// クールタイムが始まっているかどうか
};