//=============================================================================
//
// POINTER���� [pointer.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "pointer.h"
#include "main.h"
#include "minimap.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPointer(int no);
void SetVertexPointer(int no);
void SetDiffusePointer(int no, D3DXCOLOR color);
void SetTexturePointer(int no, int cntPattern);
void SetChangeCollarPointer(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
POINTER					pointerWk[POINTER_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTexturePointer[TEXTURE_GAME_POINTER_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPointer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_POINTER,				// �t�@�C���̖��O
			&pD3DTexturePointer[POINTER_TEXTURE_NORMAIL]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_OUTLINE,				// �t�@�C���̖��O
			&pD3DTexturePointer[POINTER_TEXTURE_OUTLINE]);			// �ǂݍ��ރ������̃|�C���^
	}

	// POINTER�̏�����
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		pointer->bUse = false;
		pointer->pos = D3DXVECTOR3(TEXTURE_POINTER_POS_X, TEXTURE_POINTER_POS_Y, 0.0f);
		pointer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pointer->colorPointer = D3DXCOLOR(TEXTURE_POINTER_R, TEXTURE_POINTER_G, TEXTURE_POINTER_B, TEXTURE_POINTER_A);
		pointer->fBaseAngle = 0.0f;
		pointer->fRadius = 0.0f;

		pointer->nCollarFlag = 0;
		pointer->nTex = 0;

		pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_NORMAIL];			// �e�N�X�`���ւ�POINTER
		pointer->size = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);

		// ���_���̍쐬
		MakeVertexPointer(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPointer(void)
{
	POINTER *pointer = &pointerWk[0];

	for (int i = 0; i < TEXTURE_GAME_POINTER_MAX; i++, pointer++)
	{
		// ���������
		if (pD3DTexturePointer[i] != NULL)
		{
			pD3DTexturePointer[i]->Release();
			pD3DTexturePointer[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePointer(void)
{
	POINTER *pointer = &pointerWk[0];
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->bUse)
		{
			// ��]�e�X�g
			pointer->rot.z += 0.003f;

			//SetChangeCollarPointer(i);				// �J���[�ύX����
			SetVertexPointer(i);						// �ړ���̍��W�Œ��_��ݒ�
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->bUse)
		{
			// �e�N�X�`���[�؂�ւ�
			if (pointer->nTex == POINTER_TEXTURE_NORMAIL)
			{	// �|�C���^�[�\��
				pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_NORMAIL];
			}
			else if (pointer->nTex == POINTER_TEXTURE_OUTLINE)
			{	// �A�E�g���C���\��
				pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_OUTLINE];
			}
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pointer->Texture);

			// POINTER�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POINTER, pointer->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPointer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[no];

	// �p�x�Ɣ��a�����߂�
	pointer->fBaseAngle = atan2f(TEXTURE_POINTER_SIZE_Y, TEXTURE_POINTER_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);
	pointer->fRadius = D3DXVec2Length(&temp);

	// ���_���W�̐ݒ�
	pointer->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pointer->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_POINTER_SIZE_X, 100.0f, 0.0f);
	pointer->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_POINTER_SIZE_Y, 0.0f);
	pointer->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_POINTER_SIZE_X, 100.0f + TEXTURE_POINTER_SIZE_Y, 0.0f);
	//SetVertexPointer();

	// rhw�̐ݒ�
	pointer->vertexWk[0].rhw =
	pointer->vertexWk[1].rhw =
	pointer->vertexWk[2].rhw =
	pointer->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	pointer->vertexWk[0].diffuse =
	pointer->vertexWk[1].diffuse =
	pointer->vertexWk[2].diffuse =
	pointer->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pointer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER);
	pointer->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexPointer(int no)
{
	POINTER *pointer = &pointerWk[no];
	// ���_0:��]����
	pointer->vertexWk[0].vtx.x = pointer->pos.x - cosf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[0].vtx.y = pointer->pos.y - sinf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	pointer->vertexWk[1].vtx.x = pointer->pos.x + cosf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[1].vtx.y = pointer->pos.y - sinf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	pointer->vertexWk[2].vtx.x = pointer->pos.x - cosf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[2].vtx.y = pointer->pos.y + sinf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	pointer->vertexWk[3].vtx.x = pointer->pos.x + cosf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[3].vtx.y = pointer->pos.y + sinf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffusePointer(int no, D3DXCOLOR color)
{
	POINTER *pointer = &pointerWk[no];

	// ���ˌ��̐ݒ�
	pointer->vertexWk[0].diffuse =
	pointer->vertexWk[1].diffuse =
	pointer->vertexWk[2].diffuse =
	pointer->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePointer(int no, int cntPattern)
{
	POINTER *pointer = &pointerWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_POINTER;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_POINTER;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER;
	pointer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	pointer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	pointer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	pointer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarPointer(int no)
{
	POINTER *pointer = &pointerWk[no];

	switch (pointer->nCollarFlag)
	{
	case 0:
		pointer->colorPointer -= D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		pointer->colorPointer += D3DXCOLOR(0.0f, TEXTURE_POINTER_CHANGE_COLLAR, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		if (pointer->colorPointer.r <= 0.0f)
		{
			pointer->colorPointer.r = 0.0f;
			pointer->nCollarFlag++;
		}
		if (pointer->colorPointer.g >= 1.0f)
		{
			pointer->colorPointer.g = 1.0f;
		}
		if (pointer->colorPointer.b >= 1.0f)
		{
			pointer->colorPointer.b = 1.0f;
		}
		break;
	case 1:
		pointer->colorPointer -= D3DXCOLOR(0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f);
		pointer->colorPointer += D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		if (pointer->colorPointer.r >= 1.0f)
		{
			pointer->colorPointer.r = 1.0f;
		}
		if (pointer->colorPointer.g <= 0.0f)
		{
			pointer->colorPointer.g = 0.0f;
			pointer->nCollarFlag++;
		}
		if (pointer->colorPointer.b >= 1.0f)
		{
			pointer->colorPointer.b = 1.0f;
		}
		break;
	case 2:
		pointer->colorPointer -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		pointer->colorPointer += D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f);
		if (pointer->colorPointer.r >= 1.0f)
		{
			pointer->colorPointer.r = 1.0f;
		}
		if (pointer->colorPointer.g >= 1.0f)
		{
			pointer->colorPointer.g = 1.0f;
		}
		if (pointer->colorPointer.b <= 0.0f)
		{
			pointer->colorPointer.b = 0.0f;
			pointer->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
int SetPointer(D3DXCOLOR colorfloat)
{
	POINTER *pointer = &pointerWk[0];
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{	// ���g�p��T��
		if (!pointer->bUse)
		{	// �L��������
			pointer->bUse = true;
			SetDiffusePointer(i, colorfloat);	// �J���[�K�p����
			return i;
		}
	}
}

//=============================================================================
// �X�V�ݒ�
//=============================================================================
void SetUpdatePointer(int nPointer, int nTex, D3DXVECTOR3 pos, float fAngle)
{
	POINTER *pointer = &pointerWk[nPointer];

	// �e���̍X�V
	pointer->pos = pos;
	pointer->rot.z = fAngle;
	pointer->nTex = nTex;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleasePointer(int nPointer)
{
	POINTER *pointer = &pointerWk[nPointer];
	pointer->bUse = false;
}

//=============================================================================
// POINTER�擾�֐�
//=============================================================================
POINTER *GetPointer(int no)
{
	return(&pointerWk[no]);
}

