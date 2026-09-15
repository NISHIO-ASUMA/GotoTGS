//=========================================================
//
// 3dポリゴンを管理するクラス [ polygonmanager.cpp ]
// Author: Asuma Nishio
// TODO : Loadクラスを作成する(jsonでもなんでもいい)
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "polygonmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object3drotation.h"

//=========================================================
// コンストラクタ
//=========================================================
CPolygonManager::CPolygonManager() : m_apArrayListGroup{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CPolygonManager::~CPolygonManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPolygonManager::Init(void)
{
	// 配列に要素の追加を行う
	AddPolygon(0, { 0.0f,30.0f,0.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 30.0f,60.0f });
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPolygonManager::Uninit(void)
{
	// 配列のクリア
	for (auto& groupList : m_apArrayListGroup)
	{
		for (auto pPolygon : groupList)
		{
			if (pPolygon)
			{
				// null化
				pPolygon = nullptr;
			}
		}

		// リスト配列のクリア
		groupList.clear();
	}
}
//=========================================================
// 更新処理
//=========================================================
void CPolygonManager::Update(void)
{

}
//=========================================================
// ポリゴン追加処理
//=========================================================
void CPolygonManager::AddPolygon(int nIdxGroup, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR2& size)
{
	// 番号を変換する
	auto NumberGroup = static_cast<TYPE>(nIdxGroup);

	// 配列の要素に追加していく
	m_apArrayListGroup[NumberGroup].push_back(CObjectRotation3D::Create(pos, rot, size));
}
//=========================================================
// 一括でグループの描画命令を変更
//=========================================================
void CPolygonManager::ChangeDrawFalg(int nIdxGroup,bool isFalgs)
{
	// 範囲チェック
	if (nIdxGroup < 0 || nIdxGroup >= static_cast<int>(TYPE::TYPE_MAX))
	{
		return;
	}

	// 指定グループを参照で取得
	auto& targetGroup = m_apArrayListGroup[nIdxGroup];

	// 全ポリゴンポインタをループしてアクセス
	for (auto pPolygon : targetGroup)
	{
		if (pPolygon)
		{
			// フラグ変更関数
			pPolygon->SetIsDraw(isFalgs);
		}
	}
}
