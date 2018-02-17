//=============================================================================
//
// GAGEBACK���� [gageback.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "gageback.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGageback(int no);

void SetVertexGageback(int no);
void SetDiffuseGageback(int no, D3DXCOLOR color);
void SetTextureGageback(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAGEBACK					gagebackWk[GAGEBACK_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureGageback[GAGEBACK_TEXTURE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGageback(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEBACK00,				// �t�@�C���̖��O
			&pD3DTextureGageback[GAGEBACK_MAGIC]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEBACK01,				// �t�@�C���̖��O
			&pD3DTextureGageback[GAGEBACK_DEFAULT]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGEBACK02,				// �t�@�C���̖��O
			&pD3DTextureGageback[GAGEBACK_BULLET]);			// �ǂݍ��ރ������̃|�C���^
	}

	// GAGEBACK�̏�����
	for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	{

		switch (i)
		{
		case GAGEBACK_TYPE_MAGIC:
			gageback->bUse = true;
			gageback->bPatternFlag = false;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_POS_X, TEXTURE_GAGEBACK_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_SIZE_X, TEXTURE_GAGEBACK_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_MAGIC];			// �e�N�X�`���ւ�GAGEBACK

			break;
		case GAGEBACK_TYPE_BOSS:
			gageback->bUse = false;
			gageback->bPatternFlag = true;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_BOSS_POS_X, TEXTURE_GAGEBACK_BOSS_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_BOSS_SIZE_X, TEXTURE_GAGEBACK_BOSS_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_DEFAULT];		// �e�N�X�`���ւ�GAGEBACK
			break;
		case GAGEBACK_TYPE_MODEL:
			gageback->bUse = true;
			gageback->bPatternFlag = true;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_MODEL_POS_X, TEXTURE_GAGEBACK_MODEL_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_MODEL_SIZE_X, TEXTURE_GAGEBACK_MODEL_SIZE_Y);
			gageback->nPatternAnim = 1;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_DEFAULT];		// �e�N�X�`���ւ�GAGEBACK
			break;
		case GAGEBACK_TYPE_BULLET:
			gageback->bUse = true;
			gageback->bPatternFlag = false;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_BULLET_POS_X, TEXTURE_GAGEBACK_BULLET_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_BULLET_SIZE_X, TEXTURE_GAGEBACK_BULLET_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_BULLET];		// �e�N�X�`���ւ�GAGEFREAM
			break;
		}

		gageback->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_���̍쐬
		MakeVertexGageback(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGageback(void)
{
	for (int i = 0; i < GAGEBACK_TEXTURE_MAX; i++)
	{
		// ���������
		if (pD3DTextureGageback[i] != NULL)
		{
			pD3DTextureGageback[i]->Release();
			pD3DTextureGageback[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGageback(void)
{
	//GAGEBACK *gageback = &gagebackWk[0];
	//for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	//{
	//	if (gageback->bUse)
	//	{
	//		SetDiffuseGageback(i, GetMagicColor());
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGageback(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	{
		if (gageback->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, gageback->Texture);

			// GAGEBACK�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGEBACK, gageback->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGageback(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[no];

	// ���_���W�̐ݒ�
	SetVertexGageback(no);

	// rhw�̐ݒ�
	gageback->vertexWk[0].rhw =
	gageback->vertexWk[1].rhw =
	gageback->vertexWk[2].rhw =
	gageback->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	gageback->vertexWk[0].diffuse =
	gageback->vertexWk[1].diffuse =
	gageback->vertexWk[2].diffuse =
	gageback->vertexWk[3].diffuse = D3DXCOLOR(TEXTURE_GAGEBACK_R, TEXTURE_GAGEBACK_G, TEXTURE_GAGEBACK_B, TEXTURE_GAGEBACK_A);


	if (gageback->bPatternFlag)
	{
		SetTextureGageback(no, no - 1);
	}
	else
	{
		// �e�N�X�`�����W�̐ݒ�
		gageback->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		gageback->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		gageback->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		gageback->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexGageback(int no)
{
	GAGEBACK *gageback = &gagebackWk[no];

	gageback->vertexWk[0].vtx.x = gageback->pos.x - gageback->size.x;
	gageback->vertexWk[0].vtx.y = gageback->pos.y - gageback->size.y;
	gageback->vertexWk[0].vtx.z = 0.0f;

	gageback->vertexWk[1].vtx.x = gageback->pos.x + gageback->size.x;
	gageback->vertexWk[1].vtx.y = gageback->pos.y - gageback->size.y;
	gageback->vertexWk[1].vtx.z = 0.0f;

	gageback->vertexWk[2].vtx.x = gageback->pos.x - gageback->size.x;
	gageback->vertexWk[2].vtx.y = gageback->pos.y + gageback->size.y;
	gageback->vertexWk[2].vtx.z = 0.0f;

	gageback->vertexWk[3].vtx.x = gageback->pos.x + gageback->size.x;
	gageback->vertexWk[3].vtx.y = gageback->pos.y + gageback->size.y;
	gageback->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseGageback(int no, D3DXCOLOR color)
{
	GAGEBACK *gageback = &gagebackWk[no];

	// ���ˌ��̐ݒ�
	gageback->vertexWk[0].diffuse =
		gageback->vertexWk[1].diffuse =
		gageback->vertexWk[2].diffuse =
		gageback->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGageback(int no, int cntPattern)
{
	GAGEBACK *gageback = &gagebackWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK;
	gageback->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gageback->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gageback->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gageback->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetGageback(int nGage)
{
	GAGEBACK *gageback = &gagebackWk[nGage];
	gageback->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseGageback(void)
{
	GAGEBACK *gageback = &gagebackWk[0];
	gageback->bUse = false;
}

//=============================================================================
// GAGEBACK�擾�֐�
//=============================================================================
GAGEBACK *GetGageback(int no)
{
	return(&gagebackWk[no]);
}

