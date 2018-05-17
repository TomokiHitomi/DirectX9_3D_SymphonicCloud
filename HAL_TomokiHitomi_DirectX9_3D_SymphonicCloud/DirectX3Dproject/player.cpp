//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "player.h"
#include "model.h"
#include "Light.h"
#include "input.h"
#include "magic.h"
#include "cloud.h"

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
void CheckActionPlayer(void);

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

	player->nJumpCount = 0;
	player->nJumpFlag = 0;
	player->nRecoilCount = 0;
	player->bRecoilFlag = false;
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	if (nType == 0)
	{
		player->m_CSkinMesh.Init(pDevice, PLAYER_MODEL);
	}

	//player->m_CSkinMesh.ChangeAnim((DWORD)ANIME01);

	SetPlayerAnime(0, PLAYER_ANIME_HAND);
	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ANIME);

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

	switch (GetStage())
	{
	case STAGE_GAME:
		CheckActionPlayer();
		break;
	}

	player->prs.pos = model->posModel;
	player->prs.rot = model->rotModel;

	player->m_CSkinMesh.Update(g_mtxWorldPlayer);


#ifdef _DEBUG
	PrintDebugProc("Pos[X:%f Y:%f Z:%f]\n", player->prs.pos.x, player->prs.pos.y, player->prs.pos.z);
	PrintDebugProc("Anime[%d]  AnimeTime[%d]\n", PLAYER_ANIME_MAX - player->m_CSkinMesh.GetAnimTrack() - 1, player->m_CSkinMesh.GetAnimTime());
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	SetLight(LIGHT2, TRUE);
	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	player->m_CSkinMesh.Draw(pDevice, player->prs);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	SetLight(LIGHT2, FALSE);
}

//=============================================================================
// アニメーション設定関数
//=============================================================================
void SetPlayerAnime(int nPlayer,DWORD dAnime)
{
	PLAYER *player = GetPlayer(nPlayer);
	//MODEL *model = GetModel(0);
	//DWORD dwTemp = dAnime;

	//if (model->bDash)
	//{
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
	//}
	//else
	//{
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
	//}

	//dAnime = ANIME_MAX - dAnime - 1;
	if (dAnime != player->m_CSkinMesh.GetAnimTrack())
	{
		player->m_CSkinMesh.ChangeAnim(dAnime);

	//	if (dwTemp == ANIME08)
	//	{
	//		player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ATTACK);
	//		if (player->m_CSkinMesh.GetAnimTime() < 30)
	//		{
	//			player->m_CSkinMesh.SetAnimTime(30);
	//		}
	//	}
	//	else
	//	{
	//		if (model->bDash)
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
	//		}
	//		else
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
	//		}
	//	}
	};
}

//=============================================================================
// アニメーション設定関数
//=============================================================================
void CheckActionPlayer(void)
{
	MODEL *model = GetModel(0);
	PLAYER *player = GetPlayer(0);


	if (player->bRecoilFlag)
	{
		player->nRecoilCount++;
		if (player->nRecoilCount > PLAYER_RECOIL_TIME)
		{
			player->bRecoilFlag = false;
			player->nRecoilCount = 0;
		}
	}
	else if ((GetKeyboardPress(DIK_V) || IsMobUseRightPressed() || IsButtonPressed(0, R_BUTTON_R)) && CheckMagicModel(0, GetTypeMagic()))
	{	// マジックサークルチャージ
		player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ATTACK);
		SetPlayerAnime(0, PLAYER_ANIME_OVERDRAW);
	}
	else if (GetKeyboardRelease(DIK_V) || IsMobUseRightReleased() || IsButtonReleased(0, R_BUTTON_R))
	{
		SetPlayerAnime(0, PLAYER_ANIME_RECOIL);
		player->bRecoilFlag = true;
	}
	else if (model->bJump && player->nJumpFlag == PLAYER_JUMP_START)
	{
		player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_JUMP);
		SetPlayerAnime(0, PLAYER_ANIME_JUMP_START);
		player->nJumpFlag = PLAYER_JUMP_LOOP;
	}
	else if (player->nJumpFlag == PLAYER_JUMP_LOOP)
	{
		player->nJumpCount++;
		if (player->nJumpCount > PLAYER_JUMP_TIME_START)
		{
			SetPlayerAnime(0, PLAYER_ANIME_JUMP_LOOP);
			player->nJumpFlag = PLAYER_JUMP_END;
			player->nJumpCount = 0;
		}
	}
	else if (player->nJumpFlag == PLAYER_JUMP_END)
	{
		if (!model->bJump)
		{
			SetPlayerAnime(0, PLAYER_ANIME_JUMP_END);
			player->nJumpFlag = PLAYER_JUMP_LAST;
		}
	}
	else if (player->nJumpFlag == PLAYER_JUMP_LAST)
	{
		// 着地中は雲を足元に
		SetCloud(model->posModel);
		player->nJumpCount++;
		if (player->nJumpCount > PLAYER_JUMP_TIME_END)
		{
			player->nJumpFlag = PLAYER_JUMP_START;
			player->nJumpCount = 0;
		}
	}

	else
	{	
		bool bShot = false;
		if (IsMobUseLeftPressed() || GetKeyboardPress(DIK_B) || IsButtonPressed(0, R_BUTTON_ZR))
		{	// ノーマルバレット発射
			bShot = true;
		}
		// 移動処理
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
		{// 左移動
			if	(bShot)	{SetPlayerAnime(0, PLAYER_ANIME_AIM_LEFT);}
			else		{SetPlayerAnime(0, PLAYER_ANIME_LEFT);}
		}
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
		{// 右移動
			if (bShot) { SetPlayerAnime(0, PLAYER_ANIME_AIM_RIGHT); }
			else { SetPlayerAnime(0, PLAYER_ANIME_RIGHT); }
		}
		else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{// 前移動
			if (bShot) { SetPlayerAnime(0, PLAYER_ANIME_AIM_FORWARD); }
			else { SetPlayerAnime(0, PLAYER_ANIME_FORWARD); }
		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// 後移動
			if (bShot) { SetPlayerAnime(0, PLAYER_ANIME_AIM_BACK); }
			else { SetPlayerAnime(0, PLAYER_ANIME_BACK); }
		}
		else
		{// 移動なし
			if (bShot) { SetPlayerAnime(0, PLAYER_ANIME_AIM_IDLE); }
			else { SetPlayerAnime(0, PLAYER_ANIME_IDLE); }
		}




		if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A)
			|| GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D)
			|| IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_DOWN)
			|| IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, BUTTON_RIGHT)
			|| IsButtonPressed(0, LSTICK_UP) || IsButtonPressed(0, LSTICK_DOWN)
			|| IsButtonPressed(0, LSTICK_LEFT) || IsButtonPressed(0, LSTICK_RIGHT))
		{
			// 移動中は雲を足元に
			SetCloud(model->posModel);
		}
		if (model->bDash)
		{
			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
		}
		else
		{
			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
		}
	}


}

//=============================================================================
// 取得関数
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}