//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "player.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "xfilemanager.h"
#include "boxcollider.h"
#include "collisionbox.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_pCollider(nullptr)
{

}

//=========================================================
// デストラクタ
//=========================================================
CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Init(void)
{
	return E_NOTIMPL;
}

void CPlayer::Uninit(void)
{
}

void CPlayer::Update(void)
{
}

void CPlayer::Draw(void)
{
}

CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	return nullptr;
}
