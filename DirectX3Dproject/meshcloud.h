//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshcloud.h]
// Author :  GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _MESHCLOUD_H_
#define _MESHCLOUD_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/cloud.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define MESHCLOUD_MAX				(1)

#define MESHCLOUD_X					(120)
#define MESHCLOUD_Z					(120)

#define MESHCLOUD_POS_X				(0.0f)
#define MESHCLOUD_POS_Z				(0.0f)

#define MESHCLOUD_RESET_X			(1000.0f)
#define MESHCLOUD_RESET_Z			(1000.0f)

#define MESHCLOUD_SIZE_X			(50)
#define MESHCLOUD_SIZE_Z			(50)

#define MESHCLOUD_WAVE				(5)
#define MESHCLOUD_WAVE_SPEED		(0.02f)

//#define MESHCLOUD_DIFFUSE_R			(1.0f)
//#define MESHCLOUD_DIFFUSE_G			(1.0f)
//#define MESHCLOUD_DIFFUSE_B			(1.0f)
//#define MESHCLOUD_DIFFUSE_A			(0.8f)

#define MESHCLOUD_DIFFUSE_R			(1.0f)
#define MESHCLOUD_DIFFUSE_G			(1.0f)
#define MESHCLOUD_DIFFUSE_B			(1.0f)
#define MESHCLOUD_DIFFUSE_A			(0.95f)


typedef struct
{
	VERTEX_3D						*vertexWK;	// ���_���i�[���[�N

	D3DXVECTOR3						vecPos;				// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3						vecRot;				// �|���S���̉�]�p

	D3DXMATRIX						mtxWorldCloud;		// ���[���h�}�g���b�N�X

	float							fVAnglel;
}MESHCLOUD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshcloud(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int nType);
HRESULT InitMeshcloudPos(void);
void UninitMeshcloud(void);
void UpdateMeshcloud(void);
void DrawMeshcloud(void);

#endif
