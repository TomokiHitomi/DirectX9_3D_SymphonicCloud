//=============================================================================
//
// MINIMAP���� [minimap.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_MINIMAP		(2)	// �|���S����

#define TEXTURE_GAME_MINIMAP00	("data/TEXTURE/minimap00.png")	// MINIMAP�p�摜
#define TEXTURE_GAME_MINIMAP01	("data/TEXTURE/minimap01.png")	// MINIMAP�p�摜

#define TEXTURE_MINIMAP_SIZE_X	(250 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_MINIMAP_SIZE_Y	(250 * SCREEN_SCALE / 2) // ����

#define TEXTURE_MINIMAP_POS_X	(1150 * SCREEN_SCALE) // �e�N�X�`���|�W�V����
#define TEXTURE_MINIMAP_POS_Y	(130 * SCREEN_SCALE) // ����

#define TEXTURE_MINIMAP_CHANGE_COLLAR		(0.005f) // �e�N�X�`���J���[�ύX���x

#define TEXTURE_PATTERN_DIVIDE_X_MINIMAP	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_MINIMAP	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_MINIMAP			(TEXTURE_PATTERN_DIVIDE_X_MINIMAP*TEXTURE_PATTERN_DIVIDE_Y_MINIMAP)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_MINIMAP			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define MINIMAP_MAX					(2)		// MINIMAP�̍ő吔
#define MINIMAP_SYS_MAX				(30)	// MINIMAP_SYS�̍ő吔
#define MINIMAP_POS_CONVERT			(10 / SCREEN_SCALE)
#define MINIMAP_POS_LENGTH_MAX		(TEXTURE_MINIMAP_SIZE_X * 0.8f)

#define MINIMAP_ALPHA			(0.1f)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// MINIMAP�̈ړ���
	D3DXVECTOR3				rot;					// MINIMAP�̉�]��
	D3DXVECTOR2				size;					// MINIMAP�̃e�N�X�`���T�C�Y(x,y)
	D3DXCOLOR				colorMinimap;				// �F�Ǘ�
	int						nCollarFlag;			// �F�ύX�t���O

	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}MINIMAP;

typedef struct
{
	int						nType;
	int						nIndexTag;
	int						nIndexPointer;
	bool					bUse;					// true:�g�p  false:���g�p
}MINIMAP_SYS;

//*****************************************************************************
// �񋓌^��`
//*****************************************************************************
enum
{
	MINIMAP_BACK,
	MINIMAP_MAIN
};

enum
{
	MINIMAP_MODEL,
	MINIMAP_ENEMY
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMinimap(int type);
void UninitMinimap(void);
void UpdateMinimap(void);
void DrawMinimap(void);
MINIMAP *GetMinimap(int no);
void SetMinimap(void);
void ReleaseMinimap(void);

#endif
