//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_SYSTEM_FILE			("data/FILE/enemy_system.csv")
#define ENEMY_SYSTEM_DATA_MAX		(8)

#define ENEMY_WAVE_MAX				(7)		// �ő�E�F�[�u��

//#define	ENEMY_CUBE		"data/MODEL/cube.x"							// �ǂݍ��ރ��f����

#define ENEMY_MAX		(100)

#define ENEMY_SCALE		(0.2f)
#define ENEMY_SIZE		(25.0f)

#define ENEMY_HEIGHT	(30.0f)
#define ENEMY_CENTER	(25.0f)

// �G�l�~�[�̃o���b�g���ˈʒu
#define ENEMY_BULLET_MUZZELE_HEIGHT	(35.0f)				// ����
#define ENEMY_BULLET_MUZZELE_LENGTH	(1.0f)				// ����
#define ENEMY_BULLET_MUZZELE_ANGLE	(D3DX_PI * 1.5f)

#define ENEMY_ATTACK_NORMAL_INTERVAL	(20)
#define ENEMY_ATTACK_SPECIAL_INTERVAL	(100)

#define ENEMY_MOVE_SPEED	(2)
#define ENEMY_MOVE_SLANT	(ENEMY_MOVE_SPEED * 0.71)

#define ENEMY_ACTIVE_COUNT_MAX	(5)


// �G�l�~�[�̃X�e�[�^�X
#define ENEMY_STATUS_HP	(50.0f)
#define ENEMY_STATUS_BOSS_HP	(600.0f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotEnemy;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclEnemy;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X

	int				nAttackNorCount;
	int				nAttackSpCount;
	int				nAttackPattern;

	float			fStatusHP;
	float			fAngle;
	float			fMoveSpeed;

	bool			bUse;
	bool			bShadow;
	bool			bParameter;

	int				nWave;		// �o���E�F�[�u
	int				nMoveType;
	int				nAttackType;

	// ���f��
	int				nModel;
	int				nModelIndex;
	bool			bModelSet;

	// �V���h�E�p
	int				nIdxShadow;
	float			fSizeShadow;
	D3DXCOLOR		colShadow;

	// ���b�N�I���p
	bool			bLockon;
	int				nIdxLockon;

	// �~�j�}�b�v�p
	bool			bUseMinimap;
}ENEMY;

typedef struct
{
	int					nActiveEnemy[ENEMY_ACTIVE_COUNT_MAX];
	int					nActiveCount;
}ENEMY_SYS;

enum
{
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_BOSS
};

enum
{
	ENEMY_MOVE_TRACK
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int nType);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemyFile(void);
ENEMY *GetEnemy(int no);
ENEMY_SYS *GetEnemySys(void);
void SetWaveEnemy(int no);
void ReleaseEnemy(int nEnemy);

#endif
