//=============================================================================
//
// Lockon���� [lockon.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_LOCKON	"data/TEXTURE/lockon.png"						// �ǂݍ��ރe�N�X�`���t�@�C����

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_LOCKON	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_LOCKON	(1)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_LOCKON		(TEXTURE_PATTERN_DIVIDE_X_LOCKON*TEXTURE_PATTERN_DIVIDE_Y_LOCKON)

// ���e�X�g�l
#define	LOCKON_ALPHA		(1)

// �T�C�Y
#define	LOCKON_SIZE_X		(70.0f)
#define	LOCKON_SIZE_Y		(70.0f)

#define	LOCKON_LENGTH		(500.0f)

#define LOCKON_SIZE_CHANGE	((LOCKON_SIZE_X + LOCKON_SIZE_Y) / 2 * 0.05f)

#define LOCKON_ALPHA_CHANGE	(LOCKON_SIZE_CHANGE * 0.1f)

// �ő吔
#define LOCKON_MAX				(100)

// �Ďg�p�܂ł̃J�E���g
#define LOCKON_COOLDOWN			(3)

// RGBA
#define LOCKON_DIF_R			(1.0f)
#define LOCKON_DIF_G			(1.0f)
#define LOCKON_DIF_B			(1.0f)
#define LOCKON_DIF_A			(1.0f)

#define LOCKON_RADIUS			(0.6f)
#define LOCKON_RELEASE_COUNT	(70)

#define LOCKON_SCALE_CHANGE		(0.05f)
#define LOCKON_SCALE_MIN		(0.6f)
#define LOCKON_SCALE_MAX		(1.0f)

typedef struct
{
	//VERTEX_3D				vertexWK[NUM_VERTEX];	// ���_���i�[���[�N
	VERTEX_3D				*vertexWK;	// ���_���i�[���[�N

	D3DXVECTOR3				posLockon;				// �n�ʂ̈ʒu
	D3DXVECTOR3				rotLockon;				// �n�ʂ̌���(��])
	D3DXVECTOR3				sclLockon;				// �n�ʂ̑傫��(�X�P�[��)

	D3DXVECTOR2				vec2Size;

	D3DXMATRIX				mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX				mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

	D3DXCOLOR				colorLockon;

	int						nUseCount;				// ���ŃJ�E���g
	int						nTex;
	int						nIndex;
	int						nCoolDown;
	int						nReleaseCount;

	float					fSizeChange;
	float					fAlphaChange;

	bool					bUse;					// �g�p�t���O
	bool					bRelease;				// �j���t���O
}LOCKON;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLockon(int nType);
void InitStatusLockon(int nLockon);
void UninitLockon(void);
void UpdateLockon(void);
void DrawLockon(void);
int SetLockon(D3DXVECTOR3 vecPos);
void SetPosLockon(int nIndex, D3DXVECTOR3 vecPos);
void ReleaseLockon(int nIndex);
bool AddReleaseCountLockon(int nIndex);
void ResetReleaseCountLockon(int nIndex);
LOCKON *GetLockon(int no);

#endif
