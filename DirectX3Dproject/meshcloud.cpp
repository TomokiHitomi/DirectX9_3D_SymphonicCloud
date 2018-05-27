//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshcloud.cpp]
// Author :  GP12A295 25 �l���F��
//
//=============================================================================
#include "meshcloud.h"
#include "input.h"
#include "calculate.h"
#include "model.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				g_pD3DTextureMeshCloud;		// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9			g_pD3DVtxBuffMeshCloud;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9			g_pD3DIdxBuffMeshCloud;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

MESHCLOUD				meshcloudWk[MESHCLOUD_MAX];

int g_nNumBlockXCloud,			g_nNumBlockZCloud;		// �u���b�N��
int g_nNumVertexCloud;									// �����_��	
int g_nNumVertexIndexCloud;								// ���C���f�b�N�X��
int g_nNumPolygonCloud;									// ���|���S����
float g_fBlockSizeXCloud,			g_fBlockSizeZCloud;		// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshcloud(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHCLOUD *meshcloud = &meshcloudWk[0];

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// �|���S���\���ʒu�̒��S���W��ݒ�
		meshcloud->vecPos = pos;
		if (i == 1)
		{
			meshcloud->vecRot = rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		}
		else
		{
			meshcloud->vecRot = rot;
		}
		meshcloud->fVAnglel;
	}

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&g_pD3DTextureMeshCloud);	// �ǂݍ��ރ������[
	}

	// �u���b�N���̐ݒ�
	g_nNumBlockXCloud = nNumBlockX;
	g_nNumBlockZCloud = nNumBlockZ;

	// ���_���̐ݒ�
	g_nNumVertexCloud = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexCloud = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	g_nNumPolygonCloud = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	g_fBlockSizeXCloud = fBlockSizeX;
	g_fBlockSizeZCloud = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCloud,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffMeshCloud,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexCloud,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DIdxBuffMeshCloud,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffMeshCloud->Lock( 0, 0, (void**)&meshcloud->vertexWK, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZCloud + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXCloud / 2.0f) * g_fBlockSizeXCloud + nCntVtxX * g_fBlockSizeXCloud;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = 0.0f;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.z = (g_nNumBlockZCloud / 2.0f) * g_fBlockSizeZCloud - nCntVtxZ * g_fBlockSizeZCloud;

				// �@���̐ݒ�
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].diffuse = D3DXCOLOR(MESHCLOUD_DIFFUSE_R, MESHCLOUD_DIFFUSE_G, MESHCLOUD_DIFFUSE_B, MESHCLOUD_DIFFUSE_A);

				// �e�N�X�`�����W�̐ݒ�
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX / 5;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ / 5;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffMeshCloud->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DIdxBuffMeshCloud->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZCloud; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXCloud + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXCloud + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (g_nNumBlockZCloud - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXCloud + 1) + g_nNumBlockXCloud;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pD3DIdxBuffMeshCloud->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���W����������
//=============================================================================
HRESULT InitMeshcloudPos(void)
{
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	meshcloud->vecPos = D3DXVECTOR3(MESHCLOUD_POS_X, 0.0f, MESHCLOUD_POS_Z);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshcloud(void)
{
	if(g_pD3DVtxBuffMeshCloud)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffMeshCloud->Release();
		g_pD3DVtxBuffMeshCloud = NULL;
	}

	if(g_pD3DIdxBuffMeshCloud)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pD3DIdxBuffMeshCloud->Release();
		g_pD3DIdxBuffMeshCloud = NULL;
	}

	if(g_pD3DTextureMeshCloud)
	{// �e�N�X�`���̊J��
		g_pD3DTextureMeshCloud->Release();
		g_pD3DTextureMeshCloud = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshcloud(void)
{
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	MODEL *model = GetModel(0);

	if (model->posModel.x - meshcloud->vecPos.x > MESHCLOUD_RESET_X || meshcloud->vecPos.x - model->posModel.x > MESHCLOUD_RESET_X)
	{
		meshcloud->vecPos.x = model->posModel.x;
	}
	if (model->posModel.z - meshcloud->vecPos.z > MESHCLOUD_RESET_Z || meshcloud->vecPos.z - model->posModel.z> MESHCLOUD_RESET_Z)
	{
		meshcloud->vecPos.z = model->posModel.z;
	}

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffMeshCloud->Lock(0, 0, (void**)&meshcloud->vertexWK, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZCloud + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXCloud / 2.0f) * g_fBlockSizeXCloud + nCntVtxX * g_fBlockSizeXCloud;
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = 0.0f;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = sinf(meshcloud->fVAnglel) *MESHCLOUD_WAVE;
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.z = (g_nNumBlockZCloud / 2.0f) * g_fBlockSizeZCloud - nCntVtxZ * g_fBlockSizeZCloud;
				meshcloud->fVAnglel += MESHCLOUD_WAVE_SPEED;
				meshcloud->fVAnglel = PiCalculate180(meshcloud->fVAnglel);
			}
			meshcloud->fVAnglel += MESHCLOUD_WAVE_SPEED;
			meshcloud->fVAnglel = PiCalculate180(meshcloud->fVAnglel);
			// ���ˌ��̐ݒ�

		}
		//for (int i = 0; i < (MESHCLOUD_X + 1)*(MESHCLOUD_Z + 1); i++)
		//{
		//	g_pVtx[i].vtx.y = sinf(fVAnglel) *MESHCLOUD_WAVE_UPDATE;
		//	fVAnglel += MESHCLOUD_WAVE_SPEED_UPDATE;
		//	fVAnglel = PiCalculate360(fVAnglel);
		//}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffMeshCloud->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshcloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	D3DXMATRIX mtxRot, mtxTranslate;

	//// ���C���e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&meshcloud->mtxWorldCloud);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, meshcloud->vecRot.y, meshcloud->vecRot.x, meshcloud->vecRot.z);
		D3DXMatrixMultiply(&meshcloud->mtxWorldCloud, &meshcloud->mtxWorldCloud, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, meshcloud->vecPos.x, meshcloud->vecPos.y, meshcloud->vecPos.z);
		D3DXMatrixMultiply(&meshcloud->mtxWorldCloud, &meshcloud->mtxWorldCloud, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &meshcloud->mtxWorldCloud);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, g_pD3DVtxBuffMeshCloud, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(g_pD3DIdxBuffMeshCloud);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureMeshCloud);

		// �|���S���̕`��
		//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_pVtx, sizeof(VERTEX_3D));
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexCloud, 0, g_nNumPolygonCloud);
	}

	//// ���C���e�B���O��L���ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

