//=============================================================================
//
// ���f������ [model.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "bullet.h"
#include "magiccircle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_MODEL		"data/MODEL/Alicia/Alicia_solid.x"							// �ǂݍ��ރ��f����
//#define	MODEL_MODEL		"data/MODEL/test.x"							// �ǂݍ��ރ��f����

//#define	MODEL_CAR		"data/MODEL/Tune/Tune.x"							// �ǂݍ��ރ��f����

#define MODEL_MAX						(1)

//#define MODEL_SCALE						(0.45f)
#define MODEL_SCALE						(0.20f)
#define MODEL_SIZE						(1.0f)

#define MODEL_POS_X						(0.0f)
#define MODEL_POS_Z						(0.0f)

#define MODEL_FLOAT_SPEED				(0.03f)		// ���V���x
#define MODEL_FLOAT_LENGTH				(0.1f)		// ���V���x

#define MODEL_ALPHA						(200)
#define MODEL_CENTER					(30)

#define MODEL_HEIGHT_EYE				(45.0f)
#define MODEL_HEIGHT_WEAPON				(42.0f)
#define MODEL_H_ANGLE					(D3DX_PI * 1.5f)

#define MODEL_MOVE_SPEED				(5)
#define MODEL_MOVE_BOOST				(10)
#define MODEL_MOVE_INERTIA				(0.3f)

#define MODEL_ROT_SPEED					(0.1f)
#define MODEL_ROT_AUTO_SPEED			(0.2f)

#define MODEL_INVISIBLE_COUNT			(20)

#define MODEL_TAG_COUNT					(30)		// �^�[�Q�b�g�����J�E���g

/* �X�e�[�^�X */
#define MODEL_JUMP_POWER				(8.0f)		// �W�����v�p���[
#define MODEL_JUMP_POWER_DOWN			(0.07f)		// �W�����v�p���[

#define MODEL_STATUS_HP					(40.0f)	// HP
#define MODEL_STATUS_MP					(1000.0f)	// MP
#define MODEL_STATUS_MP_SUB				(10.0f)		// MP�̌��Z��
#define MODEL_STATUS_MP_LIMITER			(100.0f)	// MP�̘A���g�p����
#define MODEL_STATUS_MP_RECOVER			(5.0f)		// MP�̉񕜑��x

// �}�W�b�N�X�e�[�^�X
#define MODEL_STATUS_NORMAL				(100.0f)	// �m�[�}��
#define MODEL_STATUS_FIRE				(100.0f)	// �t�@�C�A
#define MODEL_STATUS_THUNDER			(100.0f)	// �T���_�[
#define MODEL_STATUS_ICE				(100.0f)	// �A�C�X
#define MODEL_STATUS_WIND				(100.0f)	// �E�B���h
#define MODEL_STATUS_WARER				(100.0f)	// �E�H�[�^�[
#define MODEL_STATUS_EARTH				(100.0f)	// �A�[�X

// �}�W�b�N�̌��Z��
#define MODEL_STATUS_NORMAL_SUB			(0.5f)		// �m�[�}��
#define MODEL_STATUS_FIRE_SUB			(20.0f)		// �t�@�C�A
#define MODEL_STATUS_THUNDER_SUB		(20.0f)		// �T���_�[
#define MODEL_STATUS_ICE_SUB			(20.0f)		// �A�C�X
#define MODEL_STATUS_WIND_SUB			(20.0f)		// �E�B���h
#define MODEL_STATUS_WATER_SUB			(20.0f)		// �E�H�[�^�[
#define MODEL_STATUS_EARTH_SUB			(20.0f)		// �A�[�X
#define MODEL_STATUS_MAGIC_RECOVER		(0.03f)		// �}�W�b�N�̉񕜑��x

#define MODEL_STATUS_CHARGE_MAX			(5)			// �ő�`���[�W
#define MODEL_STATUS_CHARGE_SPEED		(12)		// �`���[�W�X�s�[�h

