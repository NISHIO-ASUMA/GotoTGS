//=========================================================
//
// ポイント(場所)を示すオブジェクト処理 [ pointobject.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "objectX.h"

//*********************************************************
// ポイントオブジェクトクラスを定義
//*********************************************************
class CPointObject : public CObjectX
{
public:

	CPointObject(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CPointObject();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetIsDraw(const bool isFlags) { m_isDraw = isFlags; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CPointObject* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:

	//********************************
	// 定数構造体
	//********************************
	struct Config
	{
		static constexpr float Rotation = 0.03f;	// 加算量
	};

private:
	bool m_isDraw;			// 描画フラグ
};