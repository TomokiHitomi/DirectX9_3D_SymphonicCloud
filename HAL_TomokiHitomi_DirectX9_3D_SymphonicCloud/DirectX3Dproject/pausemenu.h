//=============================================================================
//
// �^�C�g������ [pausemenu.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PAUSEMENU		(2)	// �|���S����

#define TEXTURE_GAME_PAUSEMENU00	_T("data/TEXTURE/pausemenu.png")	// �^�C�g��00�p�摜
#define TEXTURE_PAUSEMENU00_SIZE_X	(300 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_PAUSEMENU00_SIZE_Y	(100 * SCREEN_SCALE/2) // ����

#define PAUSEMENU_POS_X			(SCREEN_CENTER_X)	// �������WX
#define PAUSEMENU_POS_Y			(285 * SCREEN_SCALE)				// �������WY
#define PAUSEMENU_SPACING		(80 * SCREEN_SCALE)

#define TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU	(3)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_PAUSEMENU			(TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU*TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_PAUSEMENU				(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define PAUSEMENU_MAX					(3)	// �^�C�g���̍ő吔
#define PAUSEMENU_SELECT_PRESS			(40)	// �|�[�Y���j���[��������J�n�^�C��
#define PAUSEMENU_SELECT_SPEED			(7)		// �|�[�Y���j���|�������葬�x

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
}PAUSEMENU;

typedef struct
{
	bool					bSelect;
}PAUSEMENU_SYS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPausemenu(int type);
void UninitPausemenu(void);
void UpdatePausemenu(void);
void DrawPausemenu(void);
void SetPausemenu(bool bUse);
PAUSEMENU *GetPausemenu(int no);


#endif
