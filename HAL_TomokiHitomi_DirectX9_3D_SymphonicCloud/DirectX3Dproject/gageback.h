//=============================================================================
//
// GAGEBACK���� [gageback.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _GAGEBACK_H_
#define _GAGEBACK_H_

#include "main.h"

// �}�N����`
#define NUM_GAGEBACK		(2)	// �|���S����

#define TEXTURE_GAME_GAGEBACK00	("data/TEXTURE/gageback00.png")	// GAGEBACK�p�摜
#define TEXTURE_GAME_GAGEBACK01	("data/TEXTURE/gageback01.png")	// GAGEBACK�p�摜
#define TEXTURE_GAME_GAGEBACK02	("data/TEXTURE/gageback02.png")	// GAGEBACK�p�摜

// �}�W�b�N�t���[��
#define TEXTURE_GAGEBACK_SIZE_X	(260 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEBACK_SIZE_Y	(390 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEBACK_POS_X	(121 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEBACK_POS_Y	(172 * SCREEN_SCALE)	// ����

// �{�X�t���[��
#define TEXTURE_GAGEBACK_BOSS_SIZE_X	(640 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEBACK_BOSS_SIZE_Y	(100 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEBACK_BOSS_POS_X	(SCREEN_CENTER_X)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEBACK_BOSS_POS_Y	(36 * SCREEN_SCALE)	// ����

// �v���C���[�t���[��
#define TEXTURE_GAGEBACK_MODEL_SIZE_X	(640 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEBACK_MODEL_SIZE_Y	(100 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEBACK_MODEL_POS_X	(SCREEN_CENTER_X)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEBACK_MODEL_POS_Y	(662.5 * SCREEN_SCALE)	// ����

// �ʏ�o���b�g�ǉ�
#define TEXTURE_GAGEBACK_BULLET_SIZE_X	(260 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEBACK_BULLET_SIZE_Y	(65 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEBACK_BULLET_POS_X	(1159 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEBACK_BULLET_POS_Y	(308 * SCREEN_SCALE)	// ����


#define TEXTURE_GAGEBACK_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_GAGEBACK_G			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_GAGEBACK_B			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_GAGEBACK_A			(0.4f) // �e�N�X�`�����l

#define TEXTURE_PATTERN_DIVIDE_X_GAGEBACK	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GAGEBACK			(TEXTURE_PATTERN_DIVIDE_X_GAGEBACK*TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_GAGEBACK			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define GAGEBACK_MAX					(4)	// GAGEBACK�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// GAGEBACK�̈ړ���
	D3DXVECTOR3				rot;					// GAGEBACK�̉�]��
	D3DXVECTOR2				size;					// GAGEBACK�̃e�N�X�`���T�C�Y(x,y)
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
	bool					bPatternFlag;			// �e�N�X�`�����W�����t���O
}GAGEBACK;

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
enum
{
	GAGEBACK_MAGIC,
	GAGEBACK_DEFAULT,
	GAGEBACK_BULLET,
	GAGEBACK_TEXTURE_MAX
};

enum
{
	GAGEBACK_TYPE_MAGIC,
	GAGEBACK_TYPE_BOSS,
	GAGEBACK_TYPE_MODEL,
	GAGEBACK_TYPE_BULLET,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGageback(int type);
void UninitGageback(void);
void UpdateGageback(void);
void DrawGageback(void);
GAGEBACK *GetGageback(int no);
void SetGageback(int nGage);
void ReleaseGageback(void);

#endif
