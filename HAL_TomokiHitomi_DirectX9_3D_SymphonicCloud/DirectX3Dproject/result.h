//=============================================================================
//
// ���U���g���� [result.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_RESULT		(2)	// �|���S����

#define TEXTURE_GAME_RESULT00	("data/TEXTURE/result.png")	// ���U���g00�p�摜
#define TEXTURE_RESULT00_SIZE_X	(350 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_RESULT00_SIZE_Y	(250 * SCREEN_SCALE/2) // ����
#define RESULT_POS_X			(SCREEN_CENTER_X)	// �������WX
#define RESULT_POS_Y			(100 * SCREEN_SCALE)	// �������WY

#define TEXTURE_PATTERN_DIVIDE_X_RESULT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_RESULT	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_RESULT			(TEXTURE_PATTERN_DIVIDE_X_RESULT*TEXTURE_PATTERN_DIVIDE_Y_RESULT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_RESULT				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define RESULT_MAX					(1)		// ���U���g�̍ő吔
#define RESULT_MOVE_UI				(250 * SCREEN_SCALE)	// ���U���g���̑�UI�ړ�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// ���U���g�̈ړ���
	D3DXVECTOR3				rot;					// ���U���g�̉�]��
	D3DXVECTOR3				vecPosMove;				// ���U���g����UI�ړ���
	D3DXVECTOR2				size;					// ���U���g�̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bPosMoveFlag;			// ��UI�̈ړ��t���O
	bool					bUse;					// true:�g�p  false:���g�p
}RESULT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(int nType);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT *GetResult(int no);


#endif
