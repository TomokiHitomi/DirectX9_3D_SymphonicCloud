//=============================================================================
//
// Lockon���� [lockon.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "lockon.h"
#include "model.h"
#include "enemy.h"
#include "camera.h"
#include "calculate.h"
#include "checkhit.h"
#include "magic.h"

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
HRESULT MakeVertexLockon(LPDIRECT3DDEVICE9 pDevice);
void SetVtxLockon(int nLockon);
void SetDiffuseLockon(int nLockon, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLockon = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLockon = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

LOCKON					lockonWk[LOCKON_MAX];
D3DXMATRIX				g_mtxWorldLockon;			// ���[���h�}�g���b�N�X

float					g_fBaseAngleLockon;			// �p�x�i�Ɓj
float					g_fRadiusLockon;			// ���a
int						g_nLockonMagicType;			// �}�W�b�N�^�C�v�̃��O
//=============================================================================
// ����������
//=============================================================================
HRESULT InitLockon(int nType)
{
	LOCKON *lockon = &lockonWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_LOCKON,
			&g_pD3DTextureLockon);
	}

	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		InitStatusLockon(i);
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexLockon(pDevice);


	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusLockon(int nLockon)
{
	LOCKON *lockon = &lockonWk[nLockon];

	// �ʒu�E��]�E�X�P�[���̐ݒ�
	lockon->posLockon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lockon->rotLockon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	lockon->bUse = false;
	lockon->bRelease = false;
	lockon->vec2Size = D3DXVECTOR2(LOCKON_SIZE_X, LOCKON_SIZE_Y);
	lockon->colorLockon = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	lockon->nTex = 0;
	lockon->nIndex = 0;
	lockon->nUseCount = 0;
	lockon->nReleaseCount = 0;
	lockon->nCoolDown = 0;
	lockon->fSizeChange = 1.0f;
	lockon->fAlphaChange = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLockon(void)
{
	if (g_pD3DTextureLockon != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureLockon->Release();
		g_pD3DTextureLockon = NULL;
	}

	if (g_pD3DVtxBuffLockon != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffLockon->Release();
		g_pD3DVtxBuffLockon = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLockon(void)
{
	LOCKON *lockon = &lockonWk[0];

#ifdef _DEBUG
	PrintDebugProc("�y LOCKON �z\n");
#endif

	// �}�W�b�N�^�C�v�ύX���̓��b�N�I������x�����[�X
	//int nMagicType = GetTypeMagic();
	//bool bRelease;
	//if (g_nLockonMagicType == nMagicType)
	//{
	//	bRelease = false;
	//}
	//else
	//{
	//	bRelease = true;
	//}
	//g_nLockonMagicType = nMagicType;



	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		lockon->nCoolDown--;
		if (lockon->bUse)
		{
			if (lockon->nReleaseCount == LOCKON_RELEASE_COUNT)
			{
				ReleaseLockon(i);
			}

			if (lockon->bRelease)
			{
				lockon->fSizeChange += LOCKON_SCALE_CHANGE;
				lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * lockon->fSizeChange;
				if (lockon->fSizeChange > LOCKON_SCALE_MAX)
				{
					InitStatusLockon(i);
					lockon->nCoolDown = LOCKON_COOLDOWN;
				}
			}
			else
			{
				if (lockon->fSizeChange > LOCKON_SCALE_MIN)
				{
					lockon->fSizeChange -= LOCKON_SCALE_CHANGE;
					lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * lockon->fSizeChange;
				}
			}

			lockon->rotLockon.z += 0.02f;
			lockon->rotLockon.z = PiCalculate360(lockon->rotLockon.z);
			SetVtxLockon(i);
			SetDiffuseLockon(i, GetMagicColor());			// �}�W�b�N�̃J���[���擾���ēK�p

			// �f�o�b�O�p
#ifdef _DEBUG
			PrintDebugProc("Lockon[%d]  rotZ:%f  Angle:%f  Len:%f\n", i,lockon->rotLockon.z, g_fBaseAngleLockon, g_fRadiusLockon);
#endif
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLockon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// �X�P�[��, ��], ���s�ړ�

	LOCKON *lockon = &lockonWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, LOCKON_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		if (lockon->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldLockon);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();
			// �r���{�[�h��
			g_mtxWorldLockon._11 = mtxView._11;
			g_mtxWorldLockon._12 = mtxView._21;
			g_mtxWorldLockon._13 = mtxView._31;
			g_mtxWorldLockon._21 = mtxView._12;
			g_mtxWorldLockon._22 = mtxView._22;
			g_mtxWorldLockon._23 = mtxView._32;
			g_mtxWorldLockon._31 = mtxView._13;
			g_mtxWorldLockon._32 = mtxView._23;
			g_mtxWorldLockon._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, lockon->sclLockon.x, lockon->sclLockon.y, lockon->sclLockon.z);
			D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, lockon->rotLockon.y, lockon->rotLockon.x, lockon->rotLockon.z);
			//D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &GetInvRotateMat(lockon->posLockon));
			////D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, lockon->posLockon.x, lockon->posLockon.y, lockon->posLockon.z);
			D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLockon);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffLockon, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureLockon);

			// �|���S���̕`��
			//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, lockon->vertexWK, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
			//pDevice->DrawPrimitive(D3DPT_POINTLIST, (i * 4), NUM_POLYGON);


		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexLockon(LPDIRECT3DDEVICE9 pDevice)
{
	// �p�x�Ɣ��a�����߂�
	g_fBaseAngleLockon = atan2f(LOCKON_SIZE_Y, LOCKON_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(LOCKON_SIZE_X, LOCKON_SIZE_Y);
	g_fRadiusLockon = D3DXVec2Length(&temp);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * LOCKON_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffLockon,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < LOCKON_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LOCKON_SIZE_X, -LOCKON_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-LOCKON_SIZE_X, LOCKON_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(LOCKON_SIZE_X, -LOCKON_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(LOCKON_SIZE_X, LOCKON_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_LOCKON;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffLockon->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxLockon(int nLockon)
{
	LOCKON *lockon = &lockonWk[nLockon];

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nLockon * 4);

		// ���_0:��]����
		pVtx[0].vtx.x = 0.0f - cosf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[0].vtx.y = 0.0f - sinf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[0].vtx.z = 0.0f;

		// ���_1:��]����
		pVtx[1].vtx.x = 0.0f - cosf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[1].vtx.y = 0.0f + sinf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[1].vtx.z = 0.0f;

		// ���_2:��]����
		pVtx[2].vtx.x = 0.0f + cosf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[2].vtx.y = 0.0f - sinf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[2].vtx.z = 0.0f;

		// ���_3:��]����
		pVtx[3].vtx.x = 0.0f + cosf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[3].vtx.y = 0.0f + sinf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[3].vtx.z = 0.0f;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffLockon->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseLockon(int nLockon, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nLockon * 4);

		// ���_���W�̐ݒ�
			pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffLockon->Unlock();
	}
}

//=============================================================================
// �Ώۂɐݒu
//=============================================================================
int SetLockon(D3DXVECTOR3 vecPos)
{
	LOCKON *lockon = &lockonWk[0];

	// ���g�p��T��
	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		if (!lockon->bUse && lockon->nCoolDown <= 0)
		{
			lockon->bUse = true;
			lockon->posLockon = vecPos;
			return i;
		}
	}
}

//=============================================================================
// �Ώۍ��W�Ɉړ�
//=============================================================================
void SetPosLockon(int nIndex, D3DXVECTOR3 vecPos)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->posLockon = vecPos;
}

//=============================================================================
// ���g�p����
//=============================================================================
void ReleaseLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->bRelease = true;
}

//=============================================================================
// �����[�X�J�E���g���Z����
//=============================================================================
bool AddReleaseCountLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->nReleaseCount++;
	if (lockon->nReleaseCount > LOCKON_RELEASE_COUNT)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=============================================================================
// �����[�X�J�E���g����������
//=============================================================================
void ResetReleaseCountLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->nReleaseCount = 0;
}

//=============================================================================
// �擾�֐�
//=============================================================================
LOCKON *GetLockon(int no)
{
	return(&lockonWk[no]);
}