//========================================================
//
// 敵のアイコン処理 [ searchicon.h ]
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
// 画面内に出る敵のアイコンクラスを定義
//*********************************************************
class CSearchIcon : public CObject2D
{
public:

	CSearchIcon(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CSearchIcon();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetTargetPos(const D3DXVECTOR3& TargetPos) { m_TargetPos = TargetPos; }
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">ファイル名</param>
	/// <param name="TargetPos">対象の座標</param>
	/// <returns></returns>
	static CSearchIcon* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth,
		const float& fHeight,
		const char* Filename,
		const D3DXVECTOR3& TargetPos
	);

private:
	D3DXVECTOR3 m_TargetPos;		// 対象の座標
	float m_fRange;					// 一定の距離
	bool m_isDraw;					// 描画フラグ
};