//=============================================================================
//
// OUTLINE���� [outline.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _OUTLINE_H_
#define _OUTLINE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_OUTLINE		(2)	// �|���S����

#define TEXTURE_GAME_OUTLINE	("data/TEXTURE/outline.png")	// OUTLINE�p�摜
#define TEXTURE_OUTLINE_SIZE_X	(15 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_OUTLINE_SIZE_Y	(15 * SCREEN_SCALE / 2) // ����

#define TEXTURE_OUTLINE_POS_X	(SCREEN_WIDTH) // �e�N�X�`���T�C�Y
#define TEXTURE_OUTLINE_POS_Y	(SCREEN_HEIGHT) // ����

#define TEXTURE_OUTLINE_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_OUTLINE_G			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_OUTLINE_B			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_OUTLINE_A			(1.0f) // �e�N�X�`���J���[��

#define TEXTURE_OUTLINE_CHANGE_COLLAR		(0.005f) // �e�N�X�`���J���[�ύX���x

#define TEXTURE_PATTERN_DIVIDE_X_OUTLINE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_OUTLINE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_OUTLINE			(TEXTURE_PATTERN_DIVIDE_X_OUTLINE*TEXTURE_PATTERN_DIVIDE_Y_OUTLINE)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_OUTLINE			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define OUTLINE_MAX					(2)	// OUTLINE�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// OUTLINE�̈ړ���
	D3DXVECTOR3				rot;					// OUTLINE�̉�]��
	D3DXVECTOR2				size;					// OUTLINE�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				colorOutline;				// �F�Ǘ�
	int						nCollarFlag;			// �F�ύX�t���O

	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}OUTLINE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitOutline(int type);
void UninitOutline(void);
void UpdateOutline(void);
void DrawOutline(void);
OUTLINE *GetOutline(int no);
int SetOutline(D3DXCOLOR colorfloat);
void SetUpdateOutline(int nOutline, D3DXVECTOR3 pos, float fAngle);
void ReleaseOutline(int nOutline);
#endif
