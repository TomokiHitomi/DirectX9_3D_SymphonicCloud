//=============================================================================
//
// GAGEFREAM���� [gagefream.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _GAGEFREAM_H_
#define _GAGEFREAM_H_

#include "main.h"

// �}�N����`
#define NUM_GAGEFREAM		(2)	// �|���S����

#define TEXTURE_GAME_GAGEFREAM00	("data/TEXTURE/gagefream00.png")	// GAGEFREAM�p�摜
#define TEXTURE_GAME_GAGEFREAM01	("data/TEXTURE/gagefream01.png")	// GAGEFREAM�p�摜
#define TEXTURE_GAME_GAGEFREAM02	("data/TEXTURE/gagefream02.png")	// GAGEFREAM�p�摜

// �}�W�b�N�t���[��
#define TEXTURE_GAGEFREAM_SIZE_X	(260 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEFREAM_SIZE_Y	(390 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEFREAM_POS_X	(121 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEFREAM_POS_Y	(172 * SCREEN_SCALE)	// ����

// �{�X�t���[��
#define TEXTURE_GAGEFREAM_BOSS_SIZE_X	(640 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEFREAM_BOSS_SIZE_Y	(100 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEFREAM_BOSS_POS_X	(SCREEN_CENTER_X)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEFREAM_BOSS_POS_Y	(36 * SCREEN_SCALE)	// ����

// �v���C���[�t���[��
#define TEXTURE_GAGEFREAM_MODEL_SIZE_X	(640 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEFREAM_MODEL_SIZE_Y	(100 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEFREAM_MODEL_POS_X	(SCREEN_CENTER_X)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEFREAM_MODEL_POS_Y	(662.5 * SCREEN_SCALE)	// ����

// �ʏ�o���b�g�ǉ�
#define TEXTURE_GAGEFREAM_BULLET_SIZE_X	(260 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGEFREAM_BULLET_SIZE_Y	(65 * SCREEN_SCALE / 2) // ����
#define TEXTURE_GAGEFREAM_BULLET_POS_X	(1159 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGEFREAM_BULLET_POS_Y	(308 * SCREEN_SCALE)	// ����


#define TEXTURE_GAGEFREAM_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_GAGEFREAM_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_GAGEFREAM_B			(255) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GAGEFREAM			(TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM*TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_GAGEFREAM			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define GAGEFREAM_MAX					(4)	// GAGEFREAM�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// GAGEFREAM�̈ړ���
	D3DXVECTOR3				rot;					// GAGEFREAM�̉�]��
	D3DXVECTOR2				size;					// GAGEFREAM�̃e�N�X�`���T�C�Y(x,y)
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
	bool					bPatternFlag;			// �e�N�X�`�����W�����t���O
}GAGEFREAM;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum
{
	GAGEFREAM_MAGIC,
	GAGEFREAM_DEFAULT,
	GAGEFREAM_BULLET,
	GAGEFREAM_TEXTURE_MAX
};
enum
{
	GAGEFREAM_TYPE_MAGIC,
	GAGEFREAM_TYPE_BOSS,
	GAGEFREAM_TYPE_MODEL,
	GAGEFREAM_TYPE_BULLET,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGagefream(int type);
void UninitGagefream(void);
void UpdateGagefream(void);
void DrawGagefream(void);
GAGEFREAM *GetGagefream(int no);
void SetGagefream(int nGage);
void ReleaseGagefream(void);

#endif
