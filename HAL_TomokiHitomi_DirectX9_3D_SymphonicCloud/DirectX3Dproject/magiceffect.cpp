//=============================================================================
//
// MAGICEFFECT���� [magiceffect.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "magiceffect.h"
#include "magic.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexMagiceffect(int no);
void SetTextureMagiceffect(int no, int cntPattern);	//
void SetVertexMagiceffect(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
MAGICEFFECT					magiceffectWk[MAGICEFFECT_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureMagiceffect = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMagiceffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_MAGICEFFECT,				// �t�@�C���̖��O
			&pD3DTextureMagiceffect);			// �ǂݍ��ރ������̃|�C���^
	}

	// MAGICEFFECT�̏�����
	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (i < MAGICEFFECT_TYPE_MAX)
		{
			if (i == MAGICEFFECT_NORMAL)
			{
				magiceffect->pos = D3DXVECTOR3(MAGICEFFECT_NORMAL_POS_X, MAGICEFFECT_POS_Y + (i - 1) * MAGIC_GAGE_INTERVAL, 0.0f);
				magiceffect->bUse = true;
				magiceffect->size = D3DXVECTOR2(TEXTURE_MAGICEFFECT_SIZE_X, TEXTURE_MAGICEFFECT_SIZE_Y);

			}
			else if (i == MAGICEFFECT_MAGIC_L || i == MAGICEFFECT_MAGIC_R)
			{
				magiceffect->bUse = false;
				if(i == MAGICEFFECT_MAGIC_L)magiceffect->pos = 
					D3DXVECTOR3(MAGICEFFECT_POS_MAGIC_L_X, MAGICEFFECT_POS_MAGIC_L_Y, 0.0f);
				if (i == MAGICEFFECT_MAGIC_R)magiceffect->pos =
					D3DXVECTOR3(MAGICEFFECT_POS_MAGIC_R_X, MAGICEFFECT_POS_MAGIC_R_Y, 0.0f);
				magiceffect->size = D3DXVECTOR2(MAGICEFFECT_POS_MAGIC_SIZE_X, MAGICEFFECT_POS_MAGIC_SIZE_Y);
			}
			else
			{
				magiceffect->pos = D3DXVECTOR3(MAGICEFFECT_POS_X, MAGICEFFECT_POS_Y + i * MAGIC_GAGE_INTERVAL, 0.0f);
				magiceffect->bUse = false;
				magiceffect->size = D3DXVECTOR2(TEXTURE_MAGICEFFECT_SIZE_X, TEXTURE_MAGICEFFECT_SIZE_Y);
			}
			magiceffect->nType = i;
		}
		else
		{
			magiceffect->bUse = false;
			magiceffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			magiceffect->nType = 0;
		}

		magiceffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		magiceffect->nCountAnim = 0;
		magiceffect->nPatternAnim = 0;

		magiceffect->Texture = pD3DTextureMagiceffect;			// �e�N�X�`���ւ�MAGICEFFECT

		// ���_���̍쐬
		MakeVertexMagiceffect(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	// ���������
	if (pD3DTextureMagiceffect != NULL)
	{
		pD3DTextureMagiceffect->Release();
		pD3DTextureMagiceffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];
	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (magiceffect->bUse)
		{
			magiceffect->nCountAnim++;
			// �A�j���[�V����
			if (i == GetTypeMagic() || i >= MAGICEFFECT_NORMAL)
			{	
				if ((magiceffect->nCountAnim % TIME_ANIMATION_MAGICEFFECT) == 0)
				{
					if (i == MAGICEFFECT_MAGIC_L || i == MAGICEFFECT_MAGIC_R)
					{
						int nTypeMagic = GetTypeMagic();
						if (magiceffect->nType != nTypeMagic)
						{
							magiceffect->nCountAnim = 0;
							magiceffect->nType = nTypeMagic;
						}
					}
					// �p�^�[���̐؂�ւ�
					//magiceffect->nPatternAnim = (magiceffect->nPatternAnim + 1) % ANIM_PATTERN_NUM_MAGICEFFECT;
					magiceffect->nPatternAnim = (magiceffect->nPatternAnim + 1)
						% TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT
						+ TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT * magiceffect->nType;
				}
			}
			else
			{
				magiceffect->nPatternAnim = TEXTURE_PATTERN_NONACTIVE_MAGICEFFECT
					+ TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT*magiceffect->nType;
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureMagiceffect(i, magiceffect->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexMagiceffect(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagiceffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (magiceffect->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, magiceffect->Texture);

			// MAGICEFFECT�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MAGICEFFECT, magiceffect->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexMagiceffect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	// ���_���W�̐ݒ�
	magiceffect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	magiceffect->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGICEFFECT_SIZE_X, 100.0f, 0.0f);
	magiceffect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MAGICEFFECT_SIZE_Y, 0.0f);
	magiceffect->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGICEFFECT_SIZE_X, 100.0f + TEXTURE_MAGICEFFECT_SIZE_Y, 0.0f);
	//SetVertexMagiceffect();

	// rhw�̐ݒ�
	magiceffect->vertexWk[0].rhw =
	magiceffect->vertexWk[1].rhw =
	magiceffect->vertexWk[2].rhw =
	magiceffect->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	magiceffect->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);

	// �e�N�X�`�����W�̐ݒ�
	magiceffect->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	magiceffect->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT, 0.0f);
	magiceffect->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT);
	magiceffect->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureMagiceffect(int no, int cntPattern)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT;
	magiceffect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	magiceffect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	magiceffect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	magiceffect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexMagiceffect(int no)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	magiceffect->vertexWk[0].vtx.x = magiceffect->pos.x - magiceffect->size.x;
	magiceffect->vertexWk[0].vtx.y = magiceffect->pos.y - magiceffect->size.y;
	magiceffect->vertexWk[0].vtx.z = 0.0f;

	magiceffect->vertexWk[1].vtx.x = magiceffect->pos.x + magiceffect->size.x;
	magiceffect->vertexWk[1].vtx.y = magiceffect->pos.y - magiceffect->size.y;
	magiceffect->vertexWk[1].vtx.z = 0.0f;

	magiceffect->vertexWk[2].vtx.x = magiceffect->pos.x - magiceffect->size.x;
	magiceffect->vertexWk[2].vtx.y = magiceffect->pos.y + magiceffect->size.y;
	magiceffect->vertexWk[2].vtx.z = 0.0f;

	magiceffect->vertexWk[3].vtx.x = magiceffect->pos.x + magiceffect->size.x;
	magiceffect->vertexWk[3].vtx.y = magiceffect->pos.y + magiceffect->size.y;
	magiceffect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetMagiceffect(int nMagicEffect)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[nMagicEffect];
	magiceffect->bUse = true;

	magiceffectWk[MAGICEFFECT_MAGIC_L].bUse = true;
	magiceffectWk[MAGICEFFECT_MAGIC_R].bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];
	magiceffect->bUse = false;
}

//=============================================================================
// MAGICEFFECT�擾�֐�
//=============================================================================
MAGICEFFECT *GetMagiceffect(int no)
{
	return(&magiceffectWk[no]);
}

