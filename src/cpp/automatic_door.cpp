//=========================================================
//
// 自動ドアの処理 [ automatic_door.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "automatic_door.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "collisionbox.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoor::CAutoMaticDoor(int nPriority) : CObjectX(nPriority),
m_isZTestEneble(false),
m_pCollider(nullptr)
{

}

CAutoMaticDoor::~CAutoMaticDoor()
{
}

HRESULT CAutoMaticDoor::Init(void)
{
	return E_NOTIMPL;
}

void CAutoMaticDoor::Uninit(void)
{
}

void CAutoMaticDoor::Update(void)
{
}

void CAutoMaticDoor::Draw(void)
{
}

bool CAutoMaticDoor::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	return false;
}

CAutoMaticDoor* CAutoMaticDoor::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const char* pModelName)
{
	return nullptr;
}