/* �A�^�b�N */
#define MODEL_ATTACK_NORMAL				(10)		// �ʏ�U���C���^�[�o��
#define MODEL_ATTACK_FIRE				(25)		// �t�@�C�A�C���^�[�o��
#define MODEL_ATTACK_THUNDER			(25)		// �T���_�[�C���^�[�o��
#define MODEL_ATTACK_ICE				(25)		// �A�C�X�C���^�[�o��
#define MODEL_ATTACK_WIND				(25)		// �E�B���h�C���^�[�o��
#define MODEL_ATTACK_WATER				(25)		// �E�H�[�^�[�C���^�[�o��
#define MODEL_ATTACK_EARTH				(25)		// �A�[�X�C���^�[�o��


/* �X�� */
#define MODEL_TILT_ROT					(0.005f)		// �ړ����̌X�����x
#define MODEL_TILT_ROT_BOOST			(0.01f)		// �u�[�X�g�̌X�����x
#define MODEL_TILT_ROT_AUTO				(0.005f)		// �����ړ����x
/* �ʏ�ړ��� */
#define MODEL_TILT_MARGIN_FRONT			(0.1f)		// �O���ړ����E�X��
#define MODEL_TILT_MARGIN_LEAR			(0.1f)		// ����ړ����E�X��
#define MODEL_TILT_MARGIN_SIDE			(0.1f)		// ���ʈړ����E�X��
/* �u�[�X�g�� */
#define MODEL_TILT_MARGIN_BOOST_FRONT	(0.2f)		// �O���ړ����E�X��
#define MODEL_TILT_MARGIN_BOOST_LEAR	(0.2f)		// ����ړ����E�X��
#define MODEL_TILT_MARGIN_BOOST_SIDE	(0.2f)		// ���ʈړ����E�X��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//typedef struct
//{
//	bool			bUse;
//	int				nAttackCount;
//	float			fStatus;
//	float			fStatusMax;
//	float			fStatusSub;
//}MODEL_MAGIC;

typedef struct
{
	int				nIdx;
	bool			bUse;
}MODEL_MAGICCIRCLE;

typedef struct
{
	//MODEL_MAGIC		modelMagic[BULLET_TYPE_MAX];
	D3DXVECTOR3		posModel;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rotModel;			// �n�ʂ̌���(��])
	D3DXVECTOR3		sclModel;			// �n�ʂ̑傫��(�X�P�[��)
	D3DXVECTOR3		moveModel;

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X

	int				nTag;
	int				nTagNum;
	int				nTagReleaseCount;

	int				nCharge;
	int				nChargeCount;

	int				nAttackNormalCount;
	int				nAttackFireCount;
	int				nAttackThunderCount;
	int				nAttackIceCount;
	int				nAttackWindCount;
	int				nAttackWaterCount;
	int				nAttackEarthCount;

	int				nInvisibleCount;

	float			fStatusHP;
	float			fStatusMP;
	float			fStatusNormal;
	float			fStatusFire;
	float			fStatusThunder;
	float			fStatusIce;
	float			fStatusWind;
	float			fStatusWater;
	float			fStatusEarth;


	float			fHAngle;
	float			fHAngleDest;
	float			fHAngleDiff;
	float			fMoveSpeed;
	float			fFloatAngle;
	float			fFloatPos;
	float			fJumpAccel;

	bool			bUse;
	bool			bLockOn;
	bool			bParameter;
	bool			bJump;
	bool			bStatusMPLimiter;
	bool			bDash;

	bool			bActiveFire;
	bool			bActiveThunder;
	bool			bActiveIce;
	bool			bActiveWind;
	bool			bActiveWater;
	bool			bActiveEarth;

	// �V���h�E�p
	int				nIdxShadow;
	float			fSizeShadow;
	D3DXCOLOR		colShadow;
	bool			bShadow;

	// �~�j�}�b�v�p
	bool			bUseMinimap;

	// �}�W�b�N�T�[�N���p
	MODEL_MAGICCIRCLE magicCircle[MAGICCIRCLE_MAX];
}MODEL;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitModel(int nType);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
bool CheckMagicModel(int nModel, int nMagic);
void SetMagicModel(int nModel, int nMagic);
void SetMagicChargeModel(int nModel);
MODEL *GetModel(int no);
#endif
