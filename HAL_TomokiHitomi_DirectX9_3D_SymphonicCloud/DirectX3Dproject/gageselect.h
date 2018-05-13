//=============================================================================
//
// GAGESELECT���� [gageselect.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _GAGESELECT_H_
#define _GAGESELECT_H_

#include "main.h"

// �}�N����`
#define NUM_GAGESELECT		(2)	// �|���S����

#define TEXTURE_GAME_GAGESELECT	("data/TEXTURE/gageselect.png")	// GAGESELECT�p�摜
#define TEXTURE_GAGESELECT_SIZE_X	(260 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGESELECT_SIZE_Y	(65 * SCREEN_SCALE / 2) // ����

#define TEXTURE_GAGESELECT_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_GAGESELECT_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_GAGESELECT_B			(255) // �e�N�X�`���J���[��

// �}�W�b�N�o���b�gPOS
#define TEXTURE_GAGESELECT_MAGIC_POS_X	(121 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGESELECT_MAGIC_POS_Y	(35.5f * SCREEN_SCALE)	// ����
#define TEXTURE_GAGESELECT_MAGIC_INTERVAL	(54.55f * SCREEN_SCALE)

// �ʏ�o���b�gPOS
#define TEXTURE_GAGESELECT_BULLET_POS_X	(1159 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_GAGESELECT_BULLET_POS_Y	(308 * SCREEN_SCALE)	// ����

#define TEXTURE_GAGESELECT_CHANGE_COLLAR	(0.05f)
#define TEXTURE_GAGESELECT_MOVESPEED		(0.1f)

#define TEXTURE_PATTERN_DIVIDE_X_GAGESELECT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GAGESELECT			(TEXTURE_PATTERN_DIVIDE_X_GAGESELECT*TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_GAGESELECT			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define GAGESELECT_MAX					(2)	// GAGESELECT�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// GAGESELECT�̈ړ���
	D3DXVECTOR3				rot;					// GAGESELECT�̉�]��
	D3DXVECTOR2				size;					// GAGESELECT�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				xColor;

	int						nCollarFlag;
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}GAGESELECT;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum 
{
	GAGESELECT_MAGIC,
	GAGESELECT_BULLET
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGageselect(int type);
void UninitGageselect(void);
void UpdateGageselect(void);
void DrawGageselect(void);
GAGESELECT *GetGageselect(int no);
void SetGageselect(void);
void ReleaseGageselect(void);

#endif
