//=============================================================================
//
// TIMERANKING���� [timeranking.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _TIMERANKING_H_
#define _TIMERANKING_H_

// �}�N����`
#define NUM_TIMERANKING				(2)	// �|���S����
#define TIMERANKING_NUM_FILE			("data/FILE/timeranking")

#define TEXTURE_GAME_TIMERANKING		("data/TEXTURE/number.png")	// �T���v���p�摜
#define TEXTURE_TIMERANKING_SIZE_X		(70 * SCREEN_SCALE)	// �e�N�X�`���T�C�Y
#define TEXTURE_TIMERANKING_SIZE_Y		(70 * SCREEN_SCALE)	// ����

#define TEXTURE_TIMERANKING_SPAN		(45 * SCREEN_SCALE)	// ����
#define TEXTURE_TIMERANKING_INTERVAL_X	(25 * SCREEN_SCALE)	// ����
#define TEXTURE_TIMERANKING_INTERVAL_Y	(110 * SCREEN_SCALE)	// ����

#define TIMERANKING_RESULT_POS_Y		(543 * SCREEN_SCALE)	// ����

#define TIMERANKING_SECOND_MAX			(9959)		// �ő�b��
#define TIMERANKING_SECOND				(60)		// 60�b
#define TIMERANKING_DIGIT				(4)			// �\������

#define TIMERANKING_POS_X				(1155 * SCREEN_SCALE)		// �|���S���̏����ʒuX
#define TIMERANKING_POS_Y				(105 * SCREEN_SCALE)		// ����
#define TIMERANKING_MAX					(5)			// �ő�b��

#define TIMERANKING_NUM1				(330)
#define TIMERANKING_NUM2				(400)
#define TIMERANKING_NUM3				(430)
#define TIMERANKING_NUM4				(500)
#define TIMERANKING_NUM5				(530)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	VERTEX_2D				vertexWkTimeranking[TIMERANKING_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				posTimeranking;						// �|���S���̈ړ���
	int						nTimeranking;						// �^�C��
}TIMERANKING;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimeranking(int type);
void UninitTimeranking(void);
void UpdateTimeranking(void);
void DrawTimeranking(void);
void SetTimeranking(int nNum, int nTimeranking);

#endif
