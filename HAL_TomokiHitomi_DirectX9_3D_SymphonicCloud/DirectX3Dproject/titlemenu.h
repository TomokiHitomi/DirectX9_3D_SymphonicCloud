//=============================================================================
//
// �^�C�g������ [titlemenu.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_TITLEMENU		(2)	// �|���S����

#define TEXTURE_GAME_TITLEMENU	("data/TEXTURE/titlemenu.png")	// �^�C�g��00�p�摜
#define TEXTURE_TITLEMENU00_SIZE_X	(300 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_TITLEMENU00_SIZE_Y	(100 * SCREEN_SCALE/2) // ����
#define TITLEMENU_POS_X			(SCREEN_CENTER_X)	// �������WX
#define TITLEMENU_POS_Y			(320 * SCREEN_SCALE)				// �������WY
#define TITLEMENU_SPACING		(70 * SCREEN_SCALE)

#define TEXTURE_PATTERN_DIVIDE_X_TITLEMENU	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_TITLEMENU			(TEXTURE_PATTERN_DIVIDE_X_TITLEMENU*TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_TITLEMENU				(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define TITLEMENU_MAX					(5)	// �^�C�g���̍ő吔
#define TITLEMENU_SELECT_PRESS			(40)	// ���j���[��������J�n�^�C��
#define TITLEMENU_SELECT_SPEED			(7)		// ���j���|�������葬�x

#define TITLEMENU_SYS_MAX				(1)	// �^�C�g���V�X�e���̍ő吔

enum
{
	TITLE_MENU_START,
	TITLE_MENU_TUTORIAL,
	TITLE_MENU_CREDIT,
	TITLE_MENU_OPTION,
	TITLE_MENU_EXIT
};

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
	float					fRadius;					// ���a
	bool					bUse;					// true:�g�p  false:���g�p
}TITLEMENU;

typedef struct
{
	bool					bSelect;
}TITLEMENU_SYS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitlemenu(int type);
void UninitTitlemenu(void);
void UpdateTitlemenu(void);
void DrawTitlemenu(void);
void SetTitlemenu(void);
void ReleaseTitlemenu(void);
TITLEMENU *GetTitlemenu(int no);


#endif
