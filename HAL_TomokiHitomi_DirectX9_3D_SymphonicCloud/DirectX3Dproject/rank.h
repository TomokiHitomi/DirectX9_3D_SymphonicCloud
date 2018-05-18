//=============================================================================
//
// �^�C�g������ [rank.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

#include "main.h"

// �}�N����`
#define NUM_RANK		(2)	// �|���S����

#define TEXTURE_GAME_RANK00	_T("data/TEXTURE/rank.png")	// �^�C�g��00�p�摜
#define TEXTURE_RANK00_SIZE_X	(150 * SCREEN_SCALE/2)	// �e�N�X�`���T�C�Y
#define TEXTURE_RANK00_SIZE_Y	(200 * SCREEN_SCALE/2)	// ����
#define RANK_POS_X				(SCREEN_CENTER_X)	// �������WX
#define RANK_POS_Y				(SCREEN_CENTER_Y)	// �������WY

#define TEXTURE_PATTERN_DIVIDE_X_RANK	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_RANK	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_RANK			(TEXTURE_PATTERN_DIVIDE_X_RANK*TEXTURE_PATTERN_DIVIDE_Y_RANK)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_RANK				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define RANK_MAX					(1)			// �g�[�^���X�R�A�{�[�h�̍ő吔

#define RANK_S						(230)		// S�����N�\���l
#define RANK_A						(300)		// A�����N�\���l
#define RANK_B						(400)		// B�����N�\���l
#define RANK_C						(500)		// C�����N�\���l

#define TEXTURE_RANK_CHANGE_COLLAR		(0.01f)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXCOLOR				xColor;					// �J���[
	D3DXVECTOR3				pos;					// �^�C�g���̈ړ���
	D3DXVECTOR3				rot;					// �^�C�g���̉�]��
	D3DXVECTOR2				size;					// �^�C�g���̃e�N�X�`���T�C�Y(x,y)
	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						nCollarFlag;			// �J���[�`�F���W�t���O
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p

}RANK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRank(int type);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);
D3DXCOLOR GetRankColor(void);
RANK *GetRank(int no);


#endif
