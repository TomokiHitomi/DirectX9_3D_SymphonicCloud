//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "player.h"
#include "model.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER				playerWk[PLAYER_MAX];
D3DXMATRIX			g_mtxWorldPlayer;
CSkinMesh			m_CSkinMeshPlayer;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int nType)
{
	PLAYER *player = &playerWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・回転・スケールの設定
	player->prs.pos = D3DXVECTOR3(PLAYER_POS_X, 0.0f, PLAYER_POS_Z);
	player->prs.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	player->prs.scl = D3DXVECTOR3(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);

	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	if (nType == 0)
	{
		m_CSkinMeshPlayer.Init(pDevice, PLAYER_PLAYER);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//メッシュオブジェクト管理クラス初期化管理
	m_CSkinMeshPlayer.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = GetPlayer(0);
#ifdef _DEBUG
	PrintDebugProc("【 PLAYER 】\n");
#endif
	MODEL *model = GetModel(0);


	player->prs.pos = model->posModel;
	player->prs.rot = model->rotModel;

	m_CSkinMeshPlayer.Update(g_mtxWorldPlayer);


#ifdef _DEBUG
	PrintDebugProc("PlayerPos[X:%f Y:%f Z:%f]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_CSkinMeshPlayer.Draw(pDevice, player->prs);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// 取得関数
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}