//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP12A295 25 �l���F�� 
//
//=============================================================================
#include "enemy.h"
#include "enemy_normal.h"
#include "main.h"
#include "calculate.h"
#include "magic.h"
#include "calculate.h"
#include "bullet.h"

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
LPDIRECT3DTEXTURE9	*g_pD3DTextureEnemy_normal;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshEnemy_normal;

LPD3DXMESH			g_pD3DXMeshEnemy_normal;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatEnemy_normal;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatEnemy_normal;			// �}�e���A�����̐�
D3DXMATRIX			g_mtxWorldEnemy_normal;			// ���[���h�}�g���b�N�X

ENEMY_NORMAL		enemy_normalWk[ENEMY_NORMAL_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy_normal(int nType)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		enemy_normal->bUse = false;
		enemy_normal->posEnemy = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		enemy_normal->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_normal->sclEnemy = D3DXVECTOR3(ENEMY_NORMAL_SCALE, ENEMY_NORMAL_SCALE, ENEMY_NORMAL_SCALE);
	}

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureEnemy_normal = NULL;
		g_pD3DXMeshEnemy_normal = NULL;
		g_pD3DXBuffMatEnemy_normal = NULL;
		g_nNumMatEnemy_normal = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(ENEMY_NORMAL_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatEnemy_normal,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatEnemy_normal,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshEnemy_normal)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �}�e���A���������o��
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_normal->GetBufferPointer();
		g_pD3DMatMeshEnemy_normal = new D3DMATERIAL9[g_nNumMatEnemy_normal];		// ���b�V�������m��
		g_pD3DTextureEnemy_normal = new LPDIRECT3DTEXTURE9[g_nNumMatEnemy_normal];// �e�N�X�`�����m��
		for (int i = 0; i < g_nNumMatEnemy_normal; i++)
		{
			g_pD3DMatMeshEnemy_normal[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
			g_pD3DMatMeshEnemy_normal[i].Ambient = g_pD3DMatMeshEnemy_normal[i].Diffuse;// ����������
			g_pD3DTextureEnemy_normal[i] = NULL;	// �e�N�X�`��������

			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// �e�N�X�`���ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureEnemy_normal[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy_normal(void)
{
	for (int i = 0; i < g_nNumMatEnemy_normal; i++)
	{	// �e�N�X�`���̊J��
		if (g_pD3DTextureEnemy_normal[i] != NULL)
		{
			g_pD3DTextureEnemy_normal[i]->Release();
			g_pD3DTextureEnemy_normal[i] = NULL;
		}
	}

	if(g_pD3DXMeshEnemy_normal != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshEnemy_normal->Release();
		g_pD3DXMeshEnemy_normal = NULL;
	}

	if(g_pD3DXBuffMatEnemy_normal != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatEnemy_normal->Release();
		g_pD3DXBuffMatEnemy_normal = NULL;
	}

	delete[]g_pD3DMatMeshEnemy_normal;
	delete[]g_pD3DTextureEnemy_normal;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy_normal(void)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (enemy_normal->bUse)
		{
			// ���f����]
			enemy_normal->rotEnemy.x += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.y += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.z += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.x = PiCalculate180(enemy_normal->rotEnemy.y);
			enemy_normal->rotEnemy.y = PiCalculate180(enemy_normal->rotEnemy.y);
			enemy_normal->rotEnemy.z = PiCalculate180(enemy_normal->rotEnemy.y);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy_normal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (enemy_normal->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldEnemy_normal);

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, enemy_normal->sclEnemy.x, enemy_normal->sclEnemy.y, enemy_normal->sclEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxScl);

			// �yR�z��1�]�𔽉f(YawPitchRoll��y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_normal->rotEnemy.y, enemy_normal->rotEnemy.x, enemy_normal->rotEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, enemy_normal->posEnemy.x, enemy_normal->posEnemy.y, enemy_normal->posEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy_normal);

			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);
			//// �}�e���A�����ɑ΂���|�C���^�̎擾
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_normal->GetBufferPointer();

			for (int j = 0; j < g_nNumMatEnemy_normal; j++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_pD3DMatMeshEnemy_normal[j]);
				// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
				pDevice->SetTexture(0, g_pD3DTextureEnemy_normal[j]);
				// �`��
				g_pD3DXMeshEnemy_normal->DrawSubset(j);
			}
			// �}�e���A�������ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �o������
//=============================================================================
int SetEnemy_normal(void)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (!enemy_normal->bUse)
		{
			enemy_normal->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void SetEnemy_normalPos(int nIndex, D3DXVECTOR3 pos)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[nIndex];
	enemy_normal->posEnemy = pos;
	enemy_normal->posEnemy.y += ENEMY_BULLET_MUZZELE_HEIGHT;
}

//=============================================================================
// �J���[�ݒ菈��
//=============================================================================
void SetEnemy_normalColor(int nColor)
{
	D3DXCOLOR colorTemp = (1.0f, 1.0f, 1.0f, 1.0f);
	switch(nColor)
	{
	case BULLET_SPECIAL_FIRE:
		colorTemp = SetColorPallet(COLOR_PALLET_ORANGE);
		break;
	case BULLET_SPECIAL_THUNDER:
		colorTemp = SetColorPallet(COLOR_PALLET_YELLOW);
		break;
	case BULLET_SPECIAL_ICE:
		colorTemp = SetColorPallet(COLOR_PALLET_CYAN);
		break;
	case BULLET_SPECIAL_WIND:
		colorTemp = SetColorPallet(COLOR_PALLET_LIGHTGREEN);
		break;
	case BULLET_SPECIAL_WATER:
		colorTemp = SetColorPallet(COLOR_PALLET_BLUE);
		break;
	case BULLET_SPECIAL_EARTH:
		colorTemp = SetColorPallet(COLOR_PALLET_BROWN);
		break;
	}
	g_pD3DMatMeshEnemy_normal->Diffuse = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshEnemy_normal->Ambient = (D3DCOLORVALUE)(colorTemp * 0.5f);
	//g_pD3DMatMeshEnemy_normal->Specular = (D3DCOLORVALUE)colorTemp;
	//colorTemp = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	//g_pD3DMatMeshEnemy_normal->Emissive = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshEnemy_normal->Power = 1.0f;
}

//=============================================================================
// �������
//=============================================================================
void ReleaseEnemy_normal(int nIndex)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[nIndex];
	enemy_normal->bUse = false;
}

//=============================================================================
// �擾�֐�
//=============================================================================
ENEMY_NORMAL *GetEnemy_normal(int no)
{
	return(&enemy_normalWk[no]);
}
