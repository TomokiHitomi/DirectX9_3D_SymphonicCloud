//=============================================================================
//
// ���f������ [skydome.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _SKYDOME_H_
#define _SKYDOME_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	SKYDOME_MODEL		"data/MODEL/Skydome/skydome.x"	// �ǂݍ��ރ��f����
//#define	SKYDOME_TEXTURE		"data/MODEL/Skydome/incskies_014_8k.png"	// �ǂݍ��ރ��f����

#define	SKYDOME_TEXTURE		"data/MODEL/Skydome/QQ101.png"	// �ǂݍ��ރ��f����

#define SKYDOME_MAX			(1)
//#define SKYDOME_HEIGHT		(200.0f)
#define SKYDOME_HEIGHT		(-200.0f)

#define SKYDOME_SCALE		(30.0f)
#define SKYDOME_ROT_SPEED	(0.00001f)

typedef struct
{
	D3DXVECTOR3		posSkydome;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotSkydome;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclSkydome;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X

}SKYDOME;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSkydome(int nType);
void UninitSkydome(void);
void UpdateSkydome(void);
void DrawSkydome(void);
SKYDOME *GetSkydome(int no);

#endif
