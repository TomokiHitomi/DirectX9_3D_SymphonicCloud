//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET	"data/TEXTURE/bullet.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define SHADER_BULLET	"sprite.fx"

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(1)

// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_BULLET		(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)
#define TEXTURE_PATTERN_BULLET			(TEXTURE_PATTERN_NU_BULLET / 5)

// �A�j���[�V����
#define TIME_ANIMATION_BULLET	(4)

#define	BULLET_ALPHA					(10)		// ���e�X�g�l
#define	BULLET_ALPHA_CHANGE				(0.01f)		// ���l�ύX���x
#define	BULLET_SIZE_X					(10.0f)		// �T�C�Y
#define	BULLET_SIZE_Y					(10.0f)		// ���ʂɃT�C�Y�ύX����ꍇ�̓X�P�[�����g��
#define BULLET_MAX						(3000)		// �ő吔
#define BULLET_FALSE_COUNT				(300)		// ���ŃJ�E���g
#define BULLET_FALSE_HEIGHT				(1000.0f)	// ���ō��x
#define	BULLET_ROT_SPEED				(0.05f)		// ��]���x

/************************* NORMAL *************************/
#define BULLET_SPEED						(20.0f)		// �ړ��X�s�[�h
#define BULLET_TAG_VECTOR					(1000.0f)	// �e���x�N�g���p
#define BULLET_NORMAL_EFFECT_SIZE_CHANGE	(0.06f)
#define BULLET_NORMAL_EFFECT_ALPHA_CHANGE	(0.08f)
#define BULLET_NORMAL_DAMAGE				(2.3f)

/************************* SPECIAL *************************/
/* �t�@�C�A�o���b�g */
#define BULLET_SPECIAL_FIRE_SCALE					(0.3f)
#define BULLET_SPECIAL_FIRE_SPEED					(7.0f)
#define BULLET_SPECIAL_FIRE_ROT_SPEED				(0.02f)
#define BULLET_SPECIAL_FIRE_ROT_LENGTH				(0.3f)
#define BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX			(30.0f)
#define BULLET_SPECIAL_FIRE_LENGTH					(1000.0f)
#define BULLET_SPECIAL_FIRE_SPLIT					(1)
#define BULLET_SPECIAL_FIRE_SPLIT_BONUS				(1)
#define BULLET_SPECIAL_FIRE_EFFECT_SIZE_CHANGE		(0.08f)
#define BULLET_SPECIAL_FIRE_EFFECT_ALPHA_CHANGE		(0.10f)
#define BULLET_SPECIAL_FIRE_DAMAGE					(1.4f)
#define BULLET_SPECIAL_FIRE_RAPID					(25)
#define BULLET_SPECIAL_FIRE_CHILD__SCALE			(0.5f)


/* �T���_�[�o���b�g */
#define BULLET_SPECIAL_THUNDER_SCALE				(1.0f)
#define BULLET_SPECIAL_THUNDER_SPEED				(13.0f)
#define BULLET_SPECIAL_THUNDER_ROT_SPEED			(0.04f)
#define BULLET_SPECIAL_THUNDER_ROT_LENGTH			(0.5f)
#define BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX		(50.0f)
#define BULLET_SPECIAL_THUNDER_LENGTH				(1000.0f)
#define BULLET_SPECIAL_THUNDER_HEIGHT				(100.0f)
#define BULLET_SPECIAL_THUNDER_SPLIT				(1)
#define BULLET_SPECIAL_THUNDER_SPLIT_BONUS			(1)
#define BULLET_SPECIAL_THUNDER_EFFECT_SIZE_CHANGE	(0.08f)
#define BULLET_SPECIAL_THUNDER_EFFECT_ALPHA_CHANGE	(0.10f)
#define BULLET_SPECIAL_THUNDER_DAMAGE				(0.7f)
#define BULLET_SPECIAL_THUNDER_RAPID				(30)
#define BULLET_SPECIAL_THUNDER_CHILD__SCALE			(0.8f)
#define BULLET_SPECIAL_THUNDER_AXIS_Y				(0.005f)
#define BULLET_SPECIAL_THUNDER_AXIS_Y_MAX			(0.9999f)
#define BULLET_SPECIAL_THUNDER_STOP					(1.0f)

