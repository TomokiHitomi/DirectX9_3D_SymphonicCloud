//=============================================================================
//
// RETICLE���� [reticle.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "reticle.h"
#include "calculate.h"
#include "main.h"
#include "magic.h"
#include "ranking.h"
#include "rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexReticle(int no);

void SetVertexReticle(int no);
void SetDiffuseReticle(int no, D3DXCOLOR color);
void SetTextureReticle(int no, int cntPattern);
void SetChangeCollarReticle(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
RETICLE					reticleWk[RETICLE_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureReticle[RETICLE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitReticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RETICLE00,				// �t�@�C���̖��O
			&pD3DTextureReticle[0]);			// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RETICLE01,				// �t�@�C���̖��O
			&pD3DTextureReticle[1]);			// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RETICLE_CENTER,				// �t�@�C���̖��O
			&pD3DTextureReticle[2]);			// �ǂݍ��ރ������̃|�C���^
	}

	// RETICLE�̏�����
	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		reticle->bUse = true;
		reticle->pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
		reticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		reticle->colorMagic = D3DXCOLOR(TEXTURE_RETICLE_R, TEXTURE_RETICLE_G, TEXTURE_RETICLE_B, TEXTURE_RETICLE_A);
		reticle->fBaseAngle = 0.0f;
		reticle->fRadius = 0.0f;
		reticle->fScale = 1.0f;
		reticle->nCollarFlag = 0;


		reticle->Texture = pD3DTextureReticle[i];			// �e�N�X�`���ւ�RETICLE
		if (i == RETICLE_CENTER)
		{
			reticle->size = D3DXVECTOR2(TEXTURE_RETICLE_SIZE_X, TEXTURE_RETICLE_SIZE_Y);
		}
		else
		{
			reticle->size = D3DXVECTOR2(TEXTURE_RETICLE_CENTER_SIZE_X, TEXTURE_RETICLE_CENTER_SIZE_Y);
		}

		// ���_���̍쐬
		MakeVertexReticle(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitReticle(void)
{
	RETICLE *reticle = &reticleWk[0];

	for (int i = 0; i < RETICLE_MAX; i++)
	{
		// ���������
		if (pD3DTextureReticle[i] != NULL)
		{
			pD3DTextureReticle[i]->Release();
			pD3DTextureReticle[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		if (reticle->bUse)
		{
			// ��]����

			switch (i)
			{
			case RETICLE_1:
				reticle->rot.z -= RETICLE_ROTATION_SPEED;
				break;
			case RETICLE_2:
				reticle->rot.z += RETICLE_ROTATION_SPEED;
				break;
			}

			reticle->rot.z = PiCalculate360(reticle->rot.z);// PI�̌덷�C��
			if (GetStage() == STAGE_GAME)
			{
				reticle->colorMagic = GetMagicColor();			// �}�W�b�N�̃J���[���擾���ēK�p
			}
			else
			{
				reticle->colorMagic = GetRankColor();			// �}�W�b�N�̃J���[���擾���ēK�p
				reticle->fScale = RETICLE_RESULT_SCALE;
			}
			SetVertexReticle(i);							// �ړ���̍��W�Œ��_��ݒ�
			SetDiffuseReticle(i, reticle->colorMagic);		// �J���[�K�p����
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		if (reticle->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, reticle->Texture);

			// RETICLE�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RETICLE, reticle->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexReticle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[no];

	// �p�x�Ɣ��a�����߂�
	if (no == RETICLE_CENTER)
	{
		reticle->fBaseAngle = atan2f(TEXTURE_RETICLE_CENTER_SIZE_Y, TEXTURE_RETICLE_CENTER_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_RETICLE_CENTER_SIZE_X, TEXTURE_RETICLE_CENTER_SIZE_Y);
		reticle->fRadius = D3DXVec2Length(&temp);
	}
	else
	{
		reticle->fBaseAngle = atan2f(TEXTURE_RETICLE_SIZE_Y, TEXTURE_RETICLE_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_RETICLE_SIZE_X, TEXTURE_RETICLE_SIZE_Y);
		reticle->fRadius = D3DXVec2Length(&temp);
	}

	// ���_���W�̐ݒ�
	reticle->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	reticle->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_RETICLE_SIZE_X, 100.0f, 0.0f);
	reticle->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_RETICLE_SIZE_Y, 0.0f);
	reticle->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_RETICLE_SIZE_X, 100.0f + TEXTURE_RETICLE_SIZE_Y, 0.0f);
	//SetVertexReticle();

	// rhw�̐ݒ�
	reticle->vertexWk[0].rhw =
	reticle->vertexWk[1].rhw =
	reticle->vertexWk[2].rhw =
	reticle->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	reticle->vertexWk[0].diffuse =
	reticle->vertexWk[1].diffuse =
	reticle->vertexWk[2].diffuse =
	reticle->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	reticle->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	reticle->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE, 0.0f);
	reticle->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE);
	reticle->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseReticle(int no, D3DXCOLOR color)
{
	RETICLE *reticle = &reticleWk[no];

	// ���ˌ��̐ݒ�
	reticle->vertexWk[0].diffuse =
	reticle->vertexWk[1].diffuse =
	reticle->vertexWk[2].diffuse =
	reticle->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureReticle(int no, int cntPattern)
{
	RETICLE *reticle = &reticleWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE;
	reticle->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	reticle->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	reticle->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	reticle->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexReticle(int no)
{
	RETICLE *reticle = &reticleWk[no];

	// ���_0:��]����
	reticle->vertexWk[0].vtx.x = reticle->pos.x - cosf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[0].vtx.y = reticle->pos.y - sinf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	reticle->vertexWk[1].vtx.x = reticle->pos.x + cosf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[1].vtx.y = reticle->pos.y - sinf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	reticle->vertexWk[2].vtx.x = reticle->pos.x - cosf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[2].vtx.y = reticle->pos.y + sinf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	reticle->vertexWk[3].vtx.x = reticle->pos.x + cosf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[3].vtx.y = reticle->pos.y + sinf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarReticle(int no)
{
	RETICLE *reticle = &reticleWk[no];

	switch (reticle->nCollarFlag)
	{
	case 0:
		reticle->colorMagic -= D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		reticle->colorMagic += D3DXCOLOR(0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		if (reticle->colorMagic.r <= 0.0f)
		{
			reticle->colorMagic.r = 0.0f;
			reticle->nCollarFlag++;
		}
		if (reticle->colorMagic.g >= 1.0f)
		{
			reticle->colorMagic.g = 1.0f;
		}
		if (reticle->colorMagic.b >= 1.0f)
		{
			reticle->colorMagic.b = 1.0f;
		}
		break;
	case 1:
		reticle->colorMagic -= D3DXCOLOR(0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f);
		reticle->colorMagic += D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		if (reticle->colorMagic.r >= 1.0f)
		{
			reticle->colorMagic.r = 1.0f;
		}
		if (reticle->colorMagic.g <= 0.0f)
		{
			reticle->colorMagic.g = 0.0f;
			reticle->nCollarFlag++;
		}
		if (reticle->colorMagic.b >= 1.0f)
		{
			reticle->colorMagic.b = 1.0f;
		}
		break;
	case 2:
		reticle->colorMagic -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		reticle->colorMagic += D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f);
		if (reticle->colorMagic.r >= 1.0f)
		{
			reticle->colorMagic.r = 1.0f;
		}
		if (reticle->colorMagic.g >= 1.0f)
		{
			reticle->colorMagic.g = 1.0f;
		}
		if (reticle->colorMagic.b <= 0.0f)
		{
			reticle->colorMagic.b = 0.0f;
			reticle->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	reticle->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	//reticle->bUse = false;
}

//=============================================================================
// RETICLE�擾�֐�
//=============================================================================
RETICLE *GetReticle(int no)
{
	return(&reticleWk[no]);
}

