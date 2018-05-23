//=============================================================================
//
// GAGESELECT���� [gageselect.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "gageselect.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGageselect(int no);
void SetTextureGageselect(int no, int cntPattern);	//
void SetVertexGageselect(int no);
void SetDiffuseGageselect(int no);
void SetChangeCollarGageselect(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAGESELECT					gageselectWk[GAGESELECT_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureGageselect = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGageselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_GAGESELECT,				// �t�@�C���̖��O
			&pD3DTextureGageselect);			// �ǂݍ��ރ������̃|�C���^
	}

	// GAGESELECT�̏�����
	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		switch(i)
		{
		case GAGESELECT_MAGIC:
			gageselect->bUse = false;
			gageselect->pos = D3DXVECTOR3(TEXTURE_GAGESELECT_MAGIC_POS_X, TEXTURE_GAGESELECT_MAGIC_POS_Y, 0.0f);
			gageselect->rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);
			gageselect->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		case GAGESELECT_BULLET:
			gageselect->bUse = true;
			gageselect->pos = D3DXVECTOR3(TEXTURE_GAGESELECT_BULLET_POS_X, TEXTURE_GAGESELECT_BULLET_POS_Y, 0.0f);
			gageselect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			gageselect->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		gageselect->nCountAnim = 0;
		gageselect->nPatternAnim = 0;

		gageselect->Texture = pD3DTextureGageselect;			// �e�N�X�`���ւ�GAGESELECT
		gageselect->size = D3DXVECTOR2(TEXTURE_GAGESELECT_SIZE_X, TEXTURE_GAGESELECT_SIZE_Y);

		// ���_���̍쐬
		MakeVertexGageselect(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];

	// ���������
	if (pD3DTextureGageselect != NULL)
	{
		pD3DTextureGageselect->Release();
		pD3DTextureGageselect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];
	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		if (gageselect->bUse)
		{
			if (i == GAGESELECT_MAGIC)
			{
				float fMove = TEXTURE_GAGESELECT_MAGIC_POS_Y + TEXTURE_GAGESELECT_MAGIC_INTERVAL * GetTypeMagic();

				fMove -= gageselect->pos.y;
				gageselect->pos.y += fMove * TEXTURE_GAGESELECT_MOVESPEED;
			}
			SetChangeCollarGageselect(i);
			SetDiffuseGageselect(i);
			// �e�N�X�`�����W��ݒ�
			SetTextureGageselect(i, gageselect->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexGageselect(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGageselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		if (gageselect->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, gageselect->Texture);

			// GAGESELECT�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGESELECT, gageselect->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGageselect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[no];

	// �p�x�Ɣ��a�����߂�
	gageselect->fBaseAngle = atan2f(TEXTURE_GAGESELECT_SIZE_Y, TEXTURE_GAGESELECT_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_GAGESELECT_SIZE_X, TEXTURE_GAGESELECT_SIZE_Y);
	gageselect->fRadius = D3DXVec2Length(&temp);

	// ���_���W�̐ݒ�
	gageselect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	gageselect->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_GAGESELECT_SIZE_X, 100.0f, 0.0f);
	gageselect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_GAGESELECT_SIZE_Y, 0.0f);
	gageselect->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_GAGESELECT_SIZE_X, 100.0f + TEXTURE_GAGESELECT_SIZE_Y, 0.0f);
	//SetVertexGageselect();

	// rhw�̐ݒ�
	gageselect->vertexWk[0].rhw =
	gageselect->vertexWk[1].rhw =
	gageselect->vertexWk[2].rhw =
	gageselect->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	gageselect->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);

	// �e�N�X�`�����W�̐ݒ�
	gageselect->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	gageselect->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT, 0.0f);
	gageselect->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT);
	gageselect->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGageselect(int no, int cntPattern)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT;
	gageselect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gageselect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gageselect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gageselect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// ���ˌ��̐ݒ�
	gageselect->vertexWk[0].diffuse =
		gageselect->vertexWk[1].diffuse =
		gageselect->vertexWk[2].diffuse =
		gageselect->vertexWk[3].diffuse = gageselect->xColor;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// ���_0:��]����
	gageselect->vertexWk[0].vtx.x = gageselect->pos.x - cosf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[0].vtx.y = gageselect->pos.y - sinf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	gageselect->vertexWk[1].vtx.x = gageselect->pos.x + cosf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[1].vtx.y = gageselect->pos.y - sinf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	gageselect->vertexWk[2].vtx.x = gageselect->pos.x - cosf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[2].vtx.y = gageselect->pos.y + sinf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	gageselect->vertexWk[3].vtx.x = gageselect->pos.x + cosf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[3].vtx.y = gageselect->pos.y + sinf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	switch (gageselect->nCollarFlag)
	{
	case 0:
		gageselect->xColor -= D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		gageselect->xColor += D3DXCOLOR(0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		if (gageselect->xColor.r <= 0.0f)
		{
			gageselect->xColor.r = 0.0f;
			gageselect->nCollarFlag++;
		}
		if (gageselect->xColor.g >= 1.0f)
		{
			gageselect->xColor.g = 1.0f;
		}
		if (gageselect->xColor.b >= 1.0f)
		{
			gageselect->xColor.b = 1.0f;
		}
		break;
	case 1:
		gageselect->xColor -= D3DXCOLOR(0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f);
		gageselect->xColor += D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		if (gageselect->xColor.r >= 1.0f)
		{
			gageselect->xColor.r = 1.0f;
		}
		if (gageselect->xColor.g <= 0.0f)
		{
			gageselect->xColor.g = 0.0f;
			gageselect->nCollarFlag++;
		}
		if (gageselect->xColor.b >= 1.0f)
		{
			gageselect->xColor.b = 1.0f;
		}
		break;
	case 2:
		gageselect->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		gageselect->xColor += D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f);
		if (gageselect->xColor.r >= 1.0f)
		{
			gageselect->xColor.r = 1.0f;
		}
		if (gageselect->xColor.g >= 1.0f)
		{
			gageselect->xColor.g = 1.0f;
		}
		if (gageselect->xColor.b <= 0.0f)
		{
			gageselect->xColor.b = 0.0f;
			gageselect->nCollarFlag = 0;
		}
		break;
	}
}


//=============================================================================
// �L���ݒ�
//=============================================================================
void SetGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[GAGESELECT_MAGIC];
	gageselect->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];
	gageselect->bUse = false;
}

//=============================================================================
// GAGESELECT�擾�֐�
//=============================================================================
GAGESELECT *GetGageselect(int no)
{
	return(&gageselectWk[no]);
}

