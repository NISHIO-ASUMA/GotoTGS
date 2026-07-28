//=========================================================
//
// ポイント(場所)を示すオブジェクト処理 [ pointobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "pointobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CPointObject::CPointObject(int nPriority) : CObjectX(nPriority),
m_isDraw(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CPointObject::~CPointObject()
{

}
//=========================================================
// 生成処理
//=========================================================
CPointObject* CPointObject::Create
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale,
	const char* pModelName
)
{
	// インスタンス生成
	CPointObject* pObject = new CPointObject;
	if (pObject == nullptr) return nullptr;

	// オブジェクト設定
	pObject->SetFilePass(pModelName);
	pObject->SetPos(pos);
	pObject->SetRot(rot);
	pObject->SetScale(scale);

	// 初期化失敗時
	if (FAILED(pObject->Init())) return nullptr;

	return pObject;
}
//=========================================================
// ブロックの初期化処理
//=========================================================
HRESULT CPointObject::Init(void)
{
	// 親クラスの初期化処理
	CObjectX::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPointObject::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CPointObject::Update(void)
{
	if (!m_isDraw) return;

	// 回転を取得
	auto rot = GetRot();

	// 回転する
	rot.y += Config::Rotation;

	// 正規化
	rot.y = NormalAngle(rot.y);

	// 親クラスにセット
	SetRot(rot);

	// 親クラスの更新
	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CPointObject::Draw(void)
{
	if (!m_isDraw) return;

	// 親クラスの描画処理
	CObjectX::Draw();
}