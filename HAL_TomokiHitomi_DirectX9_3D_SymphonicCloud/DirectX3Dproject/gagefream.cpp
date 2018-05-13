//=============================================================================
//
// GAGEFREAM���� [gagefream.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "gagefream.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGagefream(int no);

void SetVertexGagefream(int no);
void SetDiffuseGagefream(int no, D3DXCOLOR color);
void SetTextureGagefream(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAGEFREAM					gagefreamWk[GAGEFREAM_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureGagefream[GAGEFREAM_TEXTURE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGagefream(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEFREAM00,				// �t�@�C���̖��O
			&pD3DTextureGagefream[GAGEFREAM_MAGIC]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEFREAM01,				// �t�@�C���̖��O
			&pD3DTextureGagefream[GAGEFREAM_DEFAULT]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEFREAM02,				// �t�@�C���̖��O
			&pD3DTextureGagefream[GAGEFREAM_BULLET]);			// �ǂݍ��ރ������̃|�C���^

	}

	// GAGEFREAM�̏�����
	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{

		switch (i)
		{
		case GAGEFREAM_TYPE_MAGIC:
			gagefream->bUse = true;
			gagefream->bPatternFlag = false;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_POS_X, TEXTURE_GAGEFREAM_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_SIZE_X, TEXTURE_GAGEFREAM_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_MAGIC];			// �e�N�X�`���ւ�GAGEFREAM

			break;
		case GAGEFREAM_TYPE_BOSS:
			gagefream->bUse = false;
			gagefream->bPatternFlag = true;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_BOSS_POS_X, TEXTURE_GAGEFREAM_BOSS_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_BOSS_SIZE_X, TEXTURE_GAGEFREAM_BOSS_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_DEFAULT];		// �e�N�X�`���ւ�GAGEFREAM
			break;
		case GAGEFREAM_TYPE_MODEL:
			gagefream->bUse = true;
			gagefream->bPatternFlag = true;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_MODEL_POS_X, TEXTURE_GAGEFREAM_MODEL_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_MODEL_SIZE_X, TEXTURE_GAGEFREAM_MODEL_SIZE_Y);
			gagefream->nPatternAnim = 1;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_DEFAULT];		// �e�N�X�`���ւ�GAGEFREAM
			break;
		case GAGEFREAM_TYPE_BULLET:
			gagefream->bUse = true;
			gagefream->bPatternFlag = false;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_BULLET_POS_X, TEXTURE_GAGEFREAM_BULLET_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_BULLET_SIZE_X, TEXTURE_GAGEFREAM_BULLET_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_BULLET];		// �e�N�X�`���ւ�GAGEFREAM
			break;
		}

		gagefream->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_���̍쐬
		MakeVertexGagefream(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGagefream(void)
{
	for (int i = 0; i < GAGEFREAM_TEXTURE_MAX; i++)
	{
		// ���������
		if (pD3DTextureGagefream[i] != NULL)
		{
			pD3DTextureGagefream[i]->Release();
			pD3DTextureGagefream[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGagefream(void)
{
	GAGEFREAM *gagefream = &gagefreamWk[0];
	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{
		if (gagefream->bUse)
		{
			SetDiffuseGagefream(i, GetMagicColor());
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGagefream(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{
		if (gagefream->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, gagefream->Texture);

			// GAGEFREAM�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGEFREAM, gagefream->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGagefream(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// ���_���W�̐ݒ�
	SetVertexGagefream(no);

	// rhw�̐ݒ�
	gagefream->vertexWk[0].rhw =
	gagefream->vertexWk[1].rhw =
	gagefream->vertexWk[2].rhw =
	gagefream->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	gagefream->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);


	if (gagefream->bPatternFlag)
	{
		SetTextureGagefream(no, no - 1);
	}
	else
	{
		// �e�N�X�`�����W�̐ݒ�
		gagefream->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		gagefream->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		gagefream->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		gagefream->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexGagefream(int no)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	gagefream->vertexWk[0].vtx.x = gagefream->pos.x - gagefream->size.x;
	gagefream->vertexWk[0].vtx.y = gagefream->pos.y - gagefream->size.y;
	gagefream->vertexWk[0].vtx.z = 0.0f;

	gagefream->vertexWk[1].vtx.x = gagefream->pos.x + gagefream->size.x;
	gagefream->vertexWk[1].vtx.y = gagefream->pos.y - gagefream->size.y;
	gagefream->vertexWk[1].vtx.z = 0.0f;

	gagefream->vertexWk[2].vtx.x = gagefream->pos.x - gagefream->size.x;
	gagefream->vertexWk[2].vtx.y = gagefream->pos.y + gagefream->size.y;
	gagefream->vertexWk[2].vtx.z = 0.0f;

	gagefream->vertexWk[3].vtx.x = gagefream->pos.x + gagefream->size.x;
	gagefream->vertexWk[3].vtx.y = gagefream->pos.y + gagefream->size.y;
	gagefream->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseGagefream(int no, D3DXCOLOR color)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// ���ˌ��̐ݒ�
	gagefream->vertexWk[0].diffuse =
		gagefream->vertexWk[1].diffuse =
		gagefream->vertexWk[2].diffuse =
		gagefream->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGagefream(int no, int cntPattern)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM;
	gagefream->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gagefream->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gagefream->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gagefream->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetGagefream(int nGage)
{
	GAGEFREAM *gagefream = &gagefreamWk[nGage];
	gagefream->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseGagefream(void)
{
	GAGEFREAM *gagefream = &gagefreamWk[0];
	gagefream->bUse = false;
}

//=============================================================================
// GAGEFREAM�擾�֐�
//=============================================================================
GAGEFREAM *GetGagefream(int no)
{
	return(&gagefreamWk[no]);
}

