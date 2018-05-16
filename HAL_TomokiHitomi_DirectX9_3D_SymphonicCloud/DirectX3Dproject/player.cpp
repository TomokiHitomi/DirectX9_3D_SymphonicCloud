//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "player.h"
#include "model.h"
#include "Light.h"

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
		player->m_CSkinMesh.Init(pDevice, PLAYER_PLAYER);
	}

	player->m_CSkinMesh.ChangeAnim((DWORD)ANIME01);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	//メッシュオブジェクト管理クラス初期化管理
	player->m_CSkinMesh.Release();
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

	player->m_CSkinMesh.Update(g_mtxWorldPlayer);


#ifdef _DEBUG
	PrintDebugProc("Pos[X:%f Y:%f Z:%f]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	PrintDebugProc("Anime[%d]\n",player->m_CSkinMesh.GetAnimTrack());
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	//SetLight(LIGHT2, TRUE);
	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	player->m_CSkinMesh.Draw(pDevice, player->prs);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//SetLight(LIGHT2, FALSE);
}

//=============================================================================
// アニメーション設定関数
//=============================================================================
void SetPlayerAnime(int nPlayer,DWORD dAnime)
{
	PLAYER *player = GetPlayer(nPlayer);
	if (dAnime != player->m_CSkinMesh.GetAnimTrack())
	{
		player->m_CSkinMesh.ChangeAnim(dAnime);
	};
}

//=============================================================================
// 取得関数
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}