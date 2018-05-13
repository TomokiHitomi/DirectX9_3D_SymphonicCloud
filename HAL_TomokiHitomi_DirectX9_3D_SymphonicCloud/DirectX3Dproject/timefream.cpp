//=============================================================================
//
// TIMEFREAM���� [timefream.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "timefream.h"
#include "main.h"
#include "magic.h"
#include "ranking.h"
#include "stage.h"
#include "rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTimefream(int no);

void SetVertexTimefream(int no);
void SetDiffuseTimefream(int no, D3DXCOLOR color);
void SetTextureTimefream(int no, int cntPattern);
void SetChangeCollarTimefream(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TIMEFREAM					timefreamWk[TIMEFREAM_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureTimefream[TIMEFREAM_TEXTURE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimefream(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIMEFREAMBACK,				// �t�@�C���̖��O
			&pD3DTextureTimefream[TIMEFREAM_BACK]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIMEFREAMFREAM,				// �t�@�C���̖��O
			&pD3DTextureTimefream[TIMEFREAM_FREAM]);			// �ǂݍ��ރ������̃|�C���^
	}

	// TIMEFREAM�̏�����
	for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
	{

		switch (i)
		{
		case TIMEFREAM_BACK:
			timefream->Texture = pD3DTextureTimefream[TIMEFREAM_BACK];	// �e�N�X�`���ւ�TIMEFREAM
			timefream->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, TEXTURE_TIMEFREAM_A);
			break;
		case TIMEFREAM_FREAM:
			timefream->Texture = pD3DTextureTimefream[TIMEFREAM_FREAM];	// �e�N�X�`���ւ�TIMEFREAM
			timefream->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		timefream->bUse = true;
		timefream->bPatternFlag = false;
		timefream->nPatternAnim = 0;
		timefream->nCollarFlag = 0;
		timefream->pos = D3DXVECTOR3(TEXTURE_TIMEFREAM_POS_X, TEXTURE_TIMEFREAM_POS_Y, 0.0f);
		timefream->size = D3DXVECTOR2(TEXTURE_TIMEFREAM_SIZE_X, TEXTURE_TIMEFREAM_SIZE_Y);
		timefream->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_���̍쐬
		MakeVertexTimefream(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimefream(void)
{
	for (int i = 0; i < TIMEFREAM_TEXTURE_MAX; i++)
	{
		// ���������
		if (pD3DTextureTimefream[i] != NULL)
		{
			pD3DTextureTimefream[i]->Release();
			pD3DTextureTimefream[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimefream(void)
{
	TIMEFREAM *timefream = &timefreamWk[0];
	for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
	{
		if (timefream->bUse)
		{
			if (i == TIMEFREAM_FREAM)
			{
				if (GetStage() == STAGE_GAME)
				{
					SetDiffuseTimefream(i, GetMagicColor());
				}
				else
				{
					SetDiffuseTimefream(i, GetRankColor());
				}			
			}
			else
			{
				SetDiffuseTimefream(i, timefream->xColor);
			}
			if (GetStage() == STAGE_RESULT)
			{
				timefream->pos.y = TEXTURE_TIMEFREAM_RESULT_POS_Y;
				SetVertexTimefream(i);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimefream(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[0];
	if (GetStage() == STAGE_RESULT && !GetClearFlag())
	{

	}
	else
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
		{
			if (timefream->bUse)
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, timefream->Texture);

				// TIMEFREAM�̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIMEFREAM, timefream->vertexWk, sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTimefream(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[no];

	// ���_���W�̐ݒ�
	SetVertexTimefream(no);

	// rhw�̐ݒ�
	timefream->vertexWk[0].rhw =
	timefream->vertexWk[1].rhw =
	timefream->vertexWk[2].rhw =
	timefream->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	timefream->vertexWk[0].diffuse =
	timefream->vertexWk[1].diffuse =
	timefream->vertexWk[2].diffuse =
	timefream->vertexWk[3].diffuse = timefream->xColor;


	if (timefream->bPatternFlag)
	{
		SetTextureTimefream(no, no - 1);
	}
	else
	{
		// �e�N�X�`�����W�̐ݒ�
		timefream->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		timefream->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		timefream->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		timefream->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexTimefream(int no)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	timefream->vertexWk[0].vtx.x = timefream->pos.x - timefream->size.x;
	timefream->vertexWk[0].vtx.y = timefream->pos.y - timefream->size.y;
	timefream->vertexWk[0].vtx.z = 0.0f;

	timefream->vertexWk[1].vtx.x = timefream->pos.x + timefream->size.x;
	timefream->vertexWk[1].vtx.y = timefream->pos.y - timefream->size.y;
	timefream->vertexWk[1].vtx.z = 0.0f;

	timefream->vertexWk[2].vtx.x = timefream->pos.x - timefream->size.x;
	timefream->vertexWk[2].vtx.y = timefream->pos.y + timefream->size.y;
	timefream->vertexWk[2].vtx.z = 0.0f;

	timefream->vertexWk[3].vtx.x = timefream->pos.x + timefream->size.x;
	timefream->vertexWk[3].vtx.y = timefream->pos.y + timefream->size.y;
	timefream->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTimefream(int no, D3DXCOLOR color)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	// ���ˌ��̐ݒ�
	timefream->vertexWk[0].diffuse =
		timefream->vertexWk[1].diffuse =
		timefream->vertexWk[2].diffuse =
		timefream->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTimefream(int no, int cntPattern)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM;
	timefream->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timefream->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timefream->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timefream->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarTimefream(int no)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	switch (timefream->nCollarFlag)
	{
	case 0:
		timefream->xColor -= D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		timefream->xColor += D3DXCOLOR(0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		if (timefream->xColor.r <= 0.0f)
		{
			timefream->xColor.r = 0.0f;
			timefream->nCollarFlag++;
		}
		if (timefream->xColor.g >= 1.0f)
		{
			timefream->xColor.g = 1.0f;
		}
		if (timefream->xColor.b >= 1.0f)
		{
			timefream->xColor.b = 1.0f;
		}
		break;
	case 1:
		timefream->xColor -= D3DXCOLOR(0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f);
		timefream->xColor += D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		if (timefream->xColor.r >= 1.0f)
		{
			timefream->xColor.r = 1.0f;
		}
		if (timefream->xColor.g <= 0.0f)
		{
			timefream->xColor.g = 0.0f;
			timefream->nCollarFlag++;
		}
		if (timefream->xColor.b >= 1.0f)
		{
			timefream->xColor.b = 1.0f;
		}
		break;
	case 2:
		timefream->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		timefream->xColor += D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f);
		if (timefream->xColor.r >= 1.0f)
		{
			timefream->xColor.r = 1.0f;
		}
		if (timefream->xColor.g >= 1.0f)
		{
			timefream->xColor.g = 1.0f;
		}
		if (timefream->xColor.b <= 0.0f)
		{
			timefream->xColor.b = 0.0f;
			timefream->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetTimefream(int nGage)
{
	TIMEFREAM *timefream = &timefreamWk[nGage];
	timefream->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseTimefream(void)
{
	TIMEFREAM *timefream = &timefreamWk[0];
	timefream->bUse = false;
}

//=============================================================================
// TIMEFREAM�擾�֐�
//=============================================================================
TIMEFREAM *GetTimefream(int no)
{
	return(&timefreamWk[no]);
}

