//=============================================================================
//
// �E�H�[������ [wall.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_WALL	"data/TEXTURE/wall000.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����

#define	WALL_SIZE_X		(100.0f)			// �n�ʂ̃T�C�Y(X����)
#define	WALL_SIZE_Z		(100.0f)			// �n�ʂ̃T�C�Y(Z����)

#define WALL_X				(15)
#define WALL_Y				(3)
#define WALL_MAX			(WALL_X * WALL_Y)	// �t�B�[���h�̍ő吔

typedef struct
{
	D3DXVECTOR3				posWall;			// �n�ʂ̈ʒu
	D3DXVECTOR3				rotWall;			// �n�ʂ̌���(��])
	D3DXVECTOR3				sclWall;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX				mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX				mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX				mtxWorld;			// ���[���h�}�g���b�N�X
}WALL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWall(int nType);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
WALL *GetWall(int no);

#endif
