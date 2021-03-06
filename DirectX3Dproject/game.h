//=============================================================================
//
// Game処理 [game.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_GRAVITI	(3.0f)		// 重力
#define GAME_MAGIC_MAX	(6)			// 最大マジック数

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	int					nTotalCount;
	int					nShuffle[GAME_MAGIC_MAX];
}GAME_SYS;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UpdateGame(void);
GAME_SYS *GetGameSys(void);
int GetGameSysCount(void);
int GetGameShuffle(int nWave);

#endif