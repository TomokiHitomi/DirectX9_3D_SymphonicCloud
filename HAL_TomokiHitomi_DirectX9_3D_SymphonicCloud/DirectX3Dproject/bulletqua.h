//=============================================================================
//
// �o���b�g���� [bulletqua.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _BULLETQUA_H_
#define _BULLETQUA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETQUA	"data/TEXTURE/bulletanime.png"						// �ǂݍ��ރe�N�X�`���t�@�C����


// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_BULLETQUA	(5)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA	(15)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_BULLETQUA		(TEXTURE_PATTERN_DIVIDE_X_BULLETQUA*TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA)

// �A�j���[�V����
#define TIME_ANIMATION_TEX	(3)

// ���e�X�g�l
#define	BULLETQUA_ALPHA		(100)

// ���l�ύX���x
#define	BULLETQUA_ALPHA_CHANGE	(0.01f)

// �T�C�Y
#define	BULLETQUA_SIZE_X		(15.0f)
#define	BULLETQUA_SIZE_Y		(15.0f)


// �ő吔
#define BULLETQUA_MAX			(2000)

// �ړ��X�s�[�h
#define BULLETQUA_SPEED		(10.0f)

#define BULLETQUA_FALSE_COUNT		(500)		// ���ŃJ�E���g

#define BULLETQUA_SIZE_CHANGE		(0.06f)		// �G�t�F�N�g�T�C�Y�`�F���W
#define BULLETQUA_ALPHA_CHANGE		(0.08f)		// �G�t�F�N�g���`�F���W

// �o���b�g�e�X�g�p
#define BULLETQUA_TAG_VECTOR		(5000)

// �o���b�gRGBA
#define BULLETQUA_DIF_R		(0.8f)
#define BULLETQUA_DIF_G		(0.0f)
#define BULLETQUA_DIF_B		(0.8f)
#define BULLETQUA_DIF_A		(1.0f)


// �G�l�~�[�p
#define BULLETQUA_ENEMY_SPEED		(5.0f)

#define BULLETQUA_ENEMY_DIF_R		(0.0f)
#define BULLETQUA_ENEMY_DIF_G		(0.8f)
#define BULLETQUA_ENEMY_DIF_B		(0.8f)
#define BULLETQUA_ENEMY_DIF_A		(1.0f)

#define	BULLETQUA_ENEMY_SIZE_X		(100.0f)
#define	BULLETQUA_ENEMY_SIZE_Y		(100.0f)

typedef struct
{
	//VERTEX_3D			vertexWK[NUM_VERTEX];	// ���_���i�[���[�N
	VERTEX_3D			*vertexWK;	// ���_���i�[���[�N

	D3DXVECTOR3			posBulletQua;				// �ʒu
	D3DXVECTOR3			rotBulletQua;				// ����(��])
	D3DXVECTOR3			sclBulletQua;				// �傫��(�X�P�[��)
	D3DXVECTOR3			vecTag;

	D3DXVECTOR2			vec2Size;				// �T�C�Y

	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXCOLOR			colorBulletQua;

	int					nUseCount;				// ���ŃJ�E���g
	int					nEffectCount;
	int					nSysNum;
	int					nTex;
	int					nAnimeCount;

	float				fMoveSpeed;				// �ړ����x
	float				fHAngle;
	float				fVAngle;
	
	bool				bUse;					// �g�p�t���O
	bool				bEnemy;
	bool				bVertex;
	bool				bDraw;

	// �V���h�E�p
	int					nIdxShadow;
	float				fSizeShadow;
	float				fSize;
	bool				bShadow;
	D3DXCOLOR			colShadow;
}BULLETQUA;


// �V�X�e���̍ő吔
#define	BULLETQUA_SYS_MAX			(100)

#define	BULLETQUA_SYS_TIME			(50)
#define	BULLETQUA_SYS_SPLIT		(40)

#define	BULLETQUA_SYS_WAVE			(1.5f)

#define BULLETQUA_SYS_SPEED		(3.0f)
#define BULLETQUA_SYS_COOLDOWN	(500)

typedef struct
{
	// �N�H�[�^�j�I���p�ϐ�
	D3DXVECTOR3			vecTa;			// ��]���a��ݒ�
	D3DXVECTOR3			vecAxis;		// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�

	D3DXVECTOR3			vecTagPos;
	D3DXVECTOR2			vec2Size;		// �T�C�Y

	D3DXCOLOR			colorBulletQua;

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
	int					nBulletCount;

	bool				bUse;
	bool				bProcess;

}BULLETQUA_SYS;

enum
{
	BULLET_QUA_ROT_L,
	BULLET_QUA_ROT_R,
	BULLET_QUA_ROT_LL,
	BULLET_QUA_ROT_RR,
	BULLET_QUA_ROT_LV,
	BULLET_QUA_ROT_RV
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletQua(int nType);
void InitStatusBulletQua(int nBulletQua);
void UninitBulletQua(void);
void UpdateBulletQua(void);
void DrawBulletQua(void);
void SetBulletQua(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2);
void SetEnemyBulletQua00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2);
void SetSysBulletQua(int nType, int nTex, int nSize, int nColor, int nSplitint, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos);
BULLETQUA *GetBulletQua(int no);

#endif
