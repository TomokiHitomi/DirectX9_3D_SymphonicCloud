//=============================================================================
//
// Cloud���� [cloud.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_CLOUD	"data/TEXTURE/p_cloud.png"				// �ǂݍ��ރe�N�X�`���t�@�C����

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_CLOUD	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_CLOUD	(1)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_CLOUD		(TEXTURE_PATTERN_DIVIDE_X_CLOUD*TEXTURE_PATTERN_DIVIDE_Y_CLOUD)

// ���e�X�g�l
#define	CLOUD_ALPHA		(1)

// �T�C�Y
#define	CLOUD_SIZE_X		(15.0f)
#define	CLOUD_SIZE_Y		(15.0f)

#define CLOUD_SIZE_CHANGE	((CLOUD_SIZE_X + CLOUD_SIZE_Y) / 2 * 0.05f)

#define CLOUD_ALPHA_CHANGE	(CLOUD_SIZE_CHANGE * 0.1f)

// �ő吔
#define CLOUD_MAX			(1000)

// RGBA
#define CLOUD_DIF_R		(0.8f)
#define CLOUD_DIF_G		(0.8f)
#define CLOUD_DIF_B		(0.8f)
#define CLOUD_DIF_A		(1.0f)


typedef struct
{
	//VERTEX_3D				vertexWK[NUM_VERTEX];	// ���_���i�[���[�N
	VERTEX_3D				*vertexWK;	// ���_���i�[���[�N

	D3DXVECTOR3				posCloud;				// �n�ʂ̈ʒu
	D3DXVECTOR3				rotCloud;				// �n�ʂ̌���(��])
	D3DXVECTOR3				sclCloud;				// �n�ʂ̑傫��(�X�P�[��)

	D3DXVECTOR2				vec2Size;

	D3DXMATRIX				mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX				mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

	D3DXCOLOR				colorCloud;

	int						nUseCount;				// ���ŃJ�E���g
	int						nTex;

	float					fSizeChange;
	float					fAlphaChange;

	bool					bUse;					// �g�p�t���O
}CLOUD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCloud(int nType);
void InitStatusCloud(int nCloud);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);
void SetCloud(D3DXVECTOR3 vecPos);
CLOUD *GetCloud(int no);

#endif
