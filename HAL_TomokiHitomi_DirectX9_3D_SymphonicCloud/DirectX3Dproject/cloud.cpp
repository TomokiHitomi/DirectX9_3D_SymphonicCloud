//=============================================================================
//
// Cloud���� [cloud.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "camera.h"
#include "cloud.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCloud(LPDIRECT3DDEVICE9 pDevice);
void SetVtxCloud(int nCloud, float fSizeX, float fSizeY);
void SetDiffuseCloud(int nCloud, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureCloud = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffCloud = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

CLOUD					cloudWk[CLOUD_MAX];
D3DXMATRIX				g_mtxWorldCloud;			// ���[���h�}�g���b�N�X


//=============================================================================
// ����������
//=============================================================================
HRESULT InitCloud(int nType)
{
	CLOUD *cloud = &cloudWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_CLOUD,
			&g_pD3DTextureCloud);
	}

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		cloud->posCloud = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cloud->rotCloud = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cloud->sclCloud = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		InitStatusCloud(i);

	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexCloud(pDevice);


	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusCloud(int nCloud)
{
	CLOUD *cloud = &cloudWk[nCloud];

	cloud->bUse = false;
	cloud->vec2Size = D3DXVECTOR2(CLOUD_SIZE_X, CLOUD_SIZE_Y);
	cloud->colorCloud = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	cloud->nTex = 0;
	cloud->fSizeChange = 0.0f;
	cloud->fAlphaChange = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCloud(void)
{
	if (g_pD3DTextureCloud != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureCloud->Release();
		g_pD3DTextureCloud = NULL;
	}

	if (g_pD3DVtxBuffCloud != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffCloud->Release();
		g_pD3DVtxBuffCloud = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCloud(void)
{
	CLOUD *cloud = &cloudWk[0];
	MODEL *model = GetModel(0);

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (cloud->bUse)
		{
			cloud->colorCloud.a -= CLOUD_ALPHA_CHANGE;
			cloud->vec2Size -= D3DXVECTOR2(CLOUD_SIZE_CHANGE, CLOUD_SIZE_CHANGE);

			if (cloud->vec2Size.x <= 0.0f || cloud->vec2Size.y <= 0.0f)
			{
				cloud->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
			}
			if (cloud->colorCloud.a <= 0.0f)
			{
				cloud->colorCloud.a = 0.0f;
			}

			if (cloud->colorCloud.a <= 0.0f)
			{
				InitStatusCloud(i);
			}

			SetVtxCloud(i, cloud->vec2Size.x, cloud->vec2Size.y);
			SetDiffuseCloud(i, cloud->colorCloud);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// �X�P�[��, ��], ���s�ړ�

	CLOUD *cloud = &cloudWk[0];

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (cloud->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldCloud);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();
			// �r���{�[�h��
			g_mtxWorldCloud._11 = mtxView._11;
			g_mtxWorldCloud._12 = mtxView._21;
			g_mtxWorldCloud._13 = mtxView._31;
			g_mtxWorldCloud._21 = mtxView._12;
			g_mtxWorldCloud._22 = mtxView._22;
			g_mtxWorldCloud._23 = mtxView._32;
			g_mtxWorldCloud._31 = mtxView._13;
			g_mtxWorldCloud._32 = mtxView._23;
			g_mtxWorldCloud._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, cloud->sclCloud.x, cloud->sclCloud.y, cloud->sclCloud.z);
			D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, cloud->rotCloud.y, cloud->rotCloud.x, cloud->rotCloud.z);
			//D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &GetInvRotateMat(cloud->posCloud));
			////D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, cloud->posCloud.x, cloud->posCloud.y, cloud->posCloud.z);
			D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCloud);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffCloud, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureCloud);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCloud(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * CLOUD_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffCloud,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < CLOUD_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-CLOUD_SIZE_X, -CLOUD_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-CLOUD_SIZE_X, CLOUD_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(CLOUD_SIZE_X, -CLOUD_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(CLOUD_SIZE_X, CLOUD_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_CLOUD;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCloud->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxCloud(int nCloud, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCloud * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCloud->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseCloud(int nCloud, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCloud * 4);

		// ���_���W�̐ݒ�
			pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCloud->Unlock();
	}
}

//=============================================================================
// �Ώۂɐݒu
//=============================================================================
void SetCloud(D3DXVECTOR3 vecPos)
{
	CLOUD *cloud = &cloudWk[0];

	// ���g�p��T��
	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (!cloud->bUse)
		{
			cloud->bUse = true;
			cloud->posCloud = vecPos;
			return;
		}
	}
}
//=============================================================================
// �擾�֐�
//=============================================================================
CLOUD *GetCloud(int no)
{
	return(&cloudWk[no]);
}