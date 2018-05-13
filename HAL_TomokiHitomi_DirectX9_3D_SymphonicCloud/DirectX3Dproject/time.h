//=============================================================================
//
// TIME���� [time.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

// �}�N����`
#define NUM_TIME				(2)	// �|���S����

#define TEXTURE_GAME_TIME		("data/TEXTURE/number.png")	// �T���v���p�摜
#define TEXTURE_TIME_SIZE_X		(70 * SCREEN_SCALE)	// �e�N�X�`���T�C�Y
#define TEXTURE_TIME_SIZE_Y		(70 * SCREEN_SCALE)	// ����

#define TEXTURE_TIME_SPAN		(45 * SCREEN_SCALE)	// ����
#define TEXTURE_TIME_INTERVAL	(25 * SCREEN_SCALE)	// ����

#define TIME_RESULT_POS_Y		(543 * SCREEN_SCALE)	// ����

#define TIME_MAX				(9959)		// �ő�b��
#define TIME_SECOND				(60)		// 60�b
#define TIME_DIGIT				(4)			// �\������

#define TIME_POS_X				(686 * SCREEN_SCALE)		// �|���S���̏����ʒuX
#define TIME_POS_Y				(75 * SCREEN_SCALE)		// ����


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTime(int type);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int add);

#endif
