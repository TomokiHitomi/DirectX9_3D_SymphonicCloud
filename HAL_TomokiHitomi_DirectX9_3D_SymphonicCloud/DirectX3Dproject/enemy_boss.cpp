//=============================================================================
//
// �G�l�~�[���f���i�{�X�j���� [enemy_boss.cpp]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#include "enemy.h"
#include "enemy_boss.h"
#include "main.h"
#include "calculate.h"

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
//**********************************2*******************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureEnemy_boss;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshEnemy_boss;

LPD3DXMESH			g_pD3DXMeshEnemy_boss;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatEnemy_boss;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatEnemy_boss;			// �}�e���A�����̐�
D3DXMATRIX			g_mtxWorldEnemy_boss;			// ���[���h�}�g���b�N�X

ENEMY_BOSS		enemy_bossWk[ENEMY_BOSS_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy_boss(int nType)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		enemy_boss->bUse = false;
		enemy_boss->posEnemy = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		enemy_boss->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss->sclEnemy = D3DXVECTOR3(ENEMY_BOSS_SCALE, ENEMY_BOSS_SCALE, ENEMY_BOSS_SCALE);
	}

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureEnemy_boss = NULL;
		g_pD3DXMeshEnemy_boss = NULL;
		g_pD3DXBuffMatEnemy_boss = NULL;
		g_nNumMatEnemy_boss = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(ENEMY_BOSS_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatEnemy_boss,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatEnemy_boss,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshEnemy_boss)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �}�e���A���������o��
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_boss->GetBufferPointer();
		g_pD3DMatMeshEnemy_boss = new D3DMATERIAL9[g_nNumMatEnemy_boss];		// ���b�V�������m��
		g_pD3DTextureEnemy_boss = new LPDIRECT3DTEXTURE9[g_nNumMatEnemy_boss];// �e�N�X�`�����m��
		for (int i = 0; i < g_nNumMatEnemy_boss; i++)
		{
			g_pD3DMatMeshEnemy_boss[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
			g_pD3DMatMeshEnemy_boss[i].Ambient = g_pD3DMatMeshEnemy_boss[i].Diffuse;// ����������
			g_pD3DTextureEnemy_boss[i] = NULL;	// �e�N�X�`��������

			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// �e�N�X�`���ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureEnemy_boss[i])))
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
void UninitEnemy_boss(void)
{
	for (int i = 0; i < g_nNumMatEnemy_boss; i++)
	{	// �e�N�X�`���̊J��
		if (g_pD3DTextureEnemy_boss[i] != NULL)
		{
			g_pD3DTextureEnemy_boss[i]->Release();
			g_pD3DTextureEnemy_boss[i] = NULL;
		}
	}

	if(g_pD3DXMeshEnemy_boss != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshEnemy_boss->Release();
		g_pD3DXMeshEnemy_boss = NULL;
	}

	if(g_pD3DXBuffMatEnemy_boss != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatEnemy_boss->Release();
		g_pD3DXBuffMatEnemy_boss = NULL;
	}

	delete[]g_pD3DMatMeshEnemy_boss;
	delete[]g_pD3DTextureEnemy_boss;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy_boss(void)
{
	//ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	//for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	//{
	//	if (enemy_boss->bUse)
	//	{

	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy_boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		if (enemy_boss->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldEnemy_boss);

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, enemy_boss->sclEnemy.x, enemy_boss->sclEnemy.y, enemy_boss->sclEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxScl);

			// �yR�z��1�]�𔽉f(YawPitchRoll��y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_boss->rotEnemy.y, enemy_boss->rotEnemy.x, enemy_boss->rotEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, enemy_boss->posEnemy.x, enemy_boss->posEnemy.y, enemy_boss->posEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy_boss);

			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);
			//// �}�e���A�����ɑ΂���|�C���^�̎擾
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_boss->GetBufferPointer();

			for (int j = 0; j < g_nNumMatEnemy_boss; j++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_pD3DMatMeshEnemy_boss[j]);
				// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
				pDevice->SetTexture(0, g_pD3DTextureEnemy_boss[j]);
				// �`��
				g_pD3DXMeshEnemy_boss->DrawSubset(j);
			}
			// �}�e���A�������ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �o������
//=============================================================================
int SetEnemy_boss(void)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		if (!enemy_boss->bUse)
		{
			enemy_boss->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void SetEnemy_bossPos(int nIndex, D3DXVECTOR3 pos, float rot)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[nIndex];
	enemy_boss->posEnemy = pos;
	enemy_boss->rotEnemy.y = rot;
}

//=============================================================================
// �������
//=============================================================================
void ReleaseEnemy_boss(int nIndex)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[nIndex];
	enemy_boss->bUse = false;
}

//=============================================================================
// �擾�֐�
//=============================================================================
ENEMY_BOSS *GetEnemy_boss(int no)
{
	return(&enemy_bossWk[no]);
}
