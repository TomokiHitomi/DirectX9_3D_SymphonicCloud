//=============================================================================
//
// MAGICEFFECT���� [magiceffect.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _MAGICEFFECT_H_
#define _MAGICEFFECT_H_

#include "main.h"

// �}�N����`
#define NUM_MAGICEFFECT		(2)	// �|���S����

#define TEXTURE_GAME_MAGICEFFECT	("data/TEXTURE/magiceffect.png")	// MAGICEFFECT�p�摜
#define TEXTURE_MAGICEFFECT_SIZE_X	(40 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_MAGICEFFECT_SIZE_Y	(40 * SCREEN_SCALE / 2) // ����

#define MAGICEFFECT_POS_X			(205 * SCREEN_SCALE)
#define MAGICEFFECT_POS_Y			(35.5f * SCREEN_SCALE)
#define MAGIC_GAGE_INTERVAL			(54.55f * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_L_X			(132 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_L_Y			(585 * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_R_X			(1151 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_R_Y			(585 * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_SIZE_X		(60 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_SIZE_Y		(60 * SCREEN_SCALE)

#define MAGICEFFECT_NORMAL_POS_X	(1074 * SCREEN_SCALE)
#define TEXTURE_MAGICEFFECT_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_MAGICEFFECT_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_MAGICEFFECT_B			(255) // �e�N�X�`���J���[��

#define TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT	(8)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT	(7)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_MAGICEFFECT			(TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT*TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_MAGICEFFECT				(8)	// �A�j���[�V�����̐؂�ւ��J�E���g
#define TEXTURE_PATTERN_NONACTIVE_MAGICEFFECT	(4)	// �������ɒ�~����t���[��


#define MAGICEFFECT_MAX					(7)	// MAGICEFFECT�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// MAGICEFFECT�̈ړ���
	D3DXVECTOR3				rot;					// MAGICEFFECT�̉�]��
	D3DXVECTOR2				size;					// MAGICEFFECT�̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						nType;					// �A�j���[�V�����^�C�v
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}MAGICEFFECT;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum
{
	MAGICEFFECT_FIRE,
	MAGICEFFECT_THUNDER,
	MAGICEFFECT_ICE,
	MAGICEFFECT_WIND,
	MAGICEFFECT_WATER,
	MAGICEFFECT_EARTH,
	MAGICEFFECT_NORMAL,
	MAGICEFFECT_MAGIC_L,
	MAGICEFFECT_MAGIC_R,
	MAGICEFFECT_TYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMagiceffect(int type);
void UninitMagiceffect(void);
void UpdateMagiceffect(void);
void DrawMagiceffect(void);
MAGICEFFECT *GetMagiceffect(int no);
void SetMagiceffect(int nMagicEffect);
void ReleaseMagiceffect(void);

#endif
