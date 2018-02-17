//=============================================================================
//
// ���f������ [skydomeeffect.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "skydomeeffect.h"
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
LPDIRECT3DTEXTURE9	g_pD3DTextureSkydomeeffect;		// �e�N�X�`���ւ̃|�C���^

LPD3DXMESH			g_pD3DXMeshSkydomeeffect;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatSkydomeeffect;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatSkydomeeffect;			// �}�e���A�����̐�

SKYDOMEEFFECT				skydomeeffectWk[SKYDOMEEFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkydomeeffect(int nType)
{
	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// �ʒu�E��]�E�X�P�[���̐ݒ�
	skydomeeffect->posSkydomeeffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydomeeffect->rotSkydomeeffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydomeeffect->sclSkydomeeffect = D3DXVECTOR3(SKYDOMEEFFECT_SCALE, SKYDOMEEFFECT_SCALE, SKYDOMEEFFECT_SCALE);

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureSkydomeeffect = NULL;
		g_pD3DXMeshSkydomeeffect = NULL;
		g_pD3DXBuffMatSkydomeeffect = NULL;
		g_nNumMatSkydomeeffect = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(SKYDOMEEFFECT_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatSkydomeeffect,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatSkydomeeffect,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshSkydomeeffect)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X
			SKYDOMEEFFECT_TEXTURE,	// �t�@�C����
			&g_pD3DTextureSkydomeeffect);		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkydomeeffect(void)
{
	if(g_pD3DTextureSkydomeeffect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureSkydomeeffect->Release();
		g_pD3DTextureSkydomeeffect = NULL;
	}

	if(g_pD3DXMeshSkydomeeffect != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshSkydomeeffect->Release();
		g_pD3DXMeshSkydomeeffect = NULL;
	}

	if(g_pD3DXBuffMatSkydomeeffect != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatSkydomeeffect->Release();
		g_pD3DXBuffMatSkydomeeffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSkydomeeffect(void)
{
	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];
	int pCameraMode = *GetCameraMode();
	CAMERA *camera = GetCamera(pCameraMode);
	if (pCameraMode == CAMERA_GAME)
	{
		skydomeeffect->posSkydomeeffect.x = camera->posCameraAt.x;
		skydomeeffect->posSkydomeeffect.y = SKYDOMEEFFECT_HEIGHT;
		skydomeeffect->posSkydomeeffect.z = camera->posCameraAt.z;
	}
	skydomeeffect->rotSkydomeeffect.y += SKYDOMEEFFECT_ROT_SPEED;

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkydomeeffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&skydomeeffect->mtxWorld);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, skydomeeffect->sclSkydomeeffect.x, skydomeeffect->sclSkydomeeffect.y, skydomeeffect->sclSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, skydomeeffect->rotSkydomeeffect.y, skydomeeffect->rotSkydomeeffect.x, skydomeeffect->rotSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, skydomeeffect->posSkydomeeffect.x, skydomeeffect->posSkydomeeffect.y, skydomeeffect->posSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &skydomeeffect->mtxWorld);


	/******************** �r���[�|�[�g�ϊ� ********************/
	// ���݂̃}�e���A����ۑ�
	pDevice->GetMaterial(&matDef);
	// �}�e���A�����ɑ΂���|�C���^�̎擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatSkydomeeffect->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatSkydomeeffect; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
		pDevice->SetTexture(0, g_pD3DTextureSkydomeeffect);
		// �`��
		g_pD3DXMeshSkydomeeffect->DrawSubset(i);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �擾�֐�
//=============================================================================
SKYDOMEEFFECT *GetSkydomeeffect(int no)
{
	return(&skydomeeffectWk[no]);
}