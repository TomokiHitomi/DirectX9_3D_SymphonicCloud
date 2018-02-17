//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#include "enemy.h"
#include "magiccircle.h"
#include "magic.h"
#include "main.h"
#include "calculate.h"
#include "model.h"
#include "sound.h"

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
HRESULT ReInitMagiccircle(int nType);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureMagiccircle;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshMagiccircle;

LPD3DXMESH			g_pD3DXMeshMagiccircle;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatMagiccircle;		// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatMagiccircle;			// �}�e���A�����̐�
D3DXMATRIX			g_mtxWorldMagiccircle;			// ���[���h�}�g���b�N�X

MAGICCIRCLE		magiccircleWk[MAGICCIRCLE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMagiccircle(int nType)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		ReInitMagiccircle(i);
	}

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureMagiccircle = NULL;
		g_pD3DXMeshMagiccircle = NULL;
		g_pD3DXBuffMatMagiccircle = NULL;
		g_nNumMatMagiccircle = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MAGICCIRCLE_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatMagiccircle,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatMagiccircle,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshMagiccircle)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �}�e���A���������o��
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatMagiccircle->GetBufferPointer();
		g_pD3DMatMeshMagiccircle = new D3DMATERIAL9[g_nNumMatMagiccircle];		// ���b�V�������m��
		g_pD3DTextureMagiccircle = new LPDIRECT3DTEXTURE9[g_nNumMatMagiccircle];// �e�N�X�`�����m��
		for (int i = 0; i < g_nNumMatMagiccircle; i++)
		{
			g_pD3DMatMeshMagiccircle[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
			g_pD3DMatMeshMagiccircle[i].Ambient = g_pD3DMatMeshMagiccircle[i].Diffuse;// ����������
			g_pD3DTextureMagiccircle[i] = NULL;	// �e�N�X�`��������

			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// �e�N�X�`���ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureMagiccircle[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �ď���������
//=============================================================================
HRESULT ReInitMagiccircle(int nType)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nType];
	magiccircle->bUse = false;
	magiccircle->bSe = false;
	magiccircle->pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
	magiccircle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//float fRotTemp = D3DX_PI * 2.0f / MAGICCIRCLE_MAX * nType;
	//magiccircle->rot = D3DXVECTOR3(fRotTemp, fRotTemp, fRotTemp);
	magiccircle->scl = D3DXVECTOR3(0.0f, MAGICCIRCLE_SCALE_Y, 0.0f);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagiccircle(void)
{
	for (int i = 0; i < g_nNumMatMagiccircle; i++)
	{	// �e�N�X�`���̊J��
		if (g_pD3DTextureMagiccircle[i] != NULL)
		{
			g_pD3DTextureMagiccircle[i]->Release();
			g_pD3DTextureMagiccircle[i] = NULL;
		}
	}

	if(g_pD3DXMeshMagiccircle != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshMagiccircle->Release();
		g_pD3DXMeshMagiccircle = NULL;
	}

	if(g_pD3DXBuffMatMagiccircle != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatMagiccircle->Release();
		g_pD3DXBuffMatMagiccircle = NULL;
	}

	delete[]g_pD3DMatMeshMagiccircle;
	delete[]g_pD3DTextureMagiccircle;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagiccircle(void)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (magiccircle->bUse)
		{
			// �X�P�[���`�F���W
			if (magiccircle->scl.x < MAGICCIRCLE_SCALE || magiccircle->scl.z < MAGICCIRCLE_SCALE)
			{
				magiccircle->scl.x += MAGICCIRCLE_SCALE_CHANGE;
				magiccircle->scl.z += MAGICCIRCLE_SCALE_CHANGE;
				if (magiccircle->scl.x > MAGICCIRCLE_SCALE || magiccircle->scl.z > MAGICCIRCLE_SCALE)
				{
					magiccircle->scl = D3DXVECTOR3(MAGICCIRCLE_SCALE, MAGICCIRCLE_SCALE_Y, MAGICCIRCLE_SCALE);
					SetMagicChargeModel(0);
					if (!magiccircle->bSe)
					{
						if (i == MAGICCIRCLE_MAX - 1)
						{
							SetSe(SE_CLAP009, E_DS8_FLAG_NONE, CONTINUITY_ON);
						}
						else
						{
							SetSe(SE_OPENHAT, E_DS8_FLAG_NONE, CONTINUITY_ON);
						}
						magiccircle->bSe = true;
					}
				}
			}

			// ��]����
			if (i % 2 == 0)
			{
				//magiccircle->rot.x += MAGICCIRCLE_ROT;
				magiccircle->rot.y += MAGICCIRCLE_ROT_Y;
				//magiccircle->rot.z += MAGICCIRCLE_ROT;
			}
			else
			{
				//magiccircle->rot.x -= MAGICCIRCLE_ROT;
				magiccircle->rot.y -= MAGICCIRCLE_ROT_Y;
				//magiccircle->rot.z -= MAGICCIRCLE_ROT;
			}
			// ��]�ʒ���
			//magiccircle->rot.x = PiCalculate180(magiccircle->rot.y);
			magiccircle->rot.y = PiCalculate180(magiccircle->rot.y);
			//magiccircle->rot.z = PiCalculate180(magiccircle->rot.y);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagiccircle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	MAGICCIRCLE *magiccircle = &magiccircleWk[0];

	//// ���C���e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, MAGICCIRCLE_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���ʕ\��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	D3DXCOLOR colorTemp = GetMagicColor();
	//colorTemp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_pD3DMatMeshMagiccircle[0].Diffuse = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshMagiccircle[0].Ambient = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshMagiccircle[0].Specular = (D3DCOLORVALUE)colorTemp;
	colorTemp *= 0.5f;
	g_pD3DMatMeshMagiccircle[0].Emissive = (D3DCOLORVALUE)colorTemp;

	//g_pD3DMatMeshMagiccircle[0].Power = 3.0f;


	//g_pD3DMatMeshMagiccircle->Emissive.r = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.g = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.b = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.a = 1.0f;

	//g_pD3DMatMeshMagiccircle->Diffuse.r = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.g = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.b = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.a = 1.0f;

	//DWORD dwTemp[3] = { 0,0,0 };

	//pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwTemp[0]);
	//pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTemp[1]);
	//pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwTemp[2]);

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (magiccircle->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldMagiccircle);

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, magiccircle->scl.x, magiccircle->scl.y, magiccircle->scl.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxScl);

			// �yR�z��1�]�𔽉f(YawPitchRoll��y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, magiccircle->rot.y, magiccircle->rot.x, magiccircle->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, magiccircle->pos.x, magiccircle->pos.y, magiccircle->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMagiccircle);

			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);
			//// �}�e���A�����ɑ΂���|�C���^�̎擾
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatMagiccircle->GetBufferPointer();

			for (int j = 0; j < g_nNumMatMagiccircle; j++)
			{
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_pD3DMatMeshMagiccircle[j]);

				// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
				//pDevice->SetTexture(0, NULL);

				pDevice->SetTexture(0, g_pD3DTextureMagiccircle[j]);

				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


				
				// �`��
				g_pD3DXMeshMagiccircle->DrawSubset(j);
			}
			// �}�e���A�������ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwTemp[0]);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwTemp[1]);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwTemp[2]);

	// �Жʕ\��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//// ���C���e�B���O��L���ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// �o������
//=============================================================================
int SetMagiccircle(void)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];
	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (!magiccircle->bUse)
		{
			magiccircle->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void SetMagiccirclePos(int nIndex, D3DXVECTOR3 pos, float fRotY)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nIndex];
	magiccircle->pos = pos;
	magiccircle->pos.y += MAGICCIRCLE_HEIGHT + nIndex * MAGICCIRCLE_HEIGHT_INTERVAL;
	magiccircle->rot.y += fRotY;
}

//=============================================================================
// �������
//=============================================================================
void ReleaseMagiccircle(int nIndex)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nIndex];
	ReInitMagiccircle(nIndex);
}

//=============================================================================
// �擾�֐�
//=============================================================================
MAGICCIRCLE *GetMagiccircle(int no)
{
	return(&magiccircleWk[no]);
}
