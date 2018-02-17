//=============================================================================
//
// RETICLE���� [reticle.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_RETICLE		(2)	// �|���S����

#define TEXTURE_GAME_RETICLE00		("data/TEXTURE/reticle00.png")	// RETICLE�p�摜
#define TEXTURE_GAME_RETICLE01		("data/TEXTURE/reticle01.png")
#define TEXTURE_GAME_RETICLE_CENTER	("data/TEXTURE/reticlecenter.png")


#define TEXTURE_RETICLE_SIZE_X	(400 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_RETICLE_SIZE_Y	(400 * SCREEN_SCALE / 2) // ����

#define TEXTURE_RETICLE_CENTER_SIZE_X	(32 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_RETICLE_CENTER_SIZE_Y	(32 * SCREEN_SCALE / 2) // ����

#define TEXTURE_RETICLE_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_RETICLE_G			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_RETICLE_B			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_RETICLE_A			(1.0f) // �e�N�X�`���J���[��

#define TEXTURE_RETICLE_CHANGE_COLLAR		(0.005f) // �e�N�X�`���J���[�ύX���x

#define TEXTURE_PATTERN_DIVIDE_X_RETICLE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_RETICLE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_RETICLE			(TEXTURE_PATTERN_DIVIDE_X_RETICLE*TEXTURE_PATTERN_DIVIDE_Y_RETICLE)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_RETICLE			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define RETICLE_MAX					(3)	// RETICLE�̍ő吔
#define RETICLE_ROTATION_SPEED			(0.002f)	// ��]���x

#define RETICLE_RESULT_SCALE		(0.7f)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// RETICLE�̈ړ���
	D3DXVECTOR3				rot;					// RETICLE�̉�]��
	D3DXVECTOR2				size;					// RETICLE�̃e�N�X�`���T�C�Y(x,y)
	float					fScale;					// �X�P�[��
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	D3DXCOLOR				colorMagic;				// �F�Ǘ�
	int						nCollarFlag;			// �F�ύX�t���O

	bool					bUse;					// true:�g�p  false:���g�p


}RETICLE;

enum
{
	RETICLE_1,
	RETICLE_2,
	RETICLE_CENTER
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitReticle(int type);
void UninitReticle(void);
void UpdateReticle(void);
void DrawReticle(void);
RETICLE *GetReticle(int no);
void SetReticle(void);
void ReleaseReticle(void);

#endif
