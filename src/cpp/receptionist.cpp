//========================================================
//
// 受付役の人の処理 [ receptionist.cpp ]
// Author: Asuma Nishio
// 
// NOTE : ここのクラスを修正・追加して作る
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "receptionist.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "spherecollider.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace RECEPTION_INFO
{
	constexpr const char* MOTION_NAME = "data/MOTION/Reception/ReceptionMotion.txt";
	constexpr float S_SIZE = 40.0f;
	const D3DXVECTOR3 B_SIZE = { 50.0f,50.0f,50.0f };
};

//========================================================
// コンストラクタ
//========================================================
CReceptionist::CReceptionist(int nPriority) : CNoMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_nGetTaskPaperNum(NULL)
{
}
//========================================================
// デストラクタ
//========================================================
CReceptionist::~CReceptionist()
{

}
//========================================================
// 生成処理
//========================================================
CReceptionist* CReceptionist::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CReceptionist* pReception = new CReceptionist;
	if (pReception == nullptr) return nullptr;

	// オブジェクト設定
	pReception->SetPos(pos);
	pReception->SetRot(rot);
	pReception->SetUseOutLine(true);
	pReception->SetOutLineColor(D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f));

	// 初期化失敗時
	if (FAILED(pReception->Init())) return nullptr;

	return pReception;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CReceptionist::Init(void)
{
	// この時だけ名前空間を使用する
	using namespace RECEPTION_INFO;

	// 親クラスの初期化処理
	CNoMoveCharactor::Init();

	// モーションロード
	MotionLoad(MOTION_NAME, MOTION::MAX, true);

	// 回転軸から角度を生成
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot();
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成
	m_pBoxColiider = CBoxCollider::Create(GetPos(), GetPos(),B_SIZE, matRot);

	// 球形コライダー生成
	m_pSphereColiider = CSphereCollider::Create(GetPos(), S_SIZE);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CReceptionist::Uninit(void)
{
	// ポインタの破棄
	m_pBoxColiider.reset();
	m_pSphereColiider.reset();

	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CReceptionist::Update(void)
{
	// モーション変更したら一定時間カウント後、元に戻す


	// コライダーの位置更新
	if (m_pSphereColiider)
		m_pSphereColiider->SetPos(GetPos());

	if (m_pBoxColiider)
	{
		m_pBoxColiider->SetPos(GetPos());
		m_pBoxColiider->SetPosOld(GetPos());
	}

	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CReceptionist::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}