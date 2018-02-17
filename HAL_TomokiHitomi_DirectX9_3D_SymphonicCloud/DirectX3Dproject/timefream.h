//=============================================================================
//
// TIMEFREAM���� [timefream.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _TIMEFREAM_H_
#define _TIMEFREAM_H_

#include "main.h"

// �}�N����`
#define NUM_TIMEFREAM		(2)	// �|���S����

#define TEXTURE_GAME_TIMEFREAMBACK	("data/TEXTURE/timeback.png")	// TIMEFREAM�p�摜
#define TEXTURE_GAME_TIMEFREAMFREAM	("data/TEXTURE/timefream.png")	// TIMEFREAM�p�摜

#define TEXTURE_TIMEFREAM_SIZE_X	(253 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_TIMEFREAM_SIZE_Y	(100 * SCREEN_SCALE / 2) // ����
#define TEXTURE_TIMEFREAM_POS_X	(SCREEN_CENTER_X)	// �e�N�X�`���|�W�V����
#define TEXTURE_TIMEFREAM_POS_Y	(112 * SCREEN_SCALE)	// ����

#define TEXTURE_TIMEFREAM_RESULT_POS_Y	(580 * SCREEN_SCALE)	// ����

#define TEXTURE_TIMEFREAM_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_TIMEFREAM_G			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_TIMEFREAM_B			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_TIMEFREAM_A			(0.4f) // �e�N�X�`�����l

#define TEXTURE_TIMEFREAM_CHANGE_COLLAR		(0.005f)

#define TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_TIMEFREAM			(TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM*TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_TIMEFREAM			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define TIMEFREAM_MAX					(2)	// TIMEFREAM�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXCOLOR				xColor;					// �J���[
	D3DXVECTOR3				pos;					// TIMEFREAM�̈ړ���
	D3DXVECTOR3				rot;					// TIMEFREAM�̉�]��
	D3DXVECTOR2				size;					// TIMEFREAM�̃e�N�X�`���T�C�Y(x,y)
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						nCollarFlag;			// �J���[�ڍs�t���O
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
	bool					bPatternFlag;			// �e�N�X�`�����W�����t���O
}TIMEFREAM;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum
{
	TIMEFREAM_BACK,
	TIMEFREAM_FREAM,
	TIMEFREAM_TEXTURE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimefream(int type);
void UninitTimefream(void);
void UpdateTimefream(void);
void DrawTimefream(void);
TIMEFREAM *GetTimefream(int no);
void SetTimefream(int nGage);
void ReleaseTimefream(void);

#endif
