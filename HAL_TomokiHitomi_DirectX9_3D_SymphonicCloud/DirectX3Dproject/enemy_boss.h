//=============================================================================
//
// �G�l�~�[���f���i�{�X�j���� [enemy_boss.h]
// Author : GP12A295 25 �l���F�� 
//
//=============================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ENEMY_BOSS_MODEL	"data/MODEL/iv_grimreaper/grimreaper.x"			// �ǂݍ��ރ��f����
#define ENEMY_BOSS_MAX			(1)
#define ENEMY_BOSS_SCALE		(0.4f)
#define ENEMY_BOSS_SIZE			(50.0f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotEnemy;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclEnemy;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	bool			bUse;
}ENEMY_BOSS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy_boss(int nType);
void UninitEnemy_boss(void);
void UpdateEnemy_boss(void);
void DrawEnemy_boss(void);
int SetEnemy_boss(void);
void SetEnemy_bossPos(int nIndex, D3DXVECTOR3 pos, float rot);
void ReleaseEnemy_boss(int nIndex);
void SetWaveEnemy(int no);
ENEMY_BOSS *GetEnemy_boss(int no);

#endif
