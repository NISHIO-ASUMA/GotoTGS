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
#include "manager.h"
#include "jsonmanager.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace POLYGONMANAGER
{
	constexpr const char* LOADNAME = "data/JSON/ArrowData.json";
};

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
	// 読み込み処理
	Load();

	//// 配列に要素の追加を行う ( 0グループ : near[一番近い] )
	//AddPolygon(0, { 817.0f,5.0f,-50.0f }, { 0.0f,D3DX_PI,0.0f }, { 15.0f,45.0f });
	//AddPolygon(0, { 817.0f,5.0f,-250.0f }, { 0.0f,D3DX_PI,0.0f }, { 15.0f,45.0f });
	//AddPolygon(0, { 880.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(0, { 1040.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,50.0f });
	//AddPolygon(0, { 1180.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,50.0f });
	//AddPolygon(0, { 1285.0f,5.0f,-420.0f }, { 0.0f,D3DX_PI,0.0f }, { 20.0f,50.0f });
	//AddPolygon(0, { 1285.0f,5.0f,-620.0f }, { 0.0f,D3DX_PI,0.0f }, { 20.0f,50.0f });

	//// 配列に要素の追加を行う ( 1グループ : right[2番目に遠い] )
	//AddPolygon(1, { 817.0f,5.0f,-50.0f }, { 0.0f,D3DX_PI,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 817.0f,5.0f,-250.0f }, { 0.0f,D3DX_PI,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 880.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1080.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1280.0f,5.0f,-350.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1400.0f,5.0f,-450.0f }, { 0.0f,2.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1480.0f,5.0f,-560.0f }, { 0.0f,2.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1560.0f,5.0f,-670.0f }, { 0.0f,2.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(1, { 1750.0f,5.0f,-800.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,50.0f });
	//AddPolygon(1, { 1900.0f,5.0f,-800.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,50.0f });

	//// 配列に要素の追加を行う ( 2グループ : Left[1番遠い] )
	//AddPolygon(2, { 817.0f,5.0f,283.0f }, { 0.0f,0.0f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 817.0f,5.0f,483.0f }, { 0.0f,0.0f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 817.0f,5.0f,683.0f }, { 0.0f,0.0f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 817.0f,5.0f,883.0f }, { 0.0f,0.0f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 850.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 1050.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 1250.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 1450.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 1650.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 1850.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });
	//AddPolygon(2, { 2050.0f,5.0f,1100.0f }, { 0.0f,D3DX_PI * 0.5f,0.0f }, { 15.0f,45.0f });

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
//=========================================================
// 外部ファイル読み込み処理
//=========================================================
void CPolygonManager::Load(void)
{
	// 管理クラスの取得
	const auto& jsonmanager = CManager::GetInstance()->GetJsonManager();

	// ファイルオープン
	jsonmanager->Load(POLYGONMANAGER::LOADNAME);
}
