//=============================================================================
//
// �E�H�[������ [wall.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "camera.h"
#include "wall.h"
#include "input.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureWall = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

WALL					wallWk[WALL_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitWall(int nType)
{
	WALL *wall = &wallWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_WALL,
			&g_pD3DTextureWall);
	}

	for (int i = 0; i < WALL_Y; i++)
	{
		for (int j = 0; j < WALL_X; j++, wall++)
		{
			// �ʒu�E��]�E�X�P�[���̐ݒ�
			wall->posWall = D3DXVECTOR3(0.0f + j * WALL_SIZE_X * 2, WALL_SIZE_Z + (i * WALL_SIZE_Z * 2), -WALL_SIZE_Z);
			wall->rotWall = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
			wall->sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexWall(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWall(void)
{
	if(g_pD3DTextureWall != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureWall->Release();
		g_pD3DTextureWall = NULL;
	}

	if(g_pD3DVtxBuffWall != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffWall->Release();
		g_pD3DVtxBuffWall = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWall(void)
{
	WALL *wall = &wallWk[0];
	//for (int i = 0; i < WALL_MAX; i++, wall++)
	//{
	//	// �����ړ�
	//	if (GetKeyboardPress(DIK_SPACE))
	//	{
	//		wall->posWall.y += 5;
	//	}

	//	if (GetKeyboardPress(DIK_LSHIFT))
	//	{
	//		wall->posWall.y -= 5;
	//	}


	//	// Y����]
	//	if (GetKeyboardPress(DIK_Q))
	//	{
	//		wall->rotWall.y += 0.05;
	//	}
	//	// Y�E�ړ�
	//	else if (GetKeyboardPress(DIK_E))
	//	{
	//		wall->rotWall.y -= 0.05;
	//	}


	//	// �g��
	//	if (GetKeyboardPress(DIK_Z))
	//	{
	//		wall->sclWall += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//	// �k��
	//	else if (GetKeyboardPress(DIK_X))
	//	{
	//		wall->sclWall -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	WALL *wall = &wallWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);


	for (int i = 0; i < WALL_MAX; i++, wall++)
	{
		/******************** ���[���h�ϊ� ********************/
		// ���[���h�}�g���N�X�̏�����
		D3DXMatrixIdentity(&wall->mtxWorld);

		// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
		D3DXMatrixScaling(&mtxScl, wall->sclWall.x, wall->sclWall.y, wall->sclWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxScl);

		// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, wall->rotWall.y, wall->rotWall.x, wall->rotWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxRot);

		// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
		D3DXMatrixTranslation(&mtxTranslate, wall->posWall.x, wall->posWall.y, wall->posWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &wall->mtxWorld);


		/******************** �r���[�|�[�g�ϊ� ********************/
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureWall);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	WALL *wall = &wallWk[0];

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffWall,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-WALL_SIZE_X, 0.0f, WALL_SIZE_Z);
		pVtx[1].vtx = D3DXVECTOR3(WALL_SIZE_X, 0.0f, WALL_SIZE_Z);
		pVtx[2].vtx = D3DXVECTOR3(-WALL_SIZE_X, 0.0f, -WALL_SIZE_Z);
		pVtx[3].vtx = D3DXVECTOR3(WALL_SIZE_X, 0.0f, -WALL_SIZE_Z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffWall->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �擾�֐�
//=============================================================================
WALL *GetWall(int no)
{
	return(&wallWk[no]);
}