//=============================================================================
//
// RANKING���� [ranking.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

// �}�N����`
#define NUM_RANKING		(2)	// �|���S����

#define TEXTURE_GAME_RANKINGBACK	("data/TEXTURE/rankingback.png")	// RANKING�p�摜
#define TEXTURE_GAME_RANKINGFREAM	("data/TEXTURE/rankingfream.png")	// RANKING�p�摜

#define TEXTURE_RANKING_SIZE_X	(425 * SCREEN_SCALE / 2) // �e�N�X�`���T�C�Y
#define TEXTURE_RANKING_SIZE_Y	(587 * SCREEN_SCALE / 2) // ����
#define TEXTURE_RANKING_POS_X	(1035 * SCREEN_SCALE)	// �e�N�X�`���|�W�V����
#define TEXTURE_RANKING_POS_Y	(SCREEN_CENTER_Y)	// ����

#define TEXTURE_RANKING_R			(255) // �e�N�X�`���J���[��
#define TEXTURE_RANKING_G			(255) // �e�N�X�`���J���[��
#define TEXTURE_RANKING_B			(255) // �e�N�X�`���J���[��
#define TEXTURE_RANKING_A			(0.4f) // �e�N�X�`�����l

#define TEXTURE_RANKING_CHANGE_COLLAR		(0.01f)

#define TEXTURE_PATTERN_DIVIDE_X_RANKING	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_RANKING	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_RANKING			(TEXTURE_PATTERN_DIVIDE_X_RANKING*TEXTURE_PATTERN_DIVIDE_Y_RANKING)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_RANKING			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define RANKING_MAX					(2)	// RANKING�̍ő吔


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXCOLOR				xColor;					// �J���[
	D3DXVECTOR3				pos;					// RANKING�̈ړ���
	D3DXVECTOR3				rot;					// RANKING�̉�]��
	D3DXVECTOR2				size;					// RANKING�̃e�N�X�`���T�C�Y(x,y)
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						nCollarFlag;			// �J���[�ڍs�t���O
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a
	bool					bUse;					// true:�g�p  false:���g�p
	bool					bPatternFlag;			// �e�N�X�`�����W�����t���O
}RANKING;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum
{
	RANKING_BACK,
	RANKING_FREAM,
	RANKING_TEXTURE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRanking(int type);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
D3DXCOLOR GetRankingColor(void);
RANKING *GetRanking(int no);
void SetRanking(int nGage);
void ReleaseRanking(void);

#endif
