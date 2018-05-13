//=============================================================================
//
// HITEFFECT���� [hiteffect.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HITEFFECT	"data/TEXTURE/hiteffect.png"
#define HITEFFECT_MAX			(100)		// �ő吔
#define	HITEFFECT_SIZE_X		(45.0f)		// �T�C�YX
#define	HITEFFECT_SIZE_Y		(45.0f)		// �T�C�YY
#define	HITEFFECT_ALPHA			(30)		// ���e�X�g�l

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_HITEFFECT	(8)
#define TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT	(7)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_HITEFFECT			(TEXTURE_PATTERN_DIVIDE_X_HITEFFECT*TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT)
// �A�j���[�V����
#define TIME_ANIMATION_HITEFFECT			(4)

typedef struct
{
	D3DXVECTOR3			posHiteffect;			// �ʒu
	D3DXVECTOR3			rotHiteffect;			// ����(��])
	D3DXVECTOR3			sclHiteffect;			// �傫��(�X�P�[��)
	D3DXVECTOR2			vec2Size;				// �T�C�Y
	int					nTex;					// �e�N�X�`���p�^�[��
	int					nAnimeCount;			// �A�j���[�V�����J�E���g
	int					nAnimePattern;			// �A�j���[�V�����p�^�[��
	bool				bUse;					// �g�p�t���O
}HITEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHiteffect(int nType);
void UninitHiteffect(void);
void UpdateHiteffect(void);
void DrawHiteffect(void);
void SetHiteffect(D3DXVECTOR3 pos, int nTex);
HITEFFECT *GetHiteffect(int no);

#endif
