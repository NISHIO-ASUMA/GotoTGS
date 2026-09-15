//========================================================
//
// 3dポリゴンを管理するクラス [ polygonmanager.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルード
//*********************************************************
#include <vector>
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CObjectRotation3D;

//*********************************************************
// 3dポリゴンを複数管理するクラスを定義
//*********************************************************
class CPolygonManager
{
public:

	//**********************
	// 最大種類数の列挙型
	//**********************
	enum TYPE
	{
		TYPE_LEFT,
		TYPE_RIGHT,
		TYPE_NEAR,
		TYPE_MAX
	};

	~CPolygonManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// ポリゴンの追加
	/// </summary>
	/// <param name="nIdxGroup">グループ番号</param>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	/// <param name="size">大きさ</param>
	void AddPolygon(int nIdxGroup,const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const D3DXVECTOR2& size);

	/// <summary>
	/// 描画変更命令
	/// </summary>
	/// <param name="nIdxGroup">対象のグループ</param>
	/// <param name="isFalgs">描画フラグ</param>
	void ChangeDrawFalg(int nIdxGroup, bool isFalgs);

	/// <summary>
	/// 取得関数
	/// </summary>
	/// <param name="type">グループの種類番号</param>
	/// <returns></returns>
	const std::vector<CObjectRotation3D*>& GetGroup(TYPE type) const;

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CPolygonManager* GetInstance(void)
	{
		static CPolygonManager Instance;
		return &Instance;
	}

private:
	CPolygonManager();

private:
	using PolygonList = std::vector<CObjectRotation3D*>;	// ポリゴン配列
	std::array<PolygonList, TYPE_MAX> m_apArrayListGroup;	// グループ配列
};