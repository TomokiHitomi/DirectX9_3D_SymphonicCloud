//=============================================================================
//
// �p�����[�^���� [gage.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _GAGE_H_
#define _GAGE_H_

#include "main.h"

// �}�N����`
#define NUM_GAGE		(2)	// �|���S����

#define TEXTURE_GAME_GAGE00	_T("data/TEXTURE/gage.png")	// hp�p�摜
#define TEXTURE_GAGE00_SIZE_X	(50 * SCREEN_SCALE) // �e�N�X�`���T�C�Y
#define TEXTURE_GAGE00_SIZE_Y	(5 * SCREEN_SCALE/2) // ����

#define TEXTURE_CHANGE_GAGE_X		(1.0f)
#define TEXTURE_CHANGE_GAGE_Y		(1.0f)

#define TEXTURE_CHANGE_BOSS_GAGE_X	(15.0f)
#define TEXTURE_CHANGE_BOSS_GAGE_Y	(1.5f)

#define HP_GAGE_SIZE_X			(500 * SCREEN_SCALE)
#define HP_GAGE_SIZE_Y			(15 * SCREEN_SCALE)
#define HP_GAGE_POS_X			(SCREEN_CENTER_X - (HP_GAGE_SIZE_X / 2))
#define HP_GAGE_POS_Y			(645 * SCREEN_SCALE)

#define MP_GAGE_SIZE_X			(500 * SCREEN_SCALE)
#define MP_GAGE_SIZE_Y			(15 * SCREEN_SCALE)
#define MP_GAGE_POS_X			(SCREEN_CENTER_X - (MP_GAGE_SIZE_X / 2))
#define MP_GAGE_POS_Y			(680 * SCREEN_SCALE)

#define BULLET_GAGE_SIZE_X		(157 * SCREEN_SCALE)
#define BULLET_GAGE_SIZE_Y		(14.5f * SCREEN_SCALE)
#define BULLET_GAGE_POS_X		(1187 * SCREEN_SCALE + (BULLET_GAGE_SIZE_X / 2))
#define BULLET_GAGE_POS_Y		(308 * SCREEN_SCALE)

#define MAGIC_GAGE_SIZE_X		(157 * SCREEN_SCALE)
#define MAGIC_GAGE_SIZE_Y		(14.5f * SCREEN_SCALE)
#define MAGIC_GAGE_POS_X		(94 * SCREEN_SCALE - (MAGIC_GAGE_SIZE_X / 2))
#define MAGIC_GAGE_POS_Y		(35.5f * SCREEN_SCALE)
#define MAGIC_GAGE_INTERVAL		(54.55f * SCREEN_SCALE)

#define BOSS_GAGE_SIZE_X		(500 * SCREEN_SCALE)
#define BOSS_GAGE_SIZE_Y		(15 * SCREEN_SCALE)
#define BOSS_GAGE_POS_X			(SCREEN_CENTER_X - (BOSS_GAGE_SIZE_X / 2))
#define BOSS_GAGE_POS_Y			(36 * SCREEN_SCALE)

#define GAGE_ALPHA				(0.5f)		// �Q�[�W�̃��l�i�������������j

#define GAGE_STANDBY_SPEED		(0.008f)
#define GAGE_CHANGE_SPEED		(0.01f)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// �p�����[�^�̈ړ���
	D3DXVECTOR3				rot;					// �p�����[�^�̉�]��
	D3DXVECTOR2				size;					// �p�����[�^�̃e�N�X�`���T�C�Y(x,y)
	D3DXVECTOR3				vecTargetPos;			// �^�[�Q�b�g���W
	D3DXVECTOR2				vecSize;			// �e�N�X�`���T�C�Y�ύX(x,y)

	int						nCountAnim;				// �A�j���[�V�����J�E���g
	int						nPatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					fPointGauge;			// ���݂̃|�C���g
	int						nTarget;				// �^�[�Q�b�g
	int						nType;					// �p�����[�^�^�C�v
	float					fBaseAngle;				// �p�x�i�Ɓj
	float					fRadius;				// ���a

	bool					bUse;					// true:�g�p  false:���g�p
	bool					bStandby;				// �Q�[�W�[�U�t���O
}GAGE;

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/

enum
{
	GAGE_PLAYER_HP,					// �v���C���[��HP
	GAGE_PLAYER_MP,					// �v���C���[��MP
	GAGE_BULLET_NORMAL,				// �m�[�}���o���b�g
	GAGE_BULLET_SPECIAL_FIRE,		// �t�@�C�A
	GAGE_BULLET_SPECIAL_THUNDER,	// �T���_�[
	GAGE_BULLET_SPECIAL_ICE,		// �A�C�X
	GAGE_BULLET_SPECIAL_WIND,		// �T���_�[
	GAGE_BULLET_SPECIAL_WATER,		// �E�H�[�^�[
	GAGE_BULLET_SPECIAL_EARTH,		// �A�[�X
	GAGE_BOSS_HP,					// �{�X��HP
	GAGE_MAX						// �Q�[�W�̍ő吔
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGage(int type);
void UninitGage(void);
void UpdateGage(void);
void DrawGage(void);
void SetGage(int nGage, int nBoss);
void InitStatusGage(int nGage);
GAGE *GetGage(int no);


#endif
