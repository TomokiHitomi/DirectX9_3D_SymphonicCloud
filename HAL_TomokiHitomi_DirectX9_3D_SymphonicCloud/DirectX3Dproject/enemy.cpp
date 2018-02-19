//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP11B243 24 �l���F�� 
//
//=============================================================================
#include "enemy.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "main.h"
#include "calculate.h"
#include "shadow.h"
#include "enemybullet.h"
#include "parameter.h"
#include "gage.h"
#include "gagefream.h"
#include "gageback.h"
#include "magic.h"
#include "sound.h"
#include "game.h"

#include "bulletqua.h"

// ���f��
#include "enemy_normal.h"
#include "enemy_boss.h"

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
void InitStatusEnemy(int nEnemy);
D3DXCOLOR SetEnemyBulletColor(int nColor);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureEnemy;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshEnemy;

LPD3DXMESH			g_pD3DXMeshEnemy;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatEnemy;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatEnemy;			// �}�e���A�����̐�

ENEMY				enemyWk[ENEMY_MAX];
ENEMY_SYS			enemySysWk;

int					g_nWave;						// �G�l�~�[�o���E�F�[�u
int					g_nEnemyCount[ENEMY_WAVE_MAX];	// �E�F�[�u���Ƃ̃G�l�~�[���J�E���^�[

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int nType)
{
	ENEMY *enemy = &enemyWk[0];
	ENEMY_SYS *enemySys = &enemySysWk;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nWave = 0;		// �E�F�[�u������
	for (int i = 0; i < ENEMY_WAVE_MAX; i++)
	{
		g_nEnemyCount[i] = 0;
	}


	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		InitStatusEnemy(i);
	}

	for (int i = 0; i < ENEMY_ACTIVE_COUNT_MAX; i++)
	{
		enemySys->nActiveEnemy[i] = 0;
	}
	enemySys->nActiveCount = 0;

	// �t�@�C���ǂݍ���
	SetEnemyFile();



	return S_OK;
}

