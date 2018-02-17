//=============================================================================
//
// COPYRIGHT���� [copyright.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _COPYRIGHT_H_
#define _COPYRIGHT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_COPYRIGHT		(2)	// �|���S����

#define TEXTURE_GAME_COPYRIGHT	("data/TEXTURE/copyright.png")	// COPYRIGHT�p�摜
#define TEXTURE_COPYRIGHT_SIZE_X	(300 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_COPYRIGHT_SIZE_Y	(30 * SCREEN_SCALE/2) // ����

#define COPYRIGHT_POS_X				(SCREEN_CENTER_X)	// �������WX
#define COPYRIGHT_POS_Y				(693 * SCREEN_SCALE)				// �������WY

#define TEXTURE_COPYRIGHT_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_COPYRIGHT_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_COPYRIGHT_B			(255) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_COPYRIGHT			(TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT*TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_COPYRIGHT				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define COPYRIGHT_MAX					(1)	// COPYRIGHT�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// COPYRIGHT�̈ړ���
	D3DXVECTOR3				rot;					// COPYRIGHT�̉�]��
	D3DXVECTOR2				size;					// COPYRIGHT�̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}COPYRIGHT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCopyright(int type);
void UninitCopyright(void);
void UpdateCopyright(void);
void DrawCopyright(void);
COPYRIGHT *GetCopyright(int no);


#endif
