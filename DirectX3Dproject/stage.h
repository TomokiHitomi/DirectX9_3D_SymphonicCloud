//=============================================================================
//
// Stage処理 [stage.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		STAGE_END		(100)
#define		STAGE_END_VOICE	(10)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	int					nTime;
	int					nEndCount;
	bool				bClearFlag;
	bool				bEndFlag;
}STAGE_SYS;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStage(void);
void UpdateStage(void);
STAGE_SYS *GetStageSys(void);
void SetResultTime(int nTime);
int GetResultTime(void);
void SetClearFlag(bool bClearFlag);
bool GetClearFlag(void);
bool GetEndFlag(void);

#endif