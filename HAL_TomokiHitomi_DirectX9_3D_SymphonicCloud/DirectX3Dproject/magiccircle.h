//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAGICCIRCLE_MODEL		"data/MODEL/magiccircle.x"							// �ǂݍ��ރ��f����

#define MAGICCIRCLE_MAX					(5)
#define MAGICCIRCLE_SCALE				(18.0f)
#define MAGICCIRCLE_SCALE_Y				(23.0f)
#define MAGICCIRCLE_SIZE				(25.0f)
#define MAGICCIRCLE_ROT_Y				(0.03f)
#define MAGICCIRCLE_ROT					(0.003f)

#define MAGICCIRCLE_HEIGHT				(14.0f)
#define MAGICCIRCLE_HEIGHT_INTERVAL		(7.0f)

#define MAGICCIRCLE_SCALE_CHANGE		(0.7f)

#define MAGICCIRCLE_ALPHA					(1)

typedef struct
{
	D3DXVECTOR3		pos;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rot;			// �n�ʂ̌���(��])
	D3DXVECTOR3		scl;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	bool			bUse;
	bool			bSe;
}MAGICCIRCLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMagiccircle(int nType);
void UninitMagiccircle(void);
void UpdateMagiccircle(void);
void DrawMagiccircle(void);
int SetMagiccircle(void);
void SetMagiccirclePos(int nIndex, D3DXVECTOR3 pos, float fRotY);
void ReleaseMagiccircle(int nIndex);
MAGICCIRCLE *GetMagiccircle(int no);

#endif
