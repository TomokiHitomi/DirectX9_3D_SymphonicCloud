//=============================================================================
//
// �p�����[�^���� [gage.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "gage.h"
#include "sound.h"
#include "model.h"
#include "calculate.h"
#include "gagefream.h"
#include "gageback.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGage(int no, D3DXCOLOR xColor);
void SetVertexGage(int no);
void SetPointGage(int nGage, float fGageSizeDef, float fStatusDef, float fStatus);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAGE					gageWk[GAGE_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureGage = NULL;
LPDIRECT3DTEXTURE9		pD3DTextureModelmp = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGE00,				// �t�@�C���̖��O
			&pD3DTextureGage);			// �ǂݍ��ރ������̃|�C���^
	}

	// �p�����[�^�̏�����
	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{

		InitStatusGage(i);
	}

	return S_OK;
}

//=============================================================================
// �X�e�[�^�X�̏���������
//=============================================================================
void InitStatusGage(int nGage)
{
	GAGE *gage = &gageWk[nGage];

	gage->Texture = pD3DTextureGage;			// �e�N�X�`���ւ̃G�l�~�[

	//// �p�����[�^�X�e�[�^�X�̏�����
	//if (nGage != GAGE_BOSS_HP)
	//{
	//	gage->bUse = true;
	//}
	//else
	//{
	//	gage->bUse = false;
	//}

	gage->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gage->fPointGauge = 0;
	gage->bStandby = false;
	gage->nTarget = 0;

	// ���_���̍쐬
	switch (nGage)
	{
	case GAGE_PLAYER_HP:
		gage->pos = D3DXVECTOR3(HP_GAGE_POS_X, HP_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(HP_GAGE_SIZE_X, HP_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_GREEN));
		gage->bUse = true;
		break;
	case GAGE_PLAYER_MP:
		gage->pos = D3DXVECTOR3(MP_GAGE_POS_X, MP_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(MP_GAGE_SIZE_X, MP_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BLUE));
		gage->bUse = true;
		break;
	case GAGE_BULLET_NORMAL:
		gage->pos = D3DXVECTOR3(BULLET_GAGE_POS_X, BULLET_GAGE_POS_Y , 0.0f);
		gage->vecSize = D3DXVECTOR2(BULLET_GAGE_SIZE_X, BULLET_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_MAGENTA));
		gage->bUse = true;
		break;
	case GAGE_BULLET_SPECIAL_FIRE:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_ORANGE));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_THUNDER:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_YELLOW));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_ICE:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_CYAN));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_WIND:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_LIGHTGREEN));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_WATER:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BLUE));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_EARTH:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BROWN));
		gage->bUse = false;
		break;
	case GAGE_BOSS_HP:
		gage->pos = D3DXVECTOR3(BOSS_GAGE_POS_X, BOSS_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(BOSS_GAGE_SIZE_X, BOSS_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_RED));
		gage->bUse = false;
		break;
	}




	return;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGage(void)
{
	GAGE *gage = &gageWk[0];

	// ���������
	if (pD3DTextureGage != NULL)
	{
		pD3DTextureGage->Release();
		pD3DTextureGage = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGage(void)
{
	GAGE *gage = &gageWk[0];
	MODEL *model = GetModel(0);
	ENEMY *enemy;

	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{
		if (gage->bUse)
		{
			switch (i)
			{
			case GAGE_PLAYER_HP:
				SetPointGage(GAGE_PLAYER_HP, HP_GAGE_SIZE_X, MODEL_STATUS_HP, model->fStatusHP);
				break;
			case GAGE_PLAYER_MP:
				SetPointGage(GAGE_PLAYER_MP, MP_GAGE_SIZE_X, MODEL_STATUS_MP, model->fStatusMP);
				break;
			case GAGE_BULLET_NORMAL:
				SetPointGage(GAGE_BULLET_NORMAL, BULLET_GAGE_SIZE_X, MODEL_STATUS_NORMAL, model->fStatusNormal);

				break;
			case GAGE_BULLET_SPECIAL_FIRE:
				SetPointGage(GAGE_BULLET_SPECIAL_FIRE, MAGIC_GAGE_SIZE_X, MODEL_STATUS_FIRE, model->fStatusFire);
				break;
			case GAGE_BULLET_SPECIAL_THUNDER:
				SetPointGage(GAGE_BULLET_SPECIAL_THUNDER, MAGIC_GAGE_SIZE_X, MODEL_STATUS_THUNDER, model->fStatusThunder);
				break;
			case GAGE_BULLET_SPECIAL_ICE:
				SetPointGage(GAGE_BULLET_SPECIAL_ICE, MAGIC_GAGE_SIZE_X, MODEL_STATUS_ICE, model->fStatusIce);
				break;
			case GAGE_BULLET_SPECIAL_WIND:
				SetPointGage(GAGE_BULLET_SPECIAL_WIND, MAGIC_GAGE_SIZE_X, MODEL_STATUS_WIND, model->fStatusWind);
				break;
			case GAGE_BULLET_SPECIAL_WATER:
				SetPointGage(GAGE_BULLET_SPECIAL_WATER, MAGIC_GAGE_SIZE_X, MODEL_STATUS_WARER, model->fStatusWater);
				break;
			case GAGE_BULLET_SPECIAL_EARTH:
				SetPointGage(GAGE_BULLET_SPECIAL_EARTH, MAGIC_GAGE_SIZE_X, MODEL_STATUS_EARTH, model->fStatusEarth);
				break;
			case GAGE_BOSS_HP:
				enemy = GetEnemy(gage->nTarget);
				SetPointGage(GAGE_BOSS_HP, BOSS_GAGE_SIZE_X, ENEMY_STATUS_BOSS_HP, enemy->fStatusHP);
				break;
			default:
				break;
			}

			// ���_���X�V
			SetVertexGage(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{
		if (gage->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, gage->Texture);

			// �p�����[�^�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGE, gage->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGage(int no, D3DXCOLOR xColor)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[no];

	// ���_���W�̐ݒ�
	SetVertexGage(no);

	// rhw�̐ݒ�
	gage->vertexWk[0].rhw =
	gage->vertexWk[1].rhw =
	gage->vertexWk[2].rhw =
	gage->vertexWk[3].rhw = 1.0f;

	xColor.a -= GAGE_ALPHA;

	// ���ˌ��̐ݒ�
	gage->vertexWk[0].diffuse =
	gage->vertexWk[1].diffuse =
	gage->vertexWk[2].diffuse =
	gage->vertexWk[3].diffuse = xColor;

	// �e�N�X�`�����W�̐ݒ�
	gage->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	gage->vertexWk[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
	gage->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
	gage->vertexWk[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexGage(int nGage)
{
	GAGE *gage = &gageWk[nGage];
	gage->vertexWk[0].vtx.x = gage->pos.x;
	gage->vertexWk[0].vtx.y = gage->pos.y - gage->vecSize.y;
	gage->vertexWk[0].vtx.z = 0.0f;

	gage->vertexWk[1].vtx.x = gage->pos.x + gage->fPointGauge;
	gage->vertexWk[1].vtx.y = gage->pos.y - gage->vecSize.y;
	gage->vertexWk[1].vtx.z = 0.0f;

	gage->vertexWk[2].vtx.x = gage->pos.x;
	gage->vertexWk[2].vtx.y = gage->pos.y + gage->vecSize.y;
	gage->vertexWk[2].vtx.z = 0.0f;

	gage->vertexWk[3].vtx.x = gage->pos.x + gage->fPointGauge;
	gage->vertexWk[3].vtx.y = gage->pos.y + gage->vecSize.y;
	gage->vertexWk[3].vtx.z = 0.0f;
	if (nGage == GAGE_BULLET_NORMAL)
	{
		gage->vertexWk[0].vtx.x = gage->pos.x - gage->fPointGauge;
		gage->vertexWk[1].vtx.x = gage->pos.x;
		gage->vertexWk[2].vtx.x = gage->pos.x - gage->fPointGauge;
		gage->vertexWk[3].vtx.x = gage->pos.x;
	}
}

//=============================================================================
// �Z�b�g�֐�
//=============================================================================
void SetPointGage(int nGage, float fGageSizeDef, float fStatusDef, float fStatus)
{
	GAGE *gage = &gageWk[nGage];
	if (!gage->bStandby)
	{
		gage->fPointGauge += GAGE_STANDBY_SPEED * fGageSizeDef;
		if (gage->fPointGauge > fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			gage->bStandby = true;
		}
	}
	else
	{
		if (gage->fPointGauge > fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge -= GAGE_CHANGE_SPEED * fGageSizeDef;
			if (gage->fPointGauge < fGageSizeDef / fStatusDef * fStatus)
			{
				gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			}
		}
		else if (gage->fPointGauge < fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge += GAGE_CHANGE_SPEED * fGageSizeDef;
			if (gage->fPointGauge >fGageSizeDef / fStatusDef * fStatus)
			{
				gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			}
		}
	}
}

//=============================================================================
// �Z�b�g�֐��i�ʏ�̑�������0�ABOSS�Z�b�g�̏ꍇ��nEnemy�j
//=============================================================================
void SetGage(int nGage, int nBoss)
{
	GAGE *gage = &gageWk[nGage];
	gage->bUse = true;
	if (nGage == GAGE_BOSS_HP)
	{
		gage->nTarget = nBoss;
		SetGagefream(GAGEFREAM_TYPE_BOSS);
		SetGageback(GAGEBACK_TYPE_BOSS);
	}
}

//=============================================================================
// �p�����[�^�擾�֐�
//=============================================================================
GAGE *GetGage(int no)
{
	return(&gageWk[no]);
}

