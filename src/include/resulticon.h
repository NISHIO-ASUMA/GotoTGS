//========================================================
//
// リザルト成果アイコン処理 [ resulticon.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
//リザルト成果アイコンクラスを定義
//*********************************************************
class CResultIcon : public CObject2D
{
public:

	CResultIcon(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CResultIcon();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 使うテクスチャを決定する関数
	/// </summary>
	/// <param name="nScore"></param>
	void MathScoreTexture(const int& nScore);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="nScore">スコアの値</param>
	/// <returns></returns>
	static CResultIcon* Create
	(
		const D3DXVECTOR3& pos, 
		const float fWidth, 
		const float fHeight, 
		const int nScore
	);

private:
	D3DXVECTOR3 m_targetPos;	// 本来の配置場所
	D3DXVECTOR3 m_targetSize;	// 本来のサイズ

	int   m_nFrameCount;		// アニメーションの経過フレーム
	int   m_nMaxFrame;			// アニメーション完了までの総フレーム数
	bool  m_isAnimating;		// アニメーション中かどうかのフラグ
	float m_fStartScale;		// 開始時の拡大率
};