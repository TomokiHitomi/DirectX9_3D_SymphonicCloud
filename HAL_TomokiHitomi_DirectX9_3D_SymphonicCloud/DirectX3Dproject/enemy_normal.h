//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#ifndef _ENEMY_NORMAL_H_
#define _ENEMY_NORMAL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ENEMY_NORMAL_MODEL		"data/MODEL/cube.x"							// �ǂݍ��ރ��f����

#define ENEMY_NORMAL_MAX		(30)
#define ENEMY_NORMAL_SCALE		(0.3f)
#define ENEMY_NORMAL_SIZE		(20.0f)
#define ENEMY_NORMAL_ROT		(0.02f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotEnemy;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclEnemy;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	bool			bUse;
}ENEMY_NORMAL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy_normal(int nType);
void UninitEnemy_normal(void);
void UpdateEnemy_normal(void);
void DrawEnemy_normal(void);
int SetEnemy_normal(void);
void SetEnemy_normalPos(int nIndex, D3DXVECTOR3 pos);
void SetEnemy_normalColor(int nColor);
void ReleaseEnemy_normal(int nIndex);
ENEMY_NORMAL *GetEnemy_normal(int no);

#endif
