//=============================================================================
//
// JOYCONHELP���� [joyconhelp.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _JOYCONHELP_H_
#define _JOYCONHELP_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_JOYCONHELP		(2)	// �|���S����

#define TEXTURE_GAME_JOYCONHELP	("data/TEXTURE/joycon.png")	// JOYCONHELP�p�摜
#define TEXTURE_JOYCONHELP_SIZE_X	(1280 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_JOYCONHELP_SIZE_Y	(432 * SCREEN_SCALE/2) // ����

#define JOYCONHELP_POS_X				(SCREEN_CENTER_X)			// �������WX
#define JOYCONHELP_POS_Y				(505 * SCREEN_SCALE)		// �������WY

#define TEXTURE_JOYCONHELP_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_JOYCONHELP_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_JOYCONHELP_B			(255) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_JOYCONHELP			(TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP*TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_JOYCONHELP				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define JOYCONHELP_MAX					(1)	// JOYCONHELP�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// JOYCONHELP�̈ړ���
	D3DXVECTOR3				rot;					// JOYCONHELP�̉�]��
	D3DXVECTOR2				size;					// JOYCONHELP�̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}JOYCONHELP;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitJoyconhelp(int type);
void UninitJoyconhelp(void);
void UpdateJoyconhelp(void);
void DrawJoyconhelp(void);
JOYCONHELP *GetJoyconhelp(int no);


#endif
