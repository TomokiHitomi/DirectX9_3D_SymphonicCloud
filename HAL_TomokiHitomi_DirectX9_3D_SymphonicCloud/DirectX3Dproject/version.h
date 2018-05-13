//=============================================================================
//
// VERSION���� [version.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _VERSION_H_
#define _VERSION_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERSION		(2)	// �|���S����

#define TEXTURE_GAME_VERSION	("data/TEXTURE/version.png")	// VERSION�p�摜
#define TEXTURE_VERSION_SIZE_X	(180 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_VERSION_SIZE_Y	(30 * SCREEN_SCALE/2) // ����

#define VERSION_POS_X				(1100 * SCREEN_SCALE)			// �������WX
#define VERSION_POS_Y				(693 * SCREEN_SCALE)			// �������WY

#define TEXTURE_VERSION_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_VERSION_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_VERSION_B			(255) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_VERSION	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_VERSION	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_VERSION			(TEXTURE_PATTERN_DIVIDE_X_VERSION*TEXTURE_PATTERN_DIVIDE_Y_VERSION)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_VERSION				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define VERSION_MAX					(1)	// VERSION�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// VERSION�̈ړ���
	D3DXVECTOR3				rot;					// VERSION�̉�]��
	D3DXVECTOR2				size;					// VERSION�̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}VERSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitVersion(int type);
void UninitVersion(void);
void UpdateVersion(void);
void DrawVersion(void);
VERSION *GetVersion(int no);


#endif
