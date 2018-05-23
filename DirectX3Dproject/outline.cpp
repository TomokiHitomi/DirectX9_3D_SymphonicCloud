//=============================================================================
//
// OUTLINE���� [outline.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "outline.h"
#include "main.h"
#include "minimap.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexOutline(int no);
void SetVertexOutline(int no);
void SetDiffuseOutline(int no, D3DXCOLOR color);
void SetTextureOutline(int no, int cntPattern);
void SetChangeCollarOutline(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
OUTLINE					outlineWk[OUTLINE_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureOutline = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitOutline(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_OUTLINE,				// �t�@�C���̖��O
			&pD3DTextureOutline);			// �ǂݍ��ރ������̃|�C���^
	}

	// OUTLINE�̏�����
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		outline->bUse = false;
		outline->pos = D3DXVECTOR3(TEXTURE_OUTLINE_POS_X, TEXTURE_OUTLINE_POS_Y, 0.0f);
		outline->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		outline->colorOutline = D3DXCOLOR(TEXTURE_OUTLINE_R, TEXTURE_OUTLINE_G, TEXTURE_OUTLINE_B, TEXTURE_OUTLINE_A);
		outline->fBaseAngle = 0.0f;
		outline->fRadius = 0.0f;

		outline->nCollarFlag = 0;

		outline->Texture = pD3DTextureOutline;			// �e�N�X�`���ւ�OUTLINE
		outline->size = D3DXVECTOR2(TEXTURE_OUTLINE_SIZE_X, TEXTURE_OUTLINE_SIZE_Y);

		// ���_���̍쐬
		MakeVertexOutline(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitOutline(void)
{
	OUTLINE *outline = &outlineWk[0];

	// ���������
	if (pD3DTextureOutline != NULL)
	{
		pD3DTextureOutline->Release();
		pD3DTextureOutline = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateOutline(void)
{
	OUTLINE *outline = &outlineWk[0];
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		if (outline->bUse)
		{
			// ��]�e�X�g
			outline->rot.z += 0.003f;

			//SetChangeCollarOutline(i);				// �J���[�ύX����
			SetVertexOutline(i);						// �ړ���̍��W�Œ��_��ݒ�
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawOutline(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		if (outline->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, outline->Texture);

			// OUTLINE�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_OUTLINE, outline->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexOutline(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[no];

	// �p�x�Ɣ��a�����߂�
	outline->fBaseAngle = atan2f(TEXTURE_OUTLINE_SIZE_Y, TEXTURE_OUTLINE_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_OUTLINE_SIZE_X, TEXTURE_OUTLINE_SIZE_Y);
	outline->fRadius = D3DXVec2Length(&temp);

	// ���_���W�̐ݒ�
	outline->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	outline->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_OUTLINE_SIZE_X, 100.0f, 0.0f);
	outline->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_OUTLINE_SIZE_Y, 0.0f);
	outline->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_OUTLINE_SIZE_X, 100.0f + TEXTURE_OUTLINE_SIZE_Y, 0.0f);
	//SetVertexOutline();

	// rhw�̐ݒ�
	outline->vertexWk[0].rhw =
	outline->vertexWk[1].rhw =
	outline->vertexWk[2].rhw =
	outline->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	outline->vertexWk[0].diffuse =
	outline->vertexWk[1].diffuse =
	outline->vertexWk[2].diffuse =
	outline->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	outline->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	outline->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE, 0.0f);
	outline->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE);
	outline->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexOutline(int no)
{
	OUTLINE *outline = &outlineWk[no];
	// ���_0:��]����
	outline->vertexWk[0].vtx.x = outline->pos.x - cosf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[0].vtx.y = outline->pos.y - sinf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	outline->vertexWk[1].vtx.x = outline->pos.x + cosf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[1].vtx.y = outline->pos.y - sinf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	outline->vertexWk[2].vtx.x = outline->pos.x - cosf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[2].vtx.y = outline->pos.y + sinf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	outline->vertexWk[3].vtx.x = outline->pos.x + cosf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[3].vtx.y = outline->pos.y + sinf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseOutline(int no, D3DXCOLOR color)
{
	OUTLINE *outline = &outlineWk[no];

	// ���ˌ��̐ݒ�
	outline->vertexWk[0].diffuse =
	outline->vertexWk[1].diffuse =
	outline->vertexWk[2].diffuse =
	outline->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureOutline(int no, int cntPattern)
{
	OUTLINE *outline = &outlineWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE;
	outline->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	outline->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	outline->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	outline->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarOutline(int no)
{
	OUTLINE *outline = &outlineWk[no];

	switch (outline->nCollarFlag)
	{
	case 0:
		outline->colorOutline -= D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		outline->colorOutline += D3DXCOLOR(0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		if (outline->colorOutline.r <= 0.0f)
		{
			outline->colorOutline.r = 0.0f;
			outline->nCollarFlag++;
		}
		if (outline->colorOutline.g >= 1.0f)
		{
			outline->colorOutline.g = 1.0f;
		}
		if (outline->colorOutline.b >= 1.0f)
		{
			outline->colorOutline.b = 1.0f;
		}
		break;
	case 1:
		outline->colorOutline -= D3DXCOLOR(0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f);
		outline->colorOutline += D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		if (outline->colorOutline.r >= 1.0f)
		{
			outline->colorOutline.r = 1.0f;
		}
		if (outline->colorOutline.g <= 0.0f)
		{
			outline->colorOutline.g = 0.0f;
			outline->nCollarFlag++;
		}
		if (outline->colorOutline.b >= 1.0f)
		{
			outline->colorOutline.b = 1.0f;
		}
		break;
	case 2:
		outline->colorOutline -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		outline->colorOutline += D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f);
		if (outline->colorOutline.r >= 1.0f)
		{
			outline->colorOutline.r = 1.0f;
		}
		if (outline->colorOutline.g >= 1.0f)
		{
			outline->colorOutline.g = 1.0f;
		}
		if (outline->colorOutline.b <= 0.0f)
		{
			outline->colorOutline.b = 0.0f;
			outline->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
int SetOutline(D3DXCOLOR colorfloat)
{
	OUTLINE *outline = &outlineWk[0];
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{	// ���g�p��T��
		if (!outline->bUse)
		{	// �L��������
			outline->bUse = true;
			SetDiffuseOutline(i, colorfloat);	// �J���[�K�p����
			return i;
		}
	}
}

//=============================================================================
// �X�V�ݒ�
//=============================================================================
void SetUpdateOutline(int nOutline, D3DXVECTOR3 pos, float fAngle)
{
	OUTLINE *outline = &outlineWk[nOutline];

	// �e���̍X�V
	outline->pos = pos;
	outline->rot.z = fAngle;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseOutline(int nOutline)
{
	OUTLINE *outline = &outlineWk[nOutline];
	outline->bUse = false;
}

//=============================================================================
// OUTLINE�擾�֐�
//=============================================================================
OUTLINE *GetOutline(int no)
{
	return(&outlineWk[no]);
}

