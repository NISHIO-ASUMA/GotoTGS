//=========================================================
//
// スコアポップアップ処理 [ scorepop.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "number3d.h"
#include "object.h"

//*********************************************************
// スコアのポップアップ表示クラスを定義
//*********************************************************
class CScorePop : public CObject
{
public:

	CScorePop(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CScorePop();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

public:

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }
	inline void SetPop(const int& nDamage) { m_nDamage = nDamage; }

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="pos">生成場所</param>
	/// <param name="nDamage">ダメージ数</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CScorePop* Create
	(
		const D3DXVECTOR3& pos,
		int nDamage,
		const float& fWidth = 100.0f,
		const float& fHeight = 100.0f
	);

private:

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float DIGIT_VALUE = 2.0f;		// 横並びの計算用割合
		static constexpr float UP_VALUE = 3.0f;			// 上昇速度
		static constexpr int INVISIVLE_COUNT = 120;		// 透明化するカウント
		static constexpr int NUM_DIGIT = 10;			// 分割数
		static constexpr int MAX_LIFE_FRAME = 60;		// 寿命フレーム
		static constexpr const char* TEX_NAME = "number_k.png";	// テクスチャ名
	};

private:
	std::vector<CNumber3D*> m_pDamagePopNumber; // 数字の配列
	D3DXCOLOR m_col;		// カラー
	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
	int m_nDamage;			// ダメージ値
	int m_nLifeCount;		// 寿命カウント
};