//=============================================================================
// �X�e�[�^�X����������
//=============================================================================
void InitStatusEnemy(int nEnemy)
{
	ENEMY *enemy = &enemyWk[nEnemy];
	// �ʒu�E��]�E�X�P�[���̐ݒ�
	enemy->posEnemy = D3DXVECTOR3(0.0 + nEnemy * 400.0f, 0.0f, 1000.0f);
	enemy->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemy->sclEnemy = D3DXVECTOR3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE);

	enemy->nAttackNorCount = 0;
	enemy->nAttackSpCount = 0;
	enemy->nAttackPattern = 0;
	enemy->nWave = 0;
	enemy->nMoveType = 0;
	enemy->nAttackType = 0;
	enemy->fStatusHP = ENEMY_STATUS_HP;

	enemy->bUse = false;
	enemy->bParameter = false;

	// ���f��
	enemy->nModel = 0;
	enemy->nModelIndex = 0;
	enemy->bModelSet = false;

	// ���b�N�I���p
	enemy->bLockon = false;
	enemy->nIdxLockon = 0;

	// �~�j�}�b�v�p
	enemy->bUseMinimap = false;

	// �V���h�E�p
	enemy->nIdxShadow = 0;
	enemy->fSizeShadow = 0.0f;
	enemy->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
	enemy->bShadow = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{

	for (int i = 0; i < g_nNumMatEnemy; i++)
	{	// �e�N�X�`���̊J��
		if (g_pD3DTextureEnemy[i] != NULL)
		{
			g_pD3DTextureEnemy[i]->Release();
			g_pD3DTextureEnemy[i] = NULL;
		}
	}

	if (g_pD3DXMeshEnemy != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshEnemy->Release();
		g_pD3DXMeshEnemy = NULL;
	}

	if (g_pD3DXBuffMatEnemy != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatEnemy->Release();
		g_pD3DXBuffMatEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
#ifdef _DEBUG
	PrintDebugProc("�yENEMY�z\n");
#endif

	ENEMY *enemy = &enemyWk[0];
	ENEMY_SYS *enemySys = &enemySysWk;
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	enemySys->nActiveCount = 0;

	if (g_nEnemyCount[g_nWave - 1] == 0 && g_nWave != 0) 
	{
		g_nWave++;
		//switch (g_nWave)
		//{
		//case 2:
		//	SetMagicType(BULLET_SPECIAL_FIRE);
		//	SetMagicType(BULLET_SPECIAL_THUNDER);
		//	SetMagicType(BULLET_SPECIAL_ICE);
		//	SetMagicType(BULLET_SPECIAL_WIND);
		//	SetMagicType(BULLET_SPECIAL_WATER);
		//	SetMagicType(BULLET_SPECIAL_EARTH);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//case 3:
		//	//SetMagicType(BULLET_SPECIAL_ICE);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//case 4:
		//	//SetMagicType(BULLET_SPECIAL_WATER);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//case 5:
		//	//SetMagicType(BULLET_SPECIAL_THUNDER);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//case 6:
		//	//SetMagicType(BULLET_SPECIAL_WIND);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//case 7:
		//	//SetMagicType(BULLET_SPECIAL_EARTH);
		//	SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
		//	SetSe(13, E_DS8_FLAG_NONE, false);
		//	break;
		//}
		if (g_nWave >= 2 && g_nWave <= 7)
		{
			SetEnemy_normalColor(GetGameShuffle(g_nWave - 1) + 1);
			SetMagicType(GetGameShuffle(g_nWave - 2) + 1);
			SetVoice(VOICE_POWERUP, E_DS8_FLAG_NONE, CONTINUITY_ON);
			SetSe(13, E_DS8_FLAG_NONE, false);
		}
	}



	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (!enemy->bUse && g_nWave == enemy->nWave && enemy->nWave != 0)
		{
			enemy->bUse = true;
			enemy->posEnemy.x += model->posModel.x;
			enemy->posEnemy.z += model->posModel.z;
		}
		if (enemy->bUse)
		{
#ifdef _DEBUG
			PrintDebugProc("Enemy[%d]  HP:%f", i, enemy->fStatusHP);
#endif

			// �G�l�~�[���f�������Z�b�g�̏ꍇ
			if (!enemy->bModelSet)
			{	// �G�l�~�[���f�����Z�b�g
				switch (enemy->nModel)
				{	// ���f���C���f�b�N�X���擾
				case ENEMY_TYPE_NORMAL:
					enemy->nModelIndex = SetEnemy_normal();
					break;
				case ENEMY_TYPE_BOSS:
					enemy->nModelIndex = SetEnemy_boss();
					SetGage(GAGE_BOSS_HP, i);
					SetSoundBgmBoss(true);
					break;
				}
				enemy->bModelSet = true;
			}
			// �V���h�E�����Z�b�g�̏ꍇ
			if (!enemy->bShadow)
			{
				// �e�̐���
				enemy->nIdxShadow = CreateShadow(enemy->posEnemy, 25.0f, 25.0f);
				enemy->fSizeShadow = 30.0f;
				enemy->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
				enemy->bShadow = true;
			}

			// HP�o�[�����Z�b�g�̏ꍇ
			if (!enemy->bParameter && enemy->nModel != ENEMY_TYPE_BOSS)
			{
				SetParameter(2, enemy->posEnemy, i);
				enemy->bParameter = true;
			}

			// �L���G�l�~�[���J�E���g
			if (enemySys->nActiveCount < ENEMY_ACTIVE_COUNT_MAX)
			{
				enemySys->nActiveEnemy[enemySys->nActiveCount] = i;
			}
			enemySys->nActiveCount++;

			// �p�x�v�Z
			enemy->rotEnemy.y = D3DX_PI * 0.5 - PreventionAtan2XZ(enemy->posEnemy, model->posModel);


			// �G�l�~�[�ړ�
			D3DXVECTOR3 vecMove = model->posModel - enemy->posEnemy;
			vecMove.y = 0.0f;
			D3DXVec3Normalize(&vecMove, &vecMove);
			enemy->posEnemy += vecMove * ENEMY_MOVE_SPEED;

			// �ʏ�U���Ԋu
			if (*nTotalCount - ENEMY_ATTACK_NORMAL_INTERVAL > enemy->nAttackNorCount)
			{
				switch (enemy->nAttackType)
				{
				case ENEMYBULLET_NORMAL:
					SetEnemybullet(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f),
						enemy->posEnemy + D3DXVECTOR3(
							cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH,
							ENEMY_BULLET_MUZZELE_HEIGHT,
							sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH),
						SetEnemyBulletColor(GetGameShuffle(g_nWave - 1) + 1));
					break;
				case ENEMYBULLET_HOMING:
					SetHomingEnemybullet(enemy->posEnemy + D3DXVECTOR3(
						cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH,
						ENEMY_BULLET_MUZZELE_HEIGHT,
						sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH),
						1.0f,
						SetEnemyBulletColor(GetGameShuffle(g_nWave - 1) + 1));
					break;
				case ENEMYBULLET_NORMAL_HOMING:
					SetEnemybullet(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f),
						enemy->posEnemy + D3DXVECTOR3(
							cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH,
							ENEMY_BULLET_MUZZELE_HEIGHT,
							sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH),
						SetEnemyBulletColor(GetGameShuffle(g_nWave)-1));
					SetHomingEnemybullet(enemy->posEnemy + D3DXVECTOR3(
						cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH,
						ENEMY_BULLET_MUZZELE_HEIGHT,
						sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH),
						1.0f,
						SetEnemyBulletColor(GetGameShuffle(g_nWave - 1) + 1));
					break;
				}
				enemy->nAttackNorCount = *nTotalCount;
			}

			// ����U���Ԋu
			if (*nTotalCount - ENEMY_ATTACK_SPECIAL_INTERVAL > enemy->nAttackSpCount
				&& enemy->nModel == ENEMY_TYPE_BOSS)
			{
				SetHomingEnemybullet(enemy->posEnemy + D3DXVECTOR3(
					cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH,
					ENEMY_BULLET_MUZZELE_HEIGHT,
					sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH),
					1.0f,
					SetEnemyBulletColor(GetGameShuffle(g_nWave - 1) + 1));
				switch (enemy->nAttackPattern)
				{
				case 0:
					SetSysEnemybullet(0, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));;
					SetSysBulletQua(i, 5, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 1:
					SetSysEnemybullet(1, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 2:
					SetSysBulletQua(i, 0, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, 0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 5, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 3:
					SetSysBulletQua(i, 2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, -0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 4:
					SetSysEnemybullet(0, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 4, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 5:
					SetSysEnemybullet(1, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 6:
					SetSysEnemybullet(2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 4, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 7:
					SetSysEnemybullet(3, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, -0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 8:
					SetSysBulletQua(i, 0, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, 0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 9:
					SetSysBulletQua(i, 1, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, 0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 10:
					SetSysEnemybullet(2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 4, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 11:
					SetSysEnemybullet(3, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 21, 5.0f, 1.5f, 0.05f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 12:
					SetSysBulletQua(i, 2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, -0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 4, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 13:
					SetSysBulletQua(i, 2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, -0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern++;
					break;
				case 14:
					SetSysBulletQua(i, 2, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 5.0f, 0.0f, -0.005f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					SetSysBulletQua(i, 4, ENEMYBULLET_TEX_2, 10, COLOR_PALLET_RED, 20, 0.5f, 0.0f, 0.01f,
						enemy->posEnemy + D3DXVECTOR3(cosf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH, ENEMY_BULLET_MUZZELE_HEIGHT, sinf(ENEMY_BULLET_MUZZELE_ANGLE - enemy->rotEnemy.y) * ENEMY_BULLET_MUZZELE_LENGTH));
					enemy->nAttackPattern = 0;
					break;
				}
				enemy->nAttackSpCount = *nTotalCount;
			}

			switch (enemy->nModel)
			{	// ���f���C���f�b�N�X���擾
			case ENEMY_TYPE_NORMAL:
				SetEnemy_normalPos(enemy->nModelIndex, enemy->posEnemy);
				break;
			case ENEMY_TYPE_BOSS:
				SetEnemy_bossPos(enemy->nModelIndex, enemy->posEnemy, enemy->rotEnemy.y);
				break;
			}

			// �V���h�E�Ǘ�
			SetPositionShadow(enemy->nIdxShadow, D3DXVECTOR3(enemy->posEnemy.x, 0.2f, enemy->posEnemy.z));
			SetVertexShadow(enemy->nIdxShadow, enemy->fSizeShadow, enemy->fSizeShadow);
			SetColorShadow(enemy->nIdxShadow, enemy->colShadow);
			// �f�o�b�O�p
#ifdef _DEBUG
			PrintDebugProc("Pos[X:%f Z:%f]  Rot[Y:%f]  Atk:[N:%d S:%d]\n", enemy->posEnemy.x, enemy->posEnemy.z, enemy->rotEnemy.y, enemy->nAttackNorCount, enemy->nAttackSpCount);
#endif
		}

	}
#ifdef _DEBUG
	PrintDebugProc("ActiveCount:%d  ActiveNum:", enemySys->nActiveCount);

	for (int i = 0; i < enemySys->nActiveCount; i++)
	{
		if (i < ENEMY_ACTIVE_COUNT_MAX)
		{
			PrintDebugProc("[%d]", enemySys->nActiveEnemy[i]);
		}
	}
	PrintDebugProc("\n");
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	//D3DXMATERIAL *pD3DXMat;
	//D3DMATERIAL9 matDef;

	//ENEMY *enemy = &enemyWk[0];
	//int *pCameraMode = GetCameraMode();
	//CAMERA *camera = GetCamera(*pCameraMode);

	//for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	//{
	//	if (enemy->bUse)
	//	{
	//		/******************** ���[���h�ϊ� ********************/
	//		// ���[���h�}�g���N�X�̏�����
	//		D3DXMatrixIdentity(&enemy->mtxWorld);

	//		// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	//		D3DXMatrixScaling(&mtxScl, enemy->sclEnemy.x, enemy->sclEnemy.y, enemy->sclEnemy.z);
	//		D3DXMatrixMultiply(&enemy->mtxWorld, &enemy->mtxWorld, &mtxScl);

	//		// �yR�z��1�]�𔽉f(YawPitchRoll��y,x,z)
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy->rotEnemy.y, enemy->rotEnemy.x, enemy->rotEnemy.z);
	//		D3DXMatrixMultiply(&enemy->mtxWorld, &enemy->mtxWorld, &mtxRot);

	//		// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	//		D3DXMatrixTranslation(&mtxTranslate, enemy->posEnemy.x, enemy->posEnemy.y, enemy->posEnemy.z);
	//		D3DXMatrixMultiply(&enemy->mtxWorld, &enemy->mtxWorld, &mtxTranslate);

	//		// ���[���h�}�g���N�X�̐ݒ�
	//		pDevice->SetTransform(D3DTS_WORLD, &enemy->mtxWorld);



	//		/******************** �r���[�|�[�g�ϊ� ********************/
	//		// ���݂̃}�e���A����ۑ�
	//		pDevice->GetMaterial(&matDef);
	//		//// �}�e���A�����ɑ΂���|�C���^�̎擾
	//		//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy->GetBufferPointer();

	//		for (int j = 0; j < g_nNumMatEnemy; j++)
	//		{
	//			// �}�e���A���̐ݒ�
	//			pDevice->SetMaterial(&g_pD3DMatMeshEnemy[j]);
	//			// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
	//			pDevice->SetTexture(0, g_pD3DTextureEnemy[j]);
	//			// �`��
	//			g_pD3DXMeshEnemy->DrawSubset(j);
	//		}
	//		// �}�e���A�������ɖ߂�
	//		pDevice->SetMaterial(&matDef);
	//	}
	//}
}

//=============================================================================
// �G�l�~�[�t�@�C���ǂݍ��݊֐�
//=============================================================================
void SetEnemyFile(void)
{
	ENEMY *enemy = &enemyWk[0];

	FILE *fp = fopen(ENEMY_SYSTEM_FILE, "r");
	int data[ENEMY_SYSTEM_DATA_MAX];

	// �t�@�C���I�[�v���G���[����
	if (fp == NULL)
	{
		return;
	}

	// �G�l�~�[�X�e�[�^�X�ݒ�
	while (fscanf(fp, "%d,%d,%d,%d,%d,%d,%d,%d",
		&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]) != EOF)
	{
		if (data[0] <= 0)
		{
			break;
		}
		enemy->nWave = data[0];
		enemy->nModel = data[1];
		switch (enemy->nModel)
		{
		case ENEMY_TYPE_NORMAL:
			enemy->fStatusHP = ENEMY_STATUS_HP;
			break;
		case ENEMY_TYPE_BOSS:
			enemy->fStatusHP = ENEMY_STATUS_BOSS_HP;
			break;
		}

		enemy->posEnemy = D3DXVECTOR3((float)data[2], (float)data[3], (float)data[4]);
		enemy->nMoveType = data[5];
		enemy->fMoveSpeed = (float)data[7];
		enemy->nAttackType = data[7];
		g_nEnemyCount[enemy->nWave - 1]++;
		enemy++;
	}

	fclose(fp);
}

//=============================================================================
// �E�F�[�u�ݒ�֐�
//=============================================================================
void SetWaveEnemy(int no)
{
	if (g_nWave < no)
	{
		g_nWave = no;
	}
}

//=============================================================================
// �G�l�~�[�̉���֐�
//=============================================================================
void ReleaseEnemy(int nEnemy)
{
	ENEMY *enemy = &enemyWk[nEnemy];
	g_nEnemyCount[enemy->nWave - 1]--;		// �G�l�~�[�J�E���^�����Z
	switch (enemy->nModel)
	{
	case ENEMY_TYPE_NORMAL:
		ReleaseEnemy_normal(enemy->nModelIndex);
		InitStatusEnemy(nEnemy);				// �G�l�~�[����������
		break;
	case ENEMY_TYPE_BOSS:
		InitStatusEnemy(nEnemy);				// �G�l�~�[����������
		break;
	}
}

//=============================================================================
// �o���b�g�J���[�ݒ菈��
//=============================================================================
D3DXCOLOR SetEnemyBulletColor(int nColor)
{
	D3DXCOLOR colorTemp = (1.0f, 1.0f, 1.0f, 1.0f);
	switch (nColor)
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
	return colorTemp;
}

//=============================================================================
// �擾�֐�
//=============================================================================
ENEMY *GetEnemy(int no)
{
	return(&enemyWk[no]);
}

//=============================================================================
// �V�X�e���擾�֐�
//=============================================================================
ENEMY_SYS *GetEnemySys(void)
{
	return(&enemySysWk);
}