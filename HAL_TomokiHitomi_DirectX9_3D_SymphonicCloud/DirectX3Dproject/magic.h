//=============================================================================
//
// MAGIC���� [magic.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_MAGIC		(2)	// �|���S����

#define TEXTURE_GAME_MAGIC	("data/TEXTURE/magic.png")	// MAGIC�p�摜
#define TEXTURE_MAGIC_SIZE_X	(500 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_MAGIC_SIZE_Y	(500 * SCREEN_SCALE / 2) // ����

#define TEXTURE_MAGIC_POS_X	(0.0f) // �e�N�X�`���T�C�Y
#define TEXTURE_MAGIC_POS_Y	(SCREEN_HEIGHT) // ����

#define TEXTURE_MAGIC_DUMMY_POS_X	(SCREEN_WIDTH) // �e�N�X�`���T�C�Y
#define TEXTURE_MAGIC_DUMMY_POS_Y	(SCREEN_HEIGHT) // ����

#define TEXTURE_MAGIC_R			(0.5f) // �e�N�X�`���J���[��
#define TEXTURE_MAGIC_G			(0.5f) // �e�N�X�`���J���[��
#define TEXTURE_MAGIC_B			(0.5f) // �e�N�X�`���J���[��
#define TEXTURE_MAGIC_A			(1.0f) // �e�N�X�`���J���[��

#define TEXTURE_MAGIC_CHANGE_COLLAR		(0.01f) // �e�N�X�`���J���[�ύX���x

#define TEXTURE_PATTERN_DIVIDE_X_MAGIC	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_MAGIC	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_MAGIC			(TEXTURE_PATTERN_DIVIDE_X_MAGIC*TEXTURE_PATTERN_DIVIDE_Y_MAGIC)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_MAGIC			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define MAGIC_MAX					(2)		// MAGIC�̍ő吔
#define MAGIC_TYPE_MAX				(6)		// �X�L���̍ő吔
#define MAGIC_ROT_SPEED				(0.01f)	// ��]�X�s�[�h
#define MAGIC_ROT_INERTIA			(0.1f)	// ��]����
#define MAGIC_ROT_COOLDOWN			(10)	// ��]�N�[���_�E���J�E���g

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// MAGIC�̈ړ���
	D3DXVECTOR3				rot;					// MAGIC�̉�]��
	D3DXVECTOR2				size;					// MAGIC�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				colorMagic;				// �F�Ǘ�
	int						nColorFlag;				// �F�ύX�t���O
	int						nCoolDown;				// ��]�N�[���_�E��
	int						nType;					// �^�C�v

	float					fGoalAngle;
	float					fTypeAngle;				// �����p�x
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p

	bool					bActive;				// �}�W�b�N�A�N�e�B�u�t���O
	bool					bActiveColor;			// �J���[�A�N�e�B�u
	bool					bActiveFire;
	bool					bActiveThunder;
	bool					bActiveIce;
	bool					bActiveWind;
	bool					bActiveWater;
	bool					bActiveEarth;
	bool					bSelectCheck;
}MAGIC;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum 
{
	MAGIC_MAIN,
	MAGIC_DUMMY,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMagic(int type);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);
void SetMagicType(int nMagicType);
void SetMagic(void);
void ReleaseMagic(void);
MAGIC *GetMagic(int no);
D3DXCOLOR GetMagicColor(void);
int GetTypeMagic(void);

#endif
