//=============================================================================
//
// DAMAGEEFFECT���� [damageeffect.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _DAMAGEEFFECT_H_
#define _DAMAGEEFFECT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_DAMAGEEFFECT		(2)	// �|���S����

#define TEXTURE_GAME_DAMAGEEFFECT	("data/TEXTURE/damageeffect.jpg")	// DAMAGEEFFECT�p�摜
#define TEXTURE_DAMAGEEFFECT_SIZE_X	(1280 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_DAMAGEEFFECT_SIZE_Y	(720 * SCREEN_SCALE/2) // ����

#define DAMAGEEFFECT_POS_X				(SCREEN_CENTER_X)			// �������WX
#define DAMAGEEFFECT_POS_Y				(SCREEN_CENTER_Y)			// �������WY

#define TEXTURE_DAMAGEEFFECT_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_DAMAGEEFFECT_G			(0.0f) // �e�N�X�`���J���[��
#define TEXTURE_DAMAGEEFFECT_B			(0.0f) // �e�N�X�`���J���[��
#define TEXTURE_DAMAGEEFFECT_A			(0.0f) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_DAMAGEEFFECT			(TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT*TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_DAMAGEEFFECT				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define DAMAGEEFFECT_MAX					(1)	// DAMAGEEFFECT�̍ő吔

#define DAMAGEEFFECT_ALPHA_UP				(0.1f)	// ���l�̏㏸���x
#define DAMAGEEFFECT_ALPHA_DOWN				(0.01f)	// ���l�̉��~���x

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// DAMAGEEFFECT�̈ړ���
	D3DXVECTOR3				rot;					// DAMAGEEFFECT�̉�]��
	D3DXVECTOR2				size;					// DAMAGEEFFECT�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				xColor;				// �F�Ǘ�
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
	bool					bActive;				// �L���t���O
}DAMAGEEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDamageeffect(int type);
void UninitDamageeffect(void);
void UpdateDamageeffect(void);
void DrawDamageeffect(void);
DAMAGEEFFECT *GetDamageeffect(int no);
void SetDamageeffect(void);

#endif
