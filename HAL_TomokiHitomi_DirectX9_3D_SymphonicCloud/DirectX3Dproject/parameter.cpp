//=============================================================================
//
// �p�����[�^���� [parameter.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "parameter.h"
#include "sound.h"
#include "camera.h"

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
HRESULT MakeVertexParameter(LPDIRECT3DDEVICE9 pDevice);
void SetVtxParameter(int nParameter, float fSizeX, float fSizeY);
void SetDiffuseParameter(int nParameter, D3DXCOLOR col);
void SetTexParameter(int nParameter, int nTex);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PARAMETER				parameterWk[PARAMETER_MAX];

LPDIRECT3DTEXTURE9		g_pD3DTextureParameter = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParameter = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldParameter;			// ���[���h�}�g���b�N�X


//=============================================================================
// ����������
//=============================================================================
HRESULT InitParameter(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARAMETER *parameter = &parameterWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PARAMETER00,				// �t�@�C���̖��O
			&g_pD3DTextureParameter);			// �ǂݍ��ރ������̃|�C���^
	}

	// �p�����[�^�̏�����
	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{

		parameter->Texture = g_pD3DTextureParameter;			// �e�N�X�`���ւ̃G�l�~�[
		parameter->size = D3DXVECTOR2(TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y);
		parameter->vecChangeSize = D3DXVECTOR3(TEXTURE_CHANGE_PARAMETER_X, TEXTURE_CHANGE_PARAMETER_Y, 0.0f);

		InitStatusParameter(i);
	}

	// ���_���̍쐬
	MakeVertexParameter(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParameter(void)
{
	PARAMETER *parameter = &parameterWk[0];

	// ���������
	if (g_pD3DTextureParameter != NULL)
	{
		g_pD3DTextureParameter->Release();
		g_pD3DTextureParameter = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParameter(void)
{
	PARAMETER *parameter = &parameterWk[0];
	MODEL *model;
	ENEMY *enemy;

	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (parameter->bUse)
		{
			parameter->nPatternAnim = parameter->nType;
			switch (parameter->nType)
			{
			case 0:
				model = GetModel(parameter->nTarget);
				parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)MODEL_STATUS_HP * model->fStatusHP;
				parameter->posParameter.x = model->posModel.x;
				parameter->posParameter.y = model->posModel.y + PARAMETER_HEIGHT;
				parameter->posParameter.z = model->posModel.z;
				if (!model->bUse)
				{
					InitStatusParameter(i);
				}
				break;
			case 1:
				model = GetModel(parameter->nTarget);
				parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)MODEL_STATUS_HP * model->fStatusHP;
				parameter->posParameter.x = model->posModel.x;
				parameter->posParameter.y = model->posModel.y + PARAMETER_HEIGHT;
				parameter->posParameter.z = model->posModel.z;
				if (!model->bUse)
				{
					InitStatusParameter(i);
				}
				break;
			case 2:
				enemy = GetEnemy(parameter->nTarget);
				switch (enemy->nModel)
				{
				case ENEMY_TYPE_NORMAL:
					parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)ENEMY_STATUS_HP * enemy->fStatusHP;
					parameter->posParameter.x = enemy->posEnemy.x;
					parameter->posParameter.y = enemy->posEnemy.y + PARAMETER_HEIGHT;
					parameter->posParameter.z = enemy->posEnemy.z;
					break;
				case ENEMY_TYPE_BOSS:
					parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)ENEMY_STATUS_BOSS_HP * enemy->fStatusHP;
					parameter->posParameter.x = enemy->posEnemy.x;
					parameter->posParameter.y = enemy->posEnemy.y + PARAMETER_HEIGHT;
					parameter->posParameter.z = enemy->posEnemy.z;
					break;
				}
				if (!enemy->bUse)
				{
					InitStatusParameter(i);
				}
			}
#ifdef _DEBUG
			PrintDebugProc("para[%d]  pos[X:%f  Y:%f  Z:%f] Gauge[%f]\n", i, parameter->posParameter.x, parameter->posParameter.y, parameter->posParameter.z, parameter->fPointGauge);
#endif

			// �e�N�X�`�����W��ݒ�
			SetTexParameter(i, parameter->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVtxParameter(i, parameter->posParameter.x, parameter->posParameter.y);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParameter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	PARAMETER *parameter = &parameterWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (parameter->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParameter);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();
			// �r���{�[�h��
			g_mtxWorldParameter._11 = mtxView._11;
			g_mtxWorldParameter._12 = mtxView._21;
			g_mtxWorldParameter._13 = mtxView._31;
			g_mtxWorldParameter._21 = mtxView._12;
			g_mtxWorldParameter._22 = mtxView._22;
			g_mtxWorldParameter._23 = mtxView._32;
			g_mtxWorldParameter._31 = mtxView._13;
			g_mtxWorldParameter._32 = mtxView._23;
			g_mtxWorldParameter._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, parameter->sclParameter.x, parameter->sclParameter.y, parameter->sclParameter.z);
			D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bullet->rotParameter.y, bullet->rotParameter.x, bullet->rotParameter.z);
			//D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &GetInvRotateMat(bullet->posParameter));
			////D3DXMatrixMultiply(&bullet->mtxWorld, &bullet->mtxWorld, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, parameter->posParameter.x, parameter->posParameter.y, parameter->posParameter.z);
			D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParameter);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParameter, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureParameter);

			// �|���S���̕`��
			//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->vertexWK, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexParameter(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * PARAMETER_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffParameter,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < PARAMETER_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-TEXTURE_PARAMETER00_SIZE_X, -TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TEXTURE_PARAMETER00_SIZE_X, -TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y, 0.0f);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PARAMETER;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParameter->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxParameter(int nParameter, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		PARAMETER *parameter = &parameterWk[nParameter];

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2),
			-TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[1].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2),
			TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[2].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2) + parameter->fPointGauge * parameter->vecChangeSize.x,
			- TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[3].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2) + parameter->fPointGauge * parameter->vecChangeSize.x,
			TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);


		//pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		//pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		//pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		//pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseParameter(int nParameter, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexParameter(int nParameter, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PARAMETER;

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// �p�����[�^�̃Z�b�g
//=============================================================================
void SetParameter(int nType, D3DXVECTOR3 vecTargetPos, int nTarget)
{
	PARAMETER *parameter = &parameterWk[0];

	// ���g�p�̃p�����[�^�[��T��
	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (!parameter->bUse)
		{
			// �p�����[�^��L���ɂ���
			parameter->bUse = true;
			// �p�����[�^�^�C�v��ݒ�
			parameter->nType = nType;
			// �p�����[�^�ݒu�Ώۂ��L��
			parameter->vecTargetPos = vecTargetPos;
			// �p�����[�^�^�[�Q�b�g��ݒ�
			parameter->nTarget = nTarget;
			// �e�N�X�`�����W��ݒ�
			SetTexParameter(i, parameter->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVtxParameter(i, parameter->posParameter.x, parameter->posParameter.y);
			return;
		}
	}
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusParameter(int nParameter)
{
	PARAMETER *parameter = &parameterWk[nParameter];

	// �p�����[�^�X�e�[�^�X�̏�����
	parameter->bUse = false;

	parameter->posParameter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	parameter->sclParameter = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	parameter->sclParameter = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	parameter->vecTargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	parameter->nCountAnim = 0;
	parameter->nPatternAnim = 0;
	parameter->nTarget = 0;
	parameter->nType = 0;

	parameter->fPointGauge = 0.0f;
	return;}

//=============================================================================
// �p�����[�^�擾�֐�
//=============================================================================
PARAMETER *GetParameter(int no)
{
	return(&parameterWk[no]);
}

