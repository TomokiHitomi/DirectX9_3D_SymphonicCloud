//=============================================================================
//
// ���f������ [model.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "main.h"
#include "shadow.h"
#include "calculate.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"
#include "parameter.h"
#include "cloud.h"
#include "lockon.h"
#include "game.h"
#include "magic.h"
#include "lockon.h"
#include "sound.h"
#include "player.h"
#include "Light.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MoveModel(int nModel, int CameraMode, int CameraGameMode);
void AttackNormalModel(int nModel, int CameraMode, int CameraGameMode);
void AttackMagicModel(int nModel, int CameraMode, int CameraGameMode);
void MoveAngleModel(int nModel, float fAngle);
void JumpModel(int nModel);
void TiltModel(int nModel);
void FloatModel(int nModel);
void TagModel(int nModel);
void MagicRecoverModel(int nModel, int nMagic);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshModel;

LPD3DXMESH			g_pD3DXMeshModel;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatModel;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatModel;			// �}�e���A�����̐�

MODEL				modelWk[MODEL_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(int nType)
{
	MODEL *model = &modelWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MODEL_MAX; i++, model++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		model->posModel = D3DXVECTOR3(MODEL_POS_X, 0.0f, MODEL_POS_Z);
		model->rotModel = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		model->sclModel = D3DXVECTOR3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
		model->moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		model->nTag = 0;
		model->nTagNum = 0;
		model->nTagReleaseCount = 0;

		model->nAttackNormalCount = 0;

		model->nCharge = 0;
		model->nChargeCount = 0;

		model->nChangeLightSpot = 0;
		model->nChangeLightPoint = 0;

		model->nAttackFireCount = 0;
		model->nAttackThunderCount = 0;
		model->nAttackIceCount = 0;
		model->nAttackWindCount = 0;
		model->nAttackWaterCount = 0;
		model->nAttackEarthCount = 0;

		model->nInvisibleCount = 0;
		model->nAttackSpCount = 0;

		model->bUse = true;
		model->bLockOn = false;
		model->bParameter = false;
		model->bJump = false;
		model->bStatusMPLimiter = false;
		model->bDash = false;
		model->bAttack = false;
		model->bAttackSp = false;

		model->bActiveFire = false;
		model->bActiveThunder = false;
		model->bActiveIce = false;
		model->bActiveWind = false;
		model->bActiveWater = false;
		model->bActiveEarth = false;

		model->fStatusHP = MODEL_STATUS_HP;
		model->fStatusMP = MODEL_STATUS_MP;
		model->fStatusNormal = MODEL_STATUS_NORMAL;
		model->fStatusFire = 0.0f;
		model->fStatusThunder = 0.0f;
		model->fStatusIce = 0.0f;
		model->fStatusWind = 0.0f;
		model->fStatusWater = 0.0f;
		model->fStatusEarth = 0.0f;

		model->fMoveSpeed = MODEL_MOVE_SPEED;
		model->fHAngle = 0.0f;
		model->fHAngleDest = MODEL_H_ANGLE;
		model->fHAngleDiff = 0.0f;
		model->fFloatAngle = 0.0f;
		model->fFloatPos = 0.0f;
		model->fJumpAccel = 0.0f;

		// �V���h�E�p
		model->nIdxShadow = 0;
		model->fSizeShadow = 0.0f;
		model->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
		model->bShadow = false;

		// �~�j�}�b�v
		model->bUseMinimap = false;
		//SetParameter(0, model->posModel, i);

		// �}�W�b�N�T�[�N��
		for (int j = 0; j < MAGICCIRCLE_MAX; j++)
		{
			model->magicCircle[j].bUse = false;
			model->magicCircle[j].nIdx = -1;
		}
	}

	if (nType == 0)
	{
		//SetParameter(0, model->posModel, 0);
		// ���f���֌W�̏�����
		g_pD3DTextureModel = NULL;
		g_pD3DXMeshModel = NULL;
		g_pD3DXBuffMatModel = NULL;
		g_nNumMatModel = 0;

		//// ���f���f�[�^�ǂݍ���
		//if (FAILED(D3DXLoadMeshFromX(MODEL_MODEL,			// ���f���f�[�^
		//	D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
		//	pDevice,					// �f�o�C�X
		//	NULL,						// ���g�p
		//	&g_pD3DXBuffMatModel,		// �}�e���A���f�[�^
		//	NULL,						// ���g�p
		//	&g_nNumMatModel,			// D3DXMATERIAL�\���̂̐�
		//	&g_pD3DXMeshModel)))		// ���b�V���f�[�^�ւ̃|�C���^
		//{
		//	return E_FAIL;
		//}

		//// �}�e���A���������o��
		//D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();
		//g_pD3DMatMeshModel = new D3DMATERIAL9[g_nNumMatModel];		// ���b�V�������m��
		//g_pD3DTextureModel = new LPDIRECT3DTEXTURE9[g_nNumMatModel];// �e�N�X�`�����m��
		//for (int i = 0; i < g_nNumMatModel; i++)
		//{
		//	g_pD3DMatMeshModel[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
		//	g_pD3DMatMeshModel[i].Ambient = g_pD3DMatMeshModel[i].Diffuse;// ����������
		//	g_pD3DTextureModel[i] = NULL;	// �e�N�X�`��������

		//	// �g�p���Ă���e�N�X�`��������Γǂݍ���
		//	if (d3Mat[i].pTextureFilename != NULL &&
		//		lstrlen(d3Mat[i].pTextureFilename) > 0)
		//	{
		//		// �e�N�X�`���ǂݍ���
		//		if (FAILED(D3DXCreateTextureFromFile(
		//			pDevice,
		//			d3Mat[i].pTextureFilename,
		//			&g_pD3DTextureModel[i])))
		//		{
		//			return E_FAIL;
		//		}
		//	}
		//}
	}
	return S_OK;
}

