//=============================================================================
//
// Game処理 [game.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "sound.h"
#include "pause.h"
#include "enemy.h"
#include "magic.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_normal.h"
#include "calculate.h"

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
GAME_SYS					gameSysWk;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	SetSoundBgmBoss(false);
	for (int i = 0; i < GAME_MAGIC_MAX; i++)
	{
		gameSys->nShuffle[i] = i;
	}
	SetShuffle(&gameSys->nShuffle[0], GAME_MAGIC_MAX);
	gameSys->nTotalCount = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	gameSys->nTotalCount++;

	if (gameSys->nTotalCount == 100)
	{
		SetVoice(VOICE_WARENICHIKARAWO, E_DS8_FLAG_NONE, CONTINUITY_ON);
	}
	if (gameSys->nTotalCount == 200)
	{
		//SetMagicType(BULLET_SPECIAL_WATER);
		//SetMagicType(BULLET_SPECIAL_ICE);
		//SetMagicType(BULLET_SPECIAL_EARTH);
		//SetMagicType(BULLET_SPECIAL_THUNDER);
		//SetMagicType(BULLET_SPECIAL_WIND);
		//SetMagicType(BULLET_SPECIAL_FIRE);
		SetEnemy_normalColor(GetGameShuffle(1 - 1) + 1);
		SetWaveEnemy(1);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{

}

//=============================================================================
// 取得関数
//=============================================================================
GAME_SYS *GetGameSys(void)
{
	return(&gameSysWk);
}

//=============================================================================
// カウント取得関数
//=============================================================================
int GetGameSysCount(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	return(gameSys->nTotalCount);
}

//=============================================================================
// シャッフル値取得関数
//=============================================================================
int GetGameShuffle(int nWave)
{
	GAME_SYS *gameSys = &gameSysWk;
	return(gameSys->nShuffle[nWave]);
}