/* �A�C�X�o���b�g */
#define BULLET_SPECIAL_ICE_SCALE					(0.8f)
#define BULLET_SPECIAL_ICE_SPEED					(12.0f)
#define BULLET_SPECIAL_ICE_ROT_SPEED				(0.08f)
#define BULLET_SPECIAL_ICE_ROT_LENGTH				(0.3f)
#define BULLET_SPECIAL_ICE_LENGTH					(1000.0f)
#define BULLET_SPECIAL_ICE_SPLIT					(3)
#define BULLET_SPECIAL_ICE_SPLIT_BONUS				(3)
#define BULLET_SPECIAL_ICE_EFFECT_SIZE_CHANGE		(0.08f)
#define BULLET_SPECIAL_ICE_EFFECT_ALPHA_CHANGE		(0.10f)
#define BULLET_SPECIAL_ICE_DAMAGE					(1.1f)

/* �E�C���h�o���b�g */
#define BULLET_SPECIAL_WIND_SCALE					(0.3f)
#define BULLET_SPECIAL_WIND_SPEED					(0.1f)
#define BULLET_SPECIAL_WIND_ROT_SPEED				(0.1f)
#define BULLET_SPECIAL_WIND_ROT_LENGTH				(0.03f)
#define BULLET_SPECIAL_WIND_ROT_LENGTH_ADD			(0.002f)
#define BULLET_SPECIAL_WIND_LENGTH					(0.1f)
#define BULLET_SPECIAL_WIND_SPLIT					(2)
#define BULLET_SPECIAL_WIND_SPLIT_BONUS				(2)
#define BULLET_SPECIAL_WIND_EFFECT_SIZE_CHANGE		(0.08f)
#define BULLET_SPECIAL_WIND_EFFECT_ALPHA_CHANGE		(0.10f)
#define BULLET_SPECIAL_WIND_DAMAGE					(2.0f)
#define BULLET_SPECIAL_WIND_MAX						(5)
#define BULLET_SPECIAL_WIND_INTERVAL				(7)

/* �E�H�[�^�[�o���b�g */
#define BULLET_SPECIAL_WATER_SCALE					(0.6f)
#define BULLET_SPECIAL_WATER_SPEED					(7.0f)
#define BULLET_SPECIAL_WATER_ROT_SPEED				(0.08f)
#define BULLET_SPECIAL_WATER_ROT_LENGTH				(0.1f)
#define BULLET_SPECIAL_WATER_LENGTH					(1000.0f)
#define BULLET_SPECIAL_WATER_SPLIT					(2)
#define BULLET_SPECIAL_WATER_SPLIT_BONUS			(2)
#define BULLET_SPECIAL_WATER_EFFECT_SIZE_CHANGE		(0.08f)
#define BULLET_SPECIAL_WATER_EFFECT_ALPHA_CHANGE	(0.10f)
#define BULLET_SPECIAL_WATER_DAMAGE					(1.3f)

/* �A�[�X�o���b�g */
#define BULLET_SPECIAL_EARTH_SCALE					(0.6f)
#define BULLET_SPECIAL_EARTH_SPEED					(15.0f)
#define BULLET_SPECIAL_EARTH_ROT_SPEED				(0.1f)
#define BULLET_SPECIAL_EARTH_ROT_LENGTH				(0.7f)
#define BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX			(70.0f)
#define BULLET_SPECIAL_EARTH_LENGTH					(1000.0f)
#define BULLET_SPECIAL_EARTH_HEIGHT					(250.0f)
#define BULLET_SPECIAL_EARTH_SPLIT					(1)
#define BULLET_SPECIAL_EARTH_SPLIT_BONUS			(1)
#define BULLET_SPECIAL_EARTH_EFFECT_SIZE_CHANGE		(0.08f)
#define BULLET_SPECIAL_EARTH_EFFECT_ALPHA_CHANGE	(0.10f)
#define BULLET_SPECIAL_EARTH_DAMAGE					(0.8f)
#define BULLET_SPECIAL_EARTH_RAPID					(30)
#define BULLET_SPECIAL_EARTH_CHILD__SCALE			(0.7f)
#define BULLET_SPECIAL_EARTH_AXIS_Y					(0.005f)
#define BULLET_SPECIAL_EARTH_AXIS_Y_MAX				(0.9999f)
#define BULLET_SPECIAL_EARTH_STOP					(1.0f)

