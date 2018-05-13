//=============================================================================
//
// POINTER���� [pointer.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _POINTER_H_
#define _POINTER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_POINTER		(2)	// �|���S����

#define TEXTURE_GAME_POINTER		("data/TEXTURE/pointer.png")	// POINTER�p�摜
#define TEXTURE_GAME_OUTLINE		("data/TEXTURE/outline.png")	// OUTLINE�p�摜
#define TEXTURE_GAME_POINTER_MAX	(2)

#define TEXTURE_POINTER_SIZE_X	(13 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_POINTER_SIZE_Y	(13 * SCREEN_SCALE / 2) // ����

#define TEXTURE_POINTER_POS_X	(SCREEN_WIDTH) // �e�N�X�`���T�C�Y
#define TEXTURE_POINTER_POS_Y	(SCREEN_HEIGHT) // ����

#define TEXTURE_POINTER_R			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_POINTER_G			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_POINTER_B			(1.0f) // �e�N�X�`���J���[��
#define TEXTURE_POINTER_A			(1.0f) // �e�N�X�`���J���[��

#define TEXTURE_POINTER_CHANGE_COLLAR		(0.005f) // �e�N�X�`���J���[�ύX���x

#define TEXTURE_PATTERN_DIVIDE_X_POINTER	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_POINTER	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_POINTER			(TEXTURE_PATTERN_DIVIDE_X_POINTER*TEXTURE_PATTERN_DIVIDE_Y_POINTER)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_POINTER			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define POINTER_MAX					(30)	// POINTER�̍ő吔

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// POINTER�̈ړ���
	D3DXVECTOR3				rot;					// POINTER�̉�]��
	D3DXVECTOR2				size;					// POINTER�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				colorPointer;			// �F�Ǘ�

	int						nCollarFlag;			// �F�ύX�t���O
	int						nTex;					// �g�p�e�N�X�`��

	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}POINTER;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/

enum
{
	POINTER_TEXTURE_NORMAIL,
	POINTER_TEXTURE_OUTLINE
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPointer(int type);
void UninitPointer(void);
void UpdatePointer(void);
void DrawPointer(void);
POINTER *GetPointer(int no);
int SetPointer(D3DXCOLOR colorfloat);
void SetUpdatePointer(int nPointer, int nTex, D3DXVECTOR3 pos, float fAngle);
void ReleasePointer(int nPointer);
#endif
