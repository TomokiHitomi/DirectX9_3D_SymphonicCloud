//=============================================================================
//
// Stage処理 [stage.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "stage.h"
#include "sound.h"
#include "pause.h"
#include "enemy.h"
#include "magic.h"
#include "bullet.h"

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
STAGE_SYS					stageSysWk;

//=============================================================================
// 更新処理
//=============================================================================

HRESULT InitStage(void)
{
	STAGE_SYS *stageSys = &stageSysWk;

	stageSys->nTime = 0;
	stageSys->bClearFlag = false;
	stageSys->bEndFlag = false;
	return S_OK;
}

void UninitStage(void)
{

}

void UpdateStage(void)
{
	STAGE_SYS *stageSys = &stageSysWk;
}

void DrawStage(void)
{

}

//=============================================================================
// 取得関数
//=============================================================================
STAGE_SYS *GetStageSys(void)
{
	return(&stageSysWk);
}


//=============================================================================
// カウント取得関数
//=============================================================================
void SetResultTime(int nTime)
{
	STAGE_SYS *stageSys = &stageSysWk;
	stageSys->nTime = nTime;
}

//=============================================================================
// カウント取得関数
//=============================================================================
int GetResultTime(void)
{
	STAGE_SYS *stageSys = &stageSysWk;
	return(stageSys->nTime);
}

//=============================================================================
// クリアフラグ取得関数
//=============================================================================
void SetClearFlag(bool bClearFlag)
{
	STAGE_SYS *stageSys = &stageSysWk;
	stageSys->bClearFlag = bClearFlag;
	stageSys->bEndFlag = true;
}

//=============================================================================
// クリアフラグ取得関数
//=============================================================================
bool GetClearFlag(void)
{
	STAGE_SYS *stageSys = &stageSysWk;
	return(stageSys->bClearFlag);
}

//=============================================================================
// 終了フラグ取得関数
//=============================================================================
bool GetEndFlag(void)
{
	STAGE_SYS *stageSys = &stageSysWk;
	return(stageSys->bEndFlag);
}