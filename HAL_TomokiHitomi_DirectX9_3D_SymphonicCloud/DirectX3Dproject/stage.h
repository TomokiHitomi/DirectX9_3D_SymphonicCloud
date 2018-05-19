//=============================================================================
//
// Stage���� [stage.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		STAGE_END		(100)
#define		STAGE_END_VOICE	(10)


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	int					nTime;
	int					nEndCount;
	bool				bClearFlag;
	bool				bEndFlag;
}STAGE_SYS;


//*****************************************************************************
// �v���g�^�C�v�錾
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