/************************* SYSTEM *************************/
#define	BULLET_SYS_MAX					(500)	// �V�X�e���̍ő吔
#define BULLET_SYS_FALSE_COUNT			(500)	// �o���b�g�V�X�e�����ŃJ�E���g


typedef struct
{	// Bullet�\����
	D3DXVECTOR3			posBullet;				// �ʒu
	D3DXVECTOR3			rotBullet;				// ����(��])
	D3DXVECTOR3			sclBullet;				// �傫��(�X�P�[��)
	D3DXVECTOR3			vecTag;

	D3DXVECTOR2			vec2Size;				// �T�C�Y

	D3DXCOLOR			xColor;

	int					nUseCount;				// ���ŃJ�E���g
	int					nEffectCount;
	int					nSysNum;
	int					nTex;
	int					nAnimeCount;
	int					nAnimePattern;
	int					nType;
	int					nNum;
	int					nEffect;

	float				fSpeed;				// �ړ����x
	float				fHAngle;
	float				fVAngle;
	float				fAngle;
	float				fBaseAngle;
	float				fRadius;
	float				fEffectSizeChange;
	float				fEffectAlphaChange;

	bool				bUse;					// �g�p�t���O
	bool				bSys;
	bool				bEnemy;
	bool				bVertex;
	bool				bEffect;


	// �V���h�E�p
	int					nIdxShadow;
	float				fSizeShadow;
	float				fSize;
	D3DXCOLOR			colShadow;

}BULLET;

typedef struct
{	// BulletSys�\����
	D3DXVECTOR3			posBulletSys;
	D3DXVECTOR3			posBulletSysTag;
	D3DXVECTOR3			vecBulletSys;
	D3DXVECTOR3			posTag;
	D3DXVECTOR2			vec2Size;		// �T�C�Y
	D3DXVECTOR3			vecAxis;		// ��]��
	D3DXVECTOR3			vecTa;			// ��]����

	D3DXCOLOR			xColor;

	float				fHAngle;
	float				fVAngle;
	float				fAngle;
	float				fSplitAngle;
	float				fSpeed;
	float				fRotSpeed;
	float				fRotLength;
	float				fWave;
	float				fAddAngle;
	float				fLength;

	int					nType;
	int					nChain;
	int					nFireTime;
	int					nSplit;
	int					nCoolDown;
	int					nTex;
	int					nBulletCount;
	int					nUseCount;
	int					nLockon;

	bool				bUse;
	bool				bSet;
	bool				bProcess;
	bool				bTrack;
	bool				bMove;

}BULLET_SYS;

// �o���b�g�^�C�v
enum
{
	BULLET_NORMAL,				// �m�[�}��
	BULLET_SPECIAL_FIRE,		// �t�@�C�A
	BULLET_SPECIAL_THUNDER,		// �T���_�[
	BULLET_SPECIAL_ICE,			// �A�C�X
	BULLET_SPECIAL_WIND,		// �T���_�[
	BULLET_SPECIAL_WATER,		// �E�H�[�^�[
	BULLET_SPECIAL_EARTH,		// �A�[�X
	BULLET_TYPE_MAX				// �ő吔
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(int nType);
void InitStatusBullet(int nBullet);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor, float fScl, int nEffect);
void SetSpecialBulletFire(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge);
void SetSpecialBulletThunder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge);
void SetSpecialBulletIce(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge);
void SetSpecialBulletWind(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa,int nLockon, int nCharge);
void SetSpecialBulletWater(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge);
void SetSpecialBulletEarth(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge);
float GetBulletDamage(int nType);
BULLET *GetBullet(int no);
BULLET_SYS *GetBulletSys(int no);

#endif
