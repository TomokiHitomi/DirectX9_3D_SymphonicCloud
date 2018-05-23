//=============================================================================
//
// ���f������ [cloudfield.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _CLOUDFIELD_H_
#define _CLOUDFIELD_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	CLOUDFIELD_MODEL		"data/MODEL/FIELD/cloudfield.x"	// �ǂݍ��ރ��f����
#define	CLOUDFIELD_MODEL		"data/MODEL/FIELD/cloudfield.x"	// �ǂݍ��ރ��f����

#define	CLOUDFIELD_TEXTURE		"data/TEXTURE/cloud.png"	// �ǂݍ��ރ��f����

#define CLOUDFIELD_MAX			(1)
#define CLOUDFIELD_HEIGHT		(0.0f)

#define CLOUDFIELD_SCALE		(1.0f)
#define CLOUDFIELD_ROT_SPEED	(0.00001f)

typedef struct
{
	D3DXVECTOR3		posCloudfield;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotCloudfield;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclCloudfield;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X

}CLOUDFIELD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCloudfield(int nType);
void UninitCloudfield(void);
void UpdateCloudfield(void);
void DrawCloudfield(void);
CLOUDFIELD *GetCloudfield(int no);
LPD3DXBASEMESH GetCloudfieldMesh(void);

#endif
