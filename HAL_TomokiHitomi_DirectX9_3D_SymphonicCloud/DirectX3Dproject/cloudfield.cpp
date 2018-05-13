//=============================================================================
//
// ���f������ [cloudfield.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "cloudfield.h"
#include "main.h"
#include "input.h"
#include "camera.h"
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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureCloudfield;		// �e�N�X�`���ւ̃|�C���^

LPD3DXMESH			g_pD3DXMeshCloudfield;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatCloudfield;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatCloudfield;			// �}�e���A�����̐�

CLOUDFIELD				cloudfieldWk[CLOUDFIELD_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCloudfield(int nType)
{
	CLOUDFIELD *cloudfield = &cloudfieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// �ʒu�E��]�E�X�P�[���̐ݒ�
	cloudfield->posCloudfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cloudfield->rotCloudfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cloudfield->sclCloudfield = D3DXVECTOR3(CLOUDFIELD_SCALE, CLOUDFIELD_SCALE, CLOUDFIELD_SCALE);

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureCloudfield = NULL;
		g_pD3DXMeshCloudfield = NULL;
		g_pD3DXBuffMatCloudfield = NULL;
		g_nNumMatCloudfield = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(CLOUDFIELD_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatCloudfield,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatCloudfield,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshCloudfield)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			MessageBox(NULL, "cloudfield�̃��f��������܂���B", NULL, MB_OK);
			return E_FAIL;
		}

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X
			CLOUDFIELD_TEXTURE,			// �t�@�C����
			&g_pD3DTextureCloudfield)))	// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
		{
			MessageBox(NULL, "cloudfield�̃e�N�X�`��������܂���B", NULL, MB_OK);
			return E_FAIL;
		};
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCloudfield(void)
{
	if(g_pD3DTextureCloudfield != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureCloudfield->Release();
		g_pD3DTextureCloudfield = NULL;
	}

	if(g_pD3DXMeshCloudfield != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshCloudfield->Release();
		g_pD3DXMeshCloudfield = NULL;
	}

	if(g_pD3DXBuffMatCloudfield != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatCloudfield->Release();
		g_pD3DXBuffMatCloudfield = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCloudfield(void)
{
	//CLOUDFIELD *cloudfield = &cloudfieldWk[0];
	//int pCameraMode = *GetCameraMode();
	//CAMERA *camera = GetCamera(pCameraMode);

	//if (pCameraMode == CAMERA_GAME)
	//{
	//	cloudfield->posCloudfield.x = camera->posCameraAt.x;
	//	cloudfield->posCloudfield.y = camera->posCameraAt.y + CLOUDFIELD_HEIGHT;
	//	cloudfield->posCloudfield.z = camera->posCameraAt.z;
	//}
	////cloudfield->rotCloudfield.y += CLOUDFIELD_ROT_SPEED;

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCloudfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	CLOUDFIELD *cloudfield = &cloudfieldWk[0];

	//// ���e�X�g��L����
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//// ���C���e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	SetLight(LIGHT2, TRUE);

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&cloudfield->mtxWorld);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, cloudfield->sclCloudfield.x, cloudfield->sclCloudfield.y, cloudfield->sclCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, cloudfield->rotCloudfield.y, cloudfield->rotCloudfield.x, cloudfield->rotCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, cloudfield->posCloudfield.x, cloudfield->posCloudfield.y, cloudfield->posCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &cloudfield->mtxWorld);


	/******************** �r���[�|�[�g�ϊ� ********************/
	// ���݂̃}�e���A����ۑ�
	pDevice->GetMaterial(&matDef);
	// �}�e���A�����ɑ΂���|�C���^�̎擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatCloudfield->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatCloudfield; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
		pDevice->SetTexture(0, g_pD3DTextureCloudfield);
		// �`��
		g_pD3DXMeshCloudfield->DrawSubset(i);
	}


	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	//// ���C���e�B���O��L���ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// ���e�X�g�𖳌���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	SetLight(LIGHT2, FALSE);
}

//=============================================================================
// �擾�֐�
//=============================================================================
CLOUDFIELD *GetCloudfield(int no)
{
	return(&cloudfieldWk[no]);
}

//=============================================================================
//// ���b�V���擾�֐�
////=============================================================================
//LPD3DXMESH *GetCloudfieldMesh(void)
//{
//	return(&g_pD3DXMeshCloudfield);
//	g_pD3DXMeshCloudfield
//}

//=============================================================================
// ���b�V���擾�֐�
//=============================================================================
LPD3DXBASEMESH  GetCloudfieldMesh(void)
{
	LPD3DXBASEMESH lpBasemesh;
	lpBasemesh = g_pD3DXMeshCloudfield;
	return(lpBasemesh);
}