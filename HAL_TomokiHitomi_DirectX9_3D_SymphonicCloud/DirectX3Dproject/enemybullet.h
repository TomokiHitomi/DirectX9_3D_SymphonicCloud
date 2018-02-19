//=============================================================================
//
// �o���b�g���� [enemybullet.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ENEMYBULLET	"data/TEXTURE/enemybullet.png"						// �ǂݍ��ރe�N�X�`���t�@�C����

#define ENEMYBULLET_MAX				(2000)		// �ő吔
#define	ENEMYBULLET_ALPHA			(20)		// ���e�X�g�l
#define	ENEMYBULLET_ALPHA_CHANGE	(0.01f)		// ���l�ύX���x
#define	ENEMYBULLET_SIZE_X			(5.0f)		// �T�C�YX
#define	ENEMYBULLET_SIZE_Y			(5.0f)		// �T�C�YY

#define ENEMYBULLET_SPEED_NORMAL	(10.0f)		// �o���b�g�X�s�[�h
#define ENEMYBULLET_FALSE_COUNT		(300)		// �o���b�g���ŃJ�E���g
#define ENEMYBULLET_DIF_R			(0.0f)		// �o���b�gRGBA
#define ENEMYBULLET_DIF_G			(0.8f)
#define ENEMYBULLET_DIF_B			(0.8f)
#define ENEMYBULLET_DIF_A			(1.0f)

#define ENEMYBULLET_SIZE_CHANGE		(0.06f)		// �G�t�F�N�g�T�C�Y�`�F���W
#define ENEMYBULLET_ALPHA_CHANGE	(0.08f)		// �G�t�F�N�g���`�F���W


// �z�[�~���O
#define ENEMYBULLET_VEC_POWER		(0.5f)		// �U����
#define ENEMYBULLET_HOMING_LENGTH	(50.0f)
#define ENEMYBULLET_HOMING_UPDATE	(3)

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET	(5)
#define TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET	(6)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_ENEMYBULLET		(TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET*TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET)
#define TEXTURE_PATTERN_ENEMYBULLET			(TEXTURE_PATTERN_NU_ENEMYBULLET / 5)
// �A�j���[�V����
#define TIME_ANIMATION_ENEMYBULLET	(3)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			posEnemybullet;				// �ʒu
	D3DXVECTOR3			rotEnemybullet;				// ����(��])
	D3DXVECTOR3			sclEnemybullet;				// �傫��(�X�P�[��)
	D3DXVECTOR3			vecTag;
	D3DXVECTOR2			vec2Size;				// �T�C�Y
	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXCOLOR			colorEnemybullet;

	int					nUseCount;				// ���ŃJ�E���g
	int					nEffectCount;
	int					nSysNum;
	int					nTex;
	int					nAnimeCount;
	int					nAnimePattern;
	int					nType;

	float				fMoveSpeed;				// �ړ����x
	float				fHAngle;
	float				fVAngle;
	float				fVecPower;
	
	bool				bUse;					// �g�p�t���O
	bool				bEnemy;
	bool				bVertex;
	bool				bHoming;

	// �V���h�E�p
	int					nIdxShadow;
	float				fSizeShadow;
	float				fSize;
	D3DXCOLOR			colShadow;
}ENEMYBULLET;

enum
{
	ENEMYBULLET_NORMAL,
	ENEMYBULLET_HOMING,
	ENEMYBULLET_NORMAL_HOMING
};

enum
{
	ENEMYBULLET_TEX_1,
	ENEMYBULLET_TEX_2
};


// �V�X�e���̍ő吔
#define	ENEMYBULLET_SYS_MAX			(500)
#define	ENEMYBULLET_SYS_TIME		(50)
#define	ENEMYBULLET_SYS_SPLIT		(40)
#define	ENEMYBULLET_SYS_WAVE		(1.5f)
#define ENEMYBULLET_SYS_SPEED		(3.0f)
#define ENEMYBULLET_SYS_COOLDOWN	(500)

// �R�c�|���S�����_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


typedef struct
{
	D3DXVECTOR3			vecTagPos;
	D3DXVECTOR2			vec2Size;		// �T�C�Y

	D3DXCOLOR			colorEnemybullet;

	float				fHAngle;
	float				fVAngle;
	float				fSplitAngle;
	float				fMoveSpeed;
	float				fWave;
	float				fAddAngle;
	float				fLength;

	int					nType;
	int					nFireTime;
	int					nSplitCount;
	int					nCoolDown;
	int					nTex;
	int					nEnemybulletCount;

	bool				bUse;
	bool			bProcess;

}ENEMYBULLET_SYS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemybullet(int nType);
void InitStatusEnemybullet(int nEnemybullet);
void UninitEnemybullet(void);
void UpdateEnemybullet(void);
void DrawEnemybullet(void);
void SetEnemybullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor);
void SetSysEnemybullet(int nType, int nTex, int nSize, int nColor, int nSplitint, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos);
void SetHomingEnemybullet(D3DXVECTOR3 posSet, float vecPower, D3DXCOLOR xColor);
ENEMYBULLET *GetEnemybullet(int no);
void SetEndFlag(void);

#endif
