//=============================================================================
//
// ���f������ [skydome.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "skydome.h"
#include "main.h"
#include "input.h"
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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureSkydome;		// �e�N�X�`���ւ̃|�C���^

LPD3DXMESH			g_pD3DXMeshSkydome;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatSkydome;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatSkydome;			// �}�e���A�����̐�

SKYDOME				skydomeWk[SKYDOME_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkydome(int nType)
{
	SKYDOME *skydome = &skydomeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// �ʒu�E��]�E�X�P�[���̐ݒ�
	skydome->posSkydome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydome->rotSkydome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydome->sclSkydome = D3DXVECTOR3(SKYDOME_SCALE, SKYDOME_SCALE, SKYDOME_SCALE);

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureSkydome = NULL;
		g_pD3DXMeshSkydome = NULL;
		g_pD3DXBuffMatSkydome = NULL;
		g_nNumMatSkydome = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(SKYDOME_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatSkydome,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatSkydome,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshSkydome)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X
			SKYDOME_TEXTURE,	// �t�@�C����
			&g_pD3DTextureSkydome);		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkydome(void)
{
	if(g_pD3DTextureSkydome != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureSkydome->Release();
		g_pD3DTextureSkydome = NULL;
	}

	if(g_pD3DXMeshSkydome != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshSkydome->Release();
		g_pD3DXMeshSkydome = NULL;
	}

	if(g_pD3DXBuffMatSkydome != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatSkydome->Release();
		g_pD3DXBuffMatSkydome = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSkydome(void)
{
	SKYDOME *skydome = &skydomeWk[0];
	int pCameraMode = *GetCameraMode();
	CAMERA *camera = GetCamera(pCameraMode);

	if (pCameraMode == CAMERA_GAME)
	{
		skydome->posSkydome.x = camera->posCameraAt.x;
		skydome->posSkydome.y = camera->posCameraAt.y + SKYDOME_HEIGHT;
		skydome->posSkydome.z = camera->posCameraAt.z;
	}
	//skydome->rotSkydome.y += SKYDOME_ROT_SPEED;

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkydome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	SKYDOME *skydome = &skydomeWk[0];

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&skydome->mtxWorld);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, skydome->sclSkydome.x, skydome->sclSkydome.y, skydome->sclSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, skydome->rotSkydome.y, skydome->rotSkydome.x, skydome->rotSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, skydome->posSkydome.x, skydome->posSkydome.y, skydome->posSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &skydome->mtxWorld);


	/******************** �r���[�|�[�g�ϊ� ********************/
	// ���݂̃}�e���A����ۑ�
	pDevice->GetMaterial(&matDef);
	// �}�e���A�����ɑ΂���|�C���^�̎擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatSkydome->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatSkydome; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
		pDevice->SetTexture(0, g_pD3DTextureSkydome);
		// �`��
		g_pD3DXMeshSkydome->DrawSubset(i);
	}


	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �擾�֐�
//=============================================================================
SKYDOME *GetSkydome(int no)
{
	return(&skydomeWk[no]);
}