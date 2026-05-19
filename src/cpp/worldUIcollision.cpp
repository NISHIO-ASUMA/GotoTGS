//=========================================================
//
// マップに配置するUIの球形判定処理 [ worldUIcollision.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "worldUIcollision.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "collisionsphere.h"

//*********************************************************
// 名前空間を定義
//*********************************************************
namespace CollisionJson
{
	using json = nlohmann::json;
	constexpr const char* LoadName = "data/JSON/WorldSphere.json"; // 読み込むファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CWorldUICollision::CWorldUICollision() : m_nType(TYPE_MAX),
m_pInteractPoints{}
{

}

//=========================================================
// デストラクタ
//=========================================================
CWorldUICollision::~CWorldUICollision()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorldUICollision::Init(void)
{
	// 配列のクリア処理
	m_pInteractPoints.clear();

	// jsonファイル読み込み
	LoadJson();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CWorldUICollision::Uninit(void)
{
	// リストから削除
	m_pInteractPoints.clear();
}

//=========================================================
// ADD : 西尾
// 判定等の登録処理 
//=========================================================
void CWorldUICollision::AddInteractEvent(const D3DXVECTOR3& pos, const float& fRadius, const TYPE& type)
{
	// インスタンス生成
	auto CollisionPoint = std::make_unique<InteractData>();
	if (!CollisionPoint) return;

	// 構造体変数の設定
	CollisionPoint->pos = pos;
	CollisionPoint->fRadius = fRadius;
	CollisionPoint->nType = type;

	// 実際の当たり判定コライダー生成
	CollisionPoint->pCollider = CSphereCollider::Create(pos, fRadius);

	// 配列内のオブジェクトに登録する
	m_pInteractPoints.push_back(std::move(CollisionPoint));
}
//=========================================================
// 更新処理
//=========================================================
void CWorldUICollision::Update(void)
{
	// 当たり判定の更新
	for (auto& point : m_pInteractPoints)
	{
		if (point && point->pCollider)
		{
			// 更新された値をセットする
			point->pCollider->SetPos(point->pos);
		}
	}
}
//=========================================================
// jsonファイル読み込み処理
//=========================================================
void CWorldUICollision::LoadJson(void)
{
	// ファイルストリームを開く
	std::ifstream file(CollisionJson::LoadName);

	// ファイルが正常に開けなかった場合
	if (!file.is_open())
	{
		return;
	}

	// JSONデータを格納するオブジェクト
	CollisionJson::json sphereData;

	// ファイルからJSONをパース
	file >> sphereData;

	if (sphereData.is_array())
	{
		// 配列の要素を1つずつループ処理
		for (const auto& item : sphereData)
		{
			// 必要なキーチェック
			if (item.contains("Pos") && item.contains("Radius") && item.contains("Type"))
			{
				// 座標情報
				D3DXVECTOR3 pos;
				pos.x = item["Pos"][0].get<float>();
				pos.y = item["Pos"][1].get<float>();
				pos.z = item["Pos"][2].get<float>();

				// 半径情報
				float fRadius = item["Radius"].get<float>();

				// タスク情報
				TYPE type = static_cast<TYPE>(item["Type"].get<int>()); // int型で取得し、TYPE型にキャスト

				// 登録処理でリストに追加
				AddInteractEvent(pos, fRadius, type);
			}
		}
	}
	
	// ファイルを閉じる
	file.close();	
}