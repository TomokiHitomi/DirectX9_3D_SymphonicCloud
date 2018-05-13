//=============================================================================
//
// ���f������ [skydomeeffect.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _SKYDOMEEFFECT_H_
#define _SKYDOMEEFFECT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	SKYDOMEEFFECT_MODEL		"data/MODEL/Skydome/Dome_EE401.x"	// �ǂݍ��ރ��f����
#define	SKYDOMEEFFECT_TEXTURE	"data/MODEL/Skydome/EE401.png"	// �ǂݍ��ރ��f����

#define SKYDOMEEFFECT_MAX			(1)
#define SKYDOMEEFFECT_HEIGHT		(200.0f)

#define SKYDOMEEFFECT_SCALE		(3.0f)
#define SKYDOMEEFFECT_ROT_SPEED	(-0.0003f)

typedef struct
{
	D3DXVECTOR3		posSkydomeeffect;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotSkydomeeffect;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclSkydomeeffect;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X

}SKYDOMEEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSkydomeeffect(int nType);
void UninitSkydomeeffect(void);
void UpdateSkydomeeffect(void);
void DrawSkydomeeffect(void);
SKYDOMEEFFECT *GetSkydomeeffect(int no);

#endif