////=============================================================================
//// �}�W�b�N����������
////=============================================================================
//void InitModelMagic(int nModel, int nMagic)
//{
//	MODEL *model = &modelWk[nModel];
//	if (nMagic == BULLET_NORMAL)
//	{
//		model->modelMagic[nMagic].bUse = true;
//		model->modelMagic[nMagic].fStatus = MODEL_STATUS_NORMAL;
//	}
//	else
//	{
//		model->modelMagic[nMagic].bUse = false;
//		model->modelMagic[nMagic].fStatus = 0;
//	}
//
//	model->modelMagic[nMagic].nAttackCount = 0;
//
//	switch (nMagic)
//	{
//	case BULLET_NORMAL:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_NORMAL_SUB;
//		break;
//	case BULLET_SPECIAL_FIRE:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_FIRE_SUB;
//		break;
//	case BULLET_SPECIAL_THUNDER:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_THUNDER_SUB;
//		break;
//	case BULLET_SPECIAL_ICE:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_ICE_SUB;
//		break;
//	case BULLET_SPECIAL_WIND:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_WIND_SUB;
//		break;
//	case BULLET_SPECIAL_WATER:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_WATER_SUB;
//		break;
//	case BULLET_SPECIAL_EARTH:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_EARTH_SUB;
//		break;
//
//	}
//}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	if(g_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		for (int i = 0; i < g_nNumMatModel; i++)
		{
			g_pD3DTextureModel[i]->Release();
			g_pD3DTextureModel[i] = NULL;
		}
	}

	if(g_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshModel->Release();
		g_pD3DXMeshModel = NULL;
	}

	if(g_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatModel->Release();
		g_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	MODEL *model = &modelWk[0];
	int *pCameraMode = GetCameraMode();
	int *pCameraGameMode = GetCameraGameMode();
	E_STAGE eStage = GetStage();
#ifdef _DEBUG
	PrintDebugProc("�y MODEL �z\n");
	PrintDebugProc("HP:%f  rotZ:%f\n", model->fStatusHP, model->rotModel.z);
#endif

	// �X�L���S�J��
	if (GetKeyboardPress(DIK_P) || (IsButtonPressed(0, BUTTON_R) && IsButtonPressed(0, R_BUTTON_ZR) && IsButtonPressed(0, BUTTON_HOME)))
	{
		SetMagicType(BULLET_SPECIAL_FIRE);
		SetMagicType(BULLET_SPECIAL_THUNDER);
		SetMagicType(BULLET_SPECIAL_ICE);
		SetMagicType(BULLET_SPECIAL_WIND);
		SetMagicType(BULLET_SPECIAL_WATER);
		SetMagicType(BULLET_SPECIAL_EARTH);
	}

	for (int i = 0; i < MODEL_MAX; i++, model++)
	{
		if (model->bUse)
		{
			//if (!model->bParameter)
			//{	// HP�o�[�Z�b�g�i���j
			//	SetParameter(0, model->posModel, 0);
			//	model->bParameter = true;
			//}

			if (model->nInvisibleCount > 0)
			{
				model->nInvisibleCount--;
			}

			if (!model->bShadow)
			{
				model->nIdxShadow = CreateShadow(model->posModel, 25.0f, 25.0f);
				model->fSizeShadow = 30.0f;
				model->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
				model->bShadow = true;
			}

			//// �X�e�[�W�J�ڃe�X�g
			//if (GetKeyboardTrigger(DIK_RETURN))
			//{
			//	SetFade(FADE_OUT, STAGE_RESULT);
			//}

			switch (eStage)
			{
			case STAGE_TITLE:
				FloatModel(i);									// ���V����
				break;
			case STAGE_TUTORIAL:
				break;
			case STAGE_CREDIT:
				break;
			case STAGE_GAME:
				TagModel(i);									// �^�[�Q�b�g�ύX����
				JumpModel(i);									// �W�����v����
				TiltModel(i);									// �X������
				MoveModel(i, *pCameraMode, *pCameraGameMode);	// �ړ�����
				MagicRecoverModel(i,0);
				AttackNormalModel(i, *pCameraMode, *pCameraGameMode);	// �U������
				AttackMagicModel(i, *pCameraMode, *pCameraGameMode);	// �U������

				// �V���h�E�Ǘ�
				SetPositionShadow(model->nIdxShadow, D3DXVECTOR3(model->posModel.x, 0.2f, model->posModel.z));
				SetVertexShadow(model->nIdxShadow, model->fSizeShadow, model->fSizeShadow);
				SetColorShadow(model->nIdxShadow, model->colShadow);

				break;
			case STAGE_RESULT:
				FloatModel(i);									// ���V����
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	MODEL *model = &modelWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	//// ���e�X�g��L����
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, MODEL_ALPHA);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&model->mtxWorld);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, model->sclModel.x, model->sclModel.y, model->sclModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rotModel.y, model->rotModel.x, model->rotModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, model->posModel.x, model->posModel.y, model->posModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

	/******************** �r���[�|�[�g�ϊ� ********************/
	// ���݂̃}�e���A����ۑ�
	pDevice->GetMaterial(&matDef);
	//// �}�e���A�����ɑ΂���|�C���^�̎擾
	//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for (int i = 0; i < g_nNumMatModel; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&g_pD3DMatMeshModel[i]);
		// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
		pDevice->SetTexture(0, g_pD3DTextureModel[i]);
		// �`��
		g_pD3DXMeshModel->DrawSubset(i);
	}
	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	//// ���e�X�g�𖳌���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// �X�������֐�
//=============================================================================
void TiltModel(int nModel)
{
	MODEL *model = &modelWk[nModel];

	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	{	// �O���ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (model->rotModel.x > -MODEL_TILT_MARGIN_BOOST_FRONT)
			{	// �u�[�X�g�ő�p�x�ȉ��ł���΃u�[�X�g�X�s�[�h�ŌX��
				model->rotModel.x -= MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (model->rotModel.x > -MODEL_TILT_MARGIN_FRONT)
			{	// �ʏ�ő�p�x�ȉ��ł���Βʏ�X�s�[�h�ŌX��
				model->rotModel.x -= MODEL_TILT_ROT;
			}
			else if (model->rotModel.x < -MODEL_TILT_MARGIN_FRONT)
			{	// �ʏ�ő�p�x�ȏ�ł���΃I�[�g�X�s�[�h�ŌX��
				model->rotModel.x += MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	{	// ����ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (model->rotModel.x < MODEL_TILT_MARGIN_BOOST_LEAR)
			{	// �u�[�X�g�ő�p�x�ȉ��ł���΃u�[�X�g�X�s�[�h�ŌX��
				model->rotModel.x += MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (model->rotModel.x < MODEL_TILT_MARGIN_LEAR)
			{	// �ʏ�ő�p�x�ȉ��ł���Βʏ�X�s�[�h�ŌX��
				model->rotModel.x += MODEL_TILT_ROT;
			}
			else if (model->rotModel.x > MODEL_TILT_MARGIN_LEAR)
			{	// �ʏ�ő�p�x�ȏ�ł���΃I�[�g�X�s�[�h�ŌX��
				model->rotModel.x -= MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else
	{	// �O��ړ����Ȃ�
		if (model->rotModel.x < 0.0f)
		{	// ���_������ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ŌX��
			model->rotModel.x += MODEL_TILT_ROT_AUTO;
		}
		if (model->rotModel.x > 0.0f)
		{	// ���_���O���ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ŌX��
			model->rotModel.x -= MODEL_TILT_ROT_AUTO;
		}
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	{	// �����ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (model->rotModel.z > -(MODEL_TILT_MARGIN_BOOST_SIDE))
			{	// �u�[�X�g�ő�p�x�ȉ��ł���΃u�[�X�g�X�s�[�h�ŌX��
				model->rotModel.z -= MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (model->rotModel.z > -(MODEL_TILT_MARGIN_SIDE))
			{	// �ʏ�ő�p�x�ȉ��ł���Βʏ�X�s�[�h�ŌX��
				model->rotModel.z -= MODEL_TILT_ROT;
				if (model->rotModel.z < -(MODEL_TILT_MARGIN_SIDE))
				{
					model->rotModel.z = -(MODEL_TILT_MARGIN_SIDE);
				}
			}
			else if (model->rotModel.z < -(MODEL_TILT_MARGIN_SIDE))
			{	// �ʏ�ő�p�x�ȏ�ł���΃I�[�g�X�s�[�h�ŌX��
				model->rotModel.z += MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	{	// �E���ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (model->rotModel.z < (MODEL_TILT_MARGIN_BOOST_SIDE))
			{	// �u�[�X�g�ő�p�x�ȉ��ł���΃u�[�X�g�X�s�[�h�ŌX��
				model->rotModel.z += MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (model->rotModel.z < (MODEL_TILT_MARGIN_SIDE))
			{	// �ʏ�ő�p�x�ȉ��ł���Βʏ�X�s�[�h�ŌX��
				model->rotModel.z += MODEL_TILT_ROT;
				if (model->rotModel.z > MODEL_TILT_MARGIN_SIDE)
				{
					model->rotModel.z = MODEL_TILT_MARGIN_SIDE;
				}
			}
			else if (model->rotModel.z >(MODEL_TILT_MARGIN_SIDE))
			{	// �ʏ�ő�p�x�ȏ�ł���΃I�[�g�X�s�[�h�ŌX��
				model->rotModel.z -= MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else
	{	// ���E�ړ����Ȃ�
		if (model->rotModel.z < 0.0f)
		{	// ���_������ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ŌX��
			model->rotModel.z += MODEL_TILT_ROT_AUTO;
		}
		if (model->rotModel.z > 0.0f)
		{	// ���_���O���ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ŌX��
			model->rotModel.z -= MODEL_TILT_ROT_AUTO;
		}
	}
}

//=============================================================================
// �ړ������֐�
//=============================================================================
void MoveModel(int nModel ,int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];

	if (CameraMode == CAMERA_GAME)
	{
		CAMERA *camera = GetCamera(CAMERA_GAME);

		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R))						// �V�t�g�L�[������
		{
			if (model->fStatusMP < MODEL_STATUS_MP_SUB)			// MP�����Z�l�����������
			{
				SetSe(9, E_DS8_FLAG_NONE, false);
				model->bStatusMPLimiter = true;					// MP���~�b�^�[�L��
				model->bDash = false;
			}
			else if(model->fStatusMP > MODEL_STATUS_MP_LIMITER)	// MP�����~�b�^�[�l���z������
			{
				model->bStatusMPLimiter = false;				// MP���~�b�^�[����
				model->bDash = true;
			}
			model->fStatusMP -= MODEL_STATUS_MP_SUB;			// MP�����Z
			if (model->fStatusMP < 0.0f)
			{													// �ŏ��l�����������
				model->fStatusMP = 0.0f;						// �ŏ��l����
			}
		}
		else
		{
			model->fStatusMP += MODEL_STATUS_MP_RECOVER;		// MP���R��
			if (model->fStatusMP > MODEL_STATUS_MP)				// MP�ő�l���z������
			{
				model->fStatusMP = MODEL_STATUS_MP;
			}				// MP�ő�l���ێ�
			model->bDash = false;
		}

		// �J�����̊p�x���擾
		int nAngelTemp = camera->fHAngle * 100;
		model->fHAngle = (nAngelTemp % 628) * 0.01f;

		// �ړ�����
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
		{
			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
			{// ���O�ړ�
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.25f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.25f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.25f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// ����ړ�
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.75f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.75f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.75f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
			{// ���E���������͏����Ȃ�
			}
			else
			{// ���ړ�
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.50f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.50f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.50f) * model->fMoveSpeed;
			}
			//SetPlayerAnime(0, ANIME05);
		}
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
		{
			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_UP))
			{// �E�O�ړ�
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.25f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.25f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.25f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// �E��ړ�
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.75f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.75f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.75f) * model->fMoveSpeed;
			}
			else
			{// �E�ړ�
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.50f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.50f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.50f) * model->fMoveSpeed;
			}
			//SetPlayerAnime(0, ANIME06);
		}
		else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{// �O�ړ�
			if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// �O�㓯�������͏����Ȃ�
			}
			else
			{
				MoveAngleModel(nModel, model->fHAngle);
				//model->moveModel.x = cosf(model->fHAngle) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle) * model->fMoveSpeed;
				//SetPlayerAnime(0, ANIME04);
			}
		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// ��ړ�
			MoveAngleModel(nModel, model->fHAngle + D3DX_PI);
			//model->moveModel.x = cosf(model->fHAngle + D3DX_PI) * model->fMoveSpeed;
			//model->moveModel.z = sinf(model->fHAngle + D3DX_PI) * model->fMoveSpeed;
			//SetPlayerAnime(0, ANIME07);
		}
		else
		{
			//SetPlayerAnime(0, ANIME03);
		}

		// �J�����ƃ��f���̊p�x�𓯊�
		model->fHAngleDest = D3DX_PI - model->fHAngle - D3DX_PI * 0.50f;

		// ��]��ƌ��݂̊p�x�����v�Z
		model->fHAngleDiff = model->fHAngleDest - model->rotModel.y;
		model->fHAngleDiff = PiCalculate180(model->fHAngleDiff);

		// �p�x����K�p
		model->rotModel.y += model->fHAngleDiff * MODEL_ROT_AUTO_SPEED;
		model->rotModel.y = PiCalculate180(model->rotModel.y);

		model->posModel -= model->moveModel;

		// �ړ��ʂɊ�����������
		model->moveModel.x += (0.0f - model->moveModel.x) * MODEL_MOVE_INERTIA;
		model->moveModel.y += (0.0f - model->moveModel.y) * MODEL_MOVE_INERTIA;
		model->moveModel.z += (0.0f - model->moveModel.z) * MODEL_MOVE_INERTIA;
	}

// �f�o�b�O�p
#ifdef _DEBUG
	PrintDebugProc("�yMODEL�z\n");
	PrintDebugProc("Pos[X:%f Z:%f]  Rot[Y:%f]\n", model->posModel.x, model->posModel.z, model->rotModel.y);
	PrintDebugProc("Tag[%d]  TagNum[%d]  TagReleaseCount[%d]  ", model->nTag, model->nTagNum, model->nTagReleaseCount);
	if (model->bLockOn)
	{
		PrintDebugProc("LockOn[true]\n");
	}
	else
	{
		PrintDebugProc("LockOn[false]\n");
	}
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �ړ������֐�
//=============================================================================
void MoveAngleModel(int nModel, float fAngle)
{
	MODEL *model = GetModel(nModel);
	model->moveModel.x = cosf(fAngle) * model->fMoveSpeed;
	model->moveModel.z = sinf(fAngle) * model->fMoveSpeed;

	if (model->bAttack || model->bAttackSp)
	{
	}
	else
	{
		model->fHAngle = fAngle;
	}
}

//=============================================================================
// �^�[�Q�b�g�ύX�����֐�
//=============================================================================
void TagModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	ENEMY *enemy;
	ENEMY_SYS *enemySys = GetEnemySys();

	int pCameraGameMode = *GetCameraGameMode();


	if (enemySys->nActiveCount == 0)
	{
		model->bLockOn = false;
		if (pCameraGameMode != CAMERA_TPS)
		{
			SetCameraGameMode(CAMERA_NORMAL);
		}
		else if (pCameraGameMode != CAMERA_NORMAL)
		{
			SetCameraGameMode(CAMERA_TPS);
		}
	}
	

	if (GetKeyboardTrigger(DIK_TAB) && enemySys->nActiveCount != 0)
	{
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			// �^�[�Q�b�g����i�߂�j
			if (model->bLockOn)
			{
				model->nTagNum--;
			}
			// 0����������ꍇ�̓A�N�e�B�G�l�~�[�ő吔����-1�ɂ��ǂ�
			if (model->nTagNum < 0)
			{
				model->nTagNum = enemySys->nActiveCount - 1;
				if (model->nTagNum > ENEMY_ACTIVE_COUNT_MAX)
				{
					model->nTagNum = ENEMY_ACTIVE_COUNT_MAX - 1;
				}
			}
		}
		else
		{
			// �^�[�Q�b�g����
			if (model->bLockOn)
			{
				model->nTagNum++;
			}
			// �A�N�e�B�u�G�l�~�[�A�ő�J�E���g���z�����ꍇ��0�Ԗڂɂ��ǂ�
			if (model->nTagNum > enemySys->nActiveCount || model->nTagNum > ENEMY_ACTIVE_COUNT_MAX - 1)
			{
				model->nTagNum = 0;
			}
		}
		// 1�ȏ�̓G�l�~�[�ɃZ�b�g
		SetCameraTag(enemySys->nActiveEnemy[model->nTagNum]);
		// ���f�����^�[�Q�b�g����G�l�~�[���Z�b�g
		model->nTag = enemySys->nActiveEnemy[model->nTagNum];
		SetCameraGameMode(CAMERA_TARGET);
		model->bLockOn = true;
	}

	if (model->bLockOn)
	{
		// ���f�����^�[�Q�b�g���Ă���G�l�~�[���Ăяo��
		enemy = GetEnemy(model->nTag);
		if (!enemy->bUse)
		{
			// �^�[�Q�b�g����
			model->nTagNum++;
			// �A�N�e�B�u�G�l�~�[�A�ő�J�E���g���z�����ꍇ��0�Ԗڂɂ��ǂ�
			if (model->nTagNum > enemySys->nActiveCount -1 || model->nTagNum > ENEMY_ACTIVE_COUNT_MAX - 1)
			{
				model->nTagNum = 0;
			}
			// 1�ȏ�̓G�l�~�[�ɃZ�b�g
			SetCameraTag(enemySys->nActiveEnemy[model->nTagNum]);
			// ���f�����^�[�Q�b�g����G�l�~�[���Z�b�g
			model->nTag = enemySys->nActiveEnemy[model->nTagNum];
		}
		// TAB�������Ń^�[�Q�b�g����
		if (GetKeyboardPress(DIK_TAB) || enemySys->nActiveCount == 0)
		{
			model->nTagReleaseCount++;
			if (model->nTagReleaseCount > MODEL_TAG_COUNT)
			{
				model->bLockOn = false;
				// 0�̓v���C���[�ɃZ�b�g
				SetCameraTag(0);
				model->nTagReleaseCount = 0;
				// �^�[�Q�b�g�������J�������[�h�ύX
				SetCameraGameMode(CAMERA_NORMAL);
			}
		}
		else
		{
			model->nTagReleaseCount = 0;
		}
	}
}

//=============================================================================
// MP�m�F�֐�
//=============================================================================
bool CheckMagicModel(int nModel, int nMagic)
{
	MODEL *model = &modelWk[nModel];
	switch (nMagic + 1)
	{
	case BULLET_SPECIAL_FIRE:
		if (model->fStatusFire > MODEL_STATUS_FIRE_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_THUNDER:
		if (model->fStatusThunder > MODEL_STATUS_THUNDER_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_ICE:
		if (model->fStatusIce > MODEL_STATUS_ICE_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_WIND:
		if (model->fStatusWind > MODEL_STATUS_WIND_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_WATER:
		if (model->fStatusWater > MODEL_STATUS_WATER_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_EARTH:
		if (model->fStatusEarth > MODEL_STATUS_EARTH_SUB)
		{
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// �ʏ�U���֐�
//=============================================================================
void AttackNormalModel(int nModel, int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];
	CAMERA *camera = GetCamera(CameraMode);
	D3DXVECTOR3 vecTemp = model->posModel + D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI)*MODEL_LENGTH_WEAPON, MODEL_HEIGHT_WEAPON, sinf(model->fHAngle + D3DX_PI)*MODEL_LENGTH_WEAPON);

	if (IsMobUseLeftPressed() || GetKeyboardPress(DIK_B) || IsButtonPressed(0, R_BUTTON_ZR))
	{	// �m�[�}���o���b�g����
		if (model->nAttackNormalCount <= 0 && model->fStatusNormal > MODEL_STATUS_NORMAL_SUB)
		{	// �m�[�}���o���b�g�̃N�[���_�E���m�F
			model->nChangeLightPoint = 0;
			SetLight(LIGHT_POINT, TRUE);
			SetLightPoint(LIGHT_POINT, SetColorPallet(COLOR_PALLET_MAGENTA),
				vecTemp,
				30.0f, 0.03f);
			model->bAttack = true;
			// ���_�������_�̃x�N�g���Ɍ����x�N�g�����Z�b�g
			D3DXVECTOR3 vecTag = camera->posCameraAt - camera->posCameraEye;
			D3DXVec3Normalize(&vecTag, &vecTag);
			SetBullet(
				camera->posCameraAt + vecTag * BULLET_TAG_VECTOR,
				vecTemp,
				SetColorPallet(COLOR_PALLET_MAGENTA),
				1.0f,
				BULLET_NORMAL);
			model->nAttackNormalCount = MODEL_ATTACK_NORMAL;
			model->fStatusNormal -= MODEL_STATUS_NORMAL_SUB;
		}
	}
	else
	{
		model->bAttack = false;
	}

	if (model->nChangeLightPoint < MODEL_LIGHT_POINT)
	{
		model->nChangeLightPoint++;
		//SetLightPoint(LIGHT_POINT, SetColorPallet(COLOR_PALLET_MAGENTA) / (float)model->nChangeLightPoint,
		//	vecTemp,
		//	40.0f, 0.03f);
	}
	else
	{
		SetLight(LIGHT_POINT, FALSE);
	}
}

//=============================================================================
// �}�W�b�N�֐�
//=============================================================================
void AttackMagicModel(int nModel, int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];
	CAMERA *camera = GetCamera(CameraMode);

	// �}�W�b�N�T�[�N���X�V
	for (int j = 0; j < MAGICCIRCLE_MAX; j++)
	{
		if (model->magicCircle[j].bUse)
		{
			SetMagiccirclePos(model->magicCircle[j].nIdx, model->posModel, model->fHAngleDiff * MODEL_ROT_AUTO_SPEED);
		}
	}

	if (!CheckMagicModel(0, GetTypeMagic()))
	{
		// MP���Ȃ��ꍇ�̓m���`���[�W
		model->nAttackSpCount++;
		if (model->nAttackSpCount > PLAYER_RECOIL_TIME)
		{
			model->bAttackSp = false;
			model->nAttackSpCount = 0;
		}
	}
	else if (GetKeyboardPress(DIK_V) || IsMobUseRightPressed() || IsButtonPressed(0, R_BUTTON_R))
	{	// �}�W�b�N�T�[�N���`���[�W
		if (model->nChargeCount % MODEL_STATUS_CHARGE_SPEED == 0 || model->nChargeCount == 0)
		{
			model->bAttackSp = false;
			//model->bAttackSp = true;
			for (int j = 0; j < MAGICCIRCLE_MAX; j++)
			{
				if (!model->magicCircle[j].bUse)
				{
					model->magicCircle[j].nIdx = SetMagiccircle();
					model->magicCircle[j].bUse = true;
					break;
				}
			}
		}
		model->nChargeCount++;
		model->nChangeLightSpot = 0;
		SetLight(LIGHT_SPOT, TRUE);
		SetLightSpot(LIGHT_SPOT, GetMagicColor() * ((float)model->nCharge / (float)MAGICCIRCLE_MAX),
			model->posModel + D3DXVECTOR3(0.0f, 100.0f,0.0f),
			D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 0.02f, 0.5f, 0.5f);
	}
	else if (GetKeyboardRelease(DIK_V) || IsMobUseRightReleased() || IsButtonReleased(0, R_BUTTON_R))
	{
		//if (model->nChangeLightSpot < MODEL_LIGHT_SPOT)
		//{
		//	model->nChangeLightSpot++;
		//	SetLightSpot(LIGHT_SPOT, GetMagicColor() / (float)model->nChangeLightSpot,
		//		model->posModel + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		//		D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 0.02f, 0.5f, 0.5f);
		//}
		//else
		//{
			SetLight(LIGHT_SPOT, FALSE);
		//}
		model->bAttackSp = true;
		model->nAttackSpCount = 0;
		//SetPlayerAnime(0, ANIME08);

		// �}�W�b�N�o���b�g����
		switch (GetTypeMagic() + 1)
		{
		case BULLET_SPECIAL_FIRE:
			if (model->nAttackFireCount < 0 && model->fStatusFire > MODEL_STATUS_FIRE_SUB)
			{

				D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
				D3DXVec3Normalize(&vecTemp, &vecTemp);

				SetSpecialBulletFire(
					model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
					camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
					D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
					-1,
					model->nCharge);

				model->nAttackFireCount = MODEL_ATTACK_FIRE;
				model->fStatusFire -= MODEL_STATUS_FIRE_SUB;
				SetVoice(VOICE_FIRE, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_THUNDER:
			if (model->nAttackThunderCount < 0 && model->fStatusThunder > MODEL_STATUS_THUNDER_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletThunder(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletThunder(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackThunderCount = MODEL_ATTACK_THUNDER;
				model->fStatusThunder -= MODEL_STATUS_THUNDER_SUB;
				SetVoice(VOICE_THUNDER, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_ICE:
			if (model->nAttackIceCount < 0 && model->fStatusIce > MODEL_STATUS_ICE_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletIce(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletIce(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackIceCount = MODEL_ATTACK_ICE;
				model->fStatusIce -= MODEL_STATUS_ICE_SUB;
				SetVoice(VOICE_ICE, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_WIND:
			if (model->nAttackWindCount < 0 && model->fStatusWind > MODEL_STATUS_WIND_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				//int nLockonCount = 0;
				//for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				//{
					//if (lockon->bUse && !lockon->bRelease)
					//{
						SetSpecialBulletWind(
							model->posModel,
							D3DXVECTOR3(0.0f, 1.0f, 0.0f),
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							-1,
							model->nCharge);
						//nLockonCount++;
					//}
				//}
				//if (nLockonCount == 0)
				//{
				//	D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
				//	D3DXVec3Normalize(&vecTemp, &vecTemp);

				//	SetSpecialBulletWind(
				//		model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
				//		camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
				//		D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
				//		-1,
				//		model->nCharge);
				//}
				model->nAttackWindCount = MODEL_ATTACK_WIND;
				model->fStatusWind -= MODEL_STATUS_WIND_SUB;
				SetVoice(VOICE_WIND, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_WATER:
			// �E�H�[�^�[�o���b�g�̃N�[���_�E���m�F
			if (model->nAttackWaterCount < 0 && model->fStatusWater > MODEL_STATUS_WATER_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletWater(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletWater(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackWaterCount = MODEL_ATTACK_WATER;
				model->fStatusWater -= MODEL_STATUS_WATER_SUB;
				SetVoice(VOICE_WATER, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_EARTH:
			if (model->nAttackEarthCount < 0 && model->fStatusEarth > MODEL_STATUS_EARTH_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletEarth(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletEarth(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackEarthCount = MODEL_ATTACK_EARTH;
				model->fStatusEarth -= MODEL_STATUS_EARTH_SUB;
				SetVoice(VOICE_EARTH, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		}
		// �}�W�b�N�T�[�N��������
		for (int j = 0; j < MAGICCIRCLE_MAX; j++)
		{
			model->magicCircle[j].bUse = false;
			ReleaseMagiccircle(j);
		}
		model->nChargeCount = 0;
		model->nCharge = 0;
		// �}�W�b�NSE
		SetSe(SE_EXPL045, E_DS8_FLAG_NONE, CONTINUITY_ON);
	}
	else
	{
		model->nAttackSpCount++;
		if (model->nAttackSpCount > PLAYER_RECOIL_TIME)
		{
			model->bAttackSp = false;
			model->nAttackSpCount = 0;
		}
	}
}

//=============================================================================
// �W�����v�����֐�
//=============================================================================
void JumpModel(int nModel)
{
	MODEL *model = &modelWk[nModel];

	// ���V�t�g�L�[�������͖��d��+�u�[�X�g
	if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
	{	// ���V�t�g������������MP���~�b�^�[������
		if (model->fStatusMP > MODEL_STATUS_MP_SUB)
		{
			if ((GetKeyboardTrigger(DIK_LSHIFT) || IsButtonTriggered(0, BUTTON_R)))
			{
				SetSe(SE_NOGRAVITY, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			model->fMoveSpeed = MODEL_MOVE_BOOST;				// �u�[�X�g�ړ����x
			model->fJumpAccel = 0.0f;							// �W�����v�A�N�Z����������
		}
	}
	else
	{														// ���V�t�g�������Ă��Ȃ�
		model->posModel.y -= GAME_GRAVITI;					// �d�͂�������
		model->fMoveSpeed = MODEL_MOVE_SPEED;				// �ʏ�ړ����x��K�p
		model->fJumpAccel -= MODEL_JUMP_POWER_DOWN;		// �W�����v�A�N�Z���Ɍ��Z
		if (model->fJumpAccel < 0.0f)
		{
			model->fJumpAccel = 0.0f;
		}
		if (!model->bJump)
		{													// �W�����v�t���O��false�Ȃ�
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, R_BUTTON_B))
			{												// �X�y�[�X�L�[�������ꂽ��
				model->fJumpAccel = MODEL_JUMP_POWER;		// �W�����v�p���[��K�p
				model->bJump = true;						// �W�����v�t���O��ݒ�
				SetSe(10, E_DS8_FLAG_NONE, true);
			}
			else
			{
				//FloatModel(nModel);							// ���V����
			}
		}
		else
		{
		}
	}

	// �W�����v��Y���ɔ��f
	model->posModel.y += model->fJumpAccel;

	if (model->posModel.y < 0.0f)
	{												// ���f��POS��0.0f�����������
		model->posModel.y = 0.0f;					// 0.0f��K�p
		model->bJump = false;						// �W�����v�t���O������
	}
}

//=============================================================================
// ���V�����֐�
//=============================================================================
void FloatModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	model->fFloatAngle += MODEL_FLOAT_SPEED;	// �p�x�l�����Z
	model->fFloatPos = MODEL_FLOAT_LENGTH		// sin�J�[�u���v�Z
		* sinf(PiCalculate360(model->fFloatAngle));
	model->posModel.y += model->fFloatPos;		// ���f���ɉ��Z
}

//=============================================================================
// �}�W�b�N�����֐�
//=============================================================================
void MagicRecoverModel(int nModel,int nMagic)
{
	MODEL *model = &modelWk[nModel];
	//if (model->modelMagic[nMagic].bUse)
	//{
	//	model->modelMagic[nMagic].fStatus += 0.0f;
	//}

	model->fStatusNormal += MODEL_STATUS_MAGIC_RECOVER;
	if (model->fStatusNormal > MODEL_STATUS_NORMAL) { model->fStatusNormal = MODEL_STATUS_NORMAL; };
	model->nAttackNormalCount--;

	if (model->bActiveFire)
	{
		model->fStatusFire += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusFire > MODEL_STATUS_FIRE) { model->fStatusFire = MODEL_STATUS_FIRE; };
		model->nAttackFireCount--;
	}

	if (model->bActiveThunder)
	{
		model->fStatusThunder += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusThunder > MODEL_STATUS_THUNDER) { model->fStatusThunder = MODEL_STATUS_THUNDER; };
		model->nAttackThunderCount--;
	}

	if (model->bActiveIce)
	{
		model->fStatusIce += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusIce > MODEL_STATUS_ICE) { model->fStatusIce = MODEL_STATUS_ICE; };
		model->nAttackIceCount--;
	}
	if (model->bActiveWind)
	{
		model->fStatusWind += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusWind > MODEL_STATUS_WIND) { model->fStatusWind = MODEL_STATUS_WIND; };
		model->nAttackWindCount--;
	}

	if (model->bActiveWater)
	{
		model->fStatusWater += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusWater > MODEL_STATUS_WARER) { model->fStatusWater = MODEL_STATUS_WARER; };
		model->nAttackWaterCount--;
	}
	if (model->bActiveEarth)
	{
		model->fStatusEarth += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusEarth > MODEL_STATUS_EARTH) { model->fStatusEarth = MODEL_STATUS_EARTH; };
		model->nAttackEarthCount--;
	}
}

//=============================================================================
// �}�W�b�N�����֐�
//=============================================================================
void SetMagicModel(int nModel, int nMagic)
{
	MODEL *model = &modelWk[nModel];
	switch (nMagic)
	{
	case BULLET_SPECIAL_FIRE:
		model->bActiveFire = true;
		model->fStatusFire = MODEL_STATUS_FIRE;
		break;
	case BULLET_SPECIAL_THUNDER:
		model->bActiveThunder = true;
		model->fStatusThunder = MODEL_STATUS_THUNDER;
		break;
	case BULLET_SPECIAL_ICE:
		model->bActiveIce = true;
		model->fStatusIce = MODEL_STATUS_ICE;
		break;
	case BULLET_SPECIAL_WIND:
		model->bActiveWind = true;
		model->fStatusWind = MODEL_STATUS_WIND;
		break;
	case BULLET_SPECIAL_WATER:
		model->bActiveWater = true;
		model->fStatusWater = MODEL_STATUS_WARER;
		break;
	case BULLET_SPECIAL_EARTH:
		model->bActiveEarth = true;
		model->fStatusEarth = MODEL_STATUS_EARTH;
		break;
	}
}

//=============================================================================
// �}�W�b�N�`���[�W�����֐�
//=============================================================================
void SetMagicChargeModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	model->nCharge++;
}

//=============================================================================
// �擾�֐�
//=============================================================================
MODEL *GetModel(int no)
{
	return(&modelWk[no]);
}