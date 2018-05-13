//=============================================================================
//
// �^�C�g������ [pause.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PAUSE		(2)	// �|���S����

#define TEXTURE_GAME_PAUSE00	_T("data/TEXTURE/pause.png")	// �^�C�g��00�p�摜
#define TEXTURE_PAUSE00_SIZE_X	(450 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_PAUSE00_SIZE_Y	(250 * SCREEN_SCALE/2) // ����
#define PAUSE_POS_X			(SCREEN_CENTER_X)	// �������WX
#define PAUSE_POS_Y			(100 * SCREEN_SCALE)				// �������WY

#define TEXTURE_PATTERN_DIVIDE_X_PAUSE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_PAUSE	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_PAUSE			(TEXTURE_PATTERN_DIVIDE_X_PAUSE*TEXTURE_PATTERN_DIVIDE_Y_PAUSE)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_PAUSE				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define PAUSE_MAX					(1)	// �^�C�g���̍ő吔

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// �^�C�g���̈ړ���
	D3DXVECTOR3				rot;					// �^�C�g���̉�]��
	D3DXVECTOR2				size;					// �^�C�g���̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a

	bool					bUse;					// true:�g�p  false:���g�p
	bool					bStop;					// �ꎞ��~
}PAUSE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPause(int type);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
bool GetPauseFlag(void);
PAUSE *GetPause(int no);


#endif
