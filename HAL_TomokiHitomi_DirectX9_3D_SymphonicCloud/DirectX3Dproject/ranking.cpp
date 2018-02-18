//=============================================================================
//
// RANKING���� [ranking.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "magic.h"
#include "rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexRanking(int no);

void SetVertexRanking(int no);
void SetDiffuseRanking(int no, D3DXCOLOR color);
void SetTextureRanking(int no, int cntPattern);
void SetChangeCollarRanking(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
RANKING					rankingWk[RANKING_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureRanking[RANKING_TEXTURE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRanking(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[0];

	if (type == 0)
	{	
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RANKINGBACK,				// �t�@�C���̖��O
			&pD3DTextureRanking[RANKING_BACK]);			// �ǂݍ��ރ������̃|�C���^
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RANKINGFREAM,				// �t�@�C���̖��O
			&pD3DTextureRanking[RANKING_FREAM]);			// �ǂݍ��ރ������̃|�C���^
	}

	// RANKING�̏�����
	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{

		switch (i)
		{
		case RANKING_BACK:
			ranking->Texture = pD3DTextureRanking[RANKING_BACK];	// �e�N�X�`���ւ�RANKING
			ranking->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, TEXTURE_RANKING_A);
			break;
		case RANKING_FREAM:
			ranking->Texture = pD3DTextureRanking[RANKING_FREAM];	// �e�N�X�`���ւ�RANKING
			ranking->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		ranking->bUse = true;
		ranking->bPatternFlag = false;
		ranking->nPatternAnim = 0;
		ranking->nCollarFlag = 0;
		ranking->pos = D3DXVECTOR3(TEXTURE_RANKING_POS_X, TEXTURE_RANKING_POS_Y, 0.0f);
		ranking->size = D3DXVECTOR2(TEXTURE_RANKING_SIZE_X, TEXTURE_RANKING_SIZE_Y);
		ranking->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_���̍쐬
		MakeVertexRanking(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitRanking(void)
{
	for (int i = 0; i < RANKING_TEXTURE_MAX; i++)
	{
		// ���������
		if (pD3DTextureRanking[i] != NULL)
		{
			pD3DTextureRanking[i]->Release();
			pD3DTextureRanking[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRanking(void)
{
	RANKING *ranking = &rankingWk[0];
	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{
		if (ranking->bUse)
		{
			if (i == RANKING_FREAM)
			{
				//SetChangeCollarRanking(i);
				SetDiffuseRanking(i, GetRankColor());
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{
		if (ranking->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, ranking->Texture);

			// RANKING�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RANKING, ranking->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexRanking(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[no];

	// ���_���W�̐ݒ�
	SetVertexRanking(no);

	// rhw�̐ݒ�
	ranking->vertexWk[0].rhw =
	ranking->vertexWk[1].rhw =
	ranking->vertexWk[2].rhw =
	ranking->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	ranking->vertexWk[0].diffuse =
	ranking->vertexWk[1].diffuse =
	ranking->vertexWk[2].diffuse =
	ranking->vertexWk[3].diffuse = ranking->xColor;


	if (ranking->bPatternFlag)
	{
		SetTextureRanking(no, no - 1);
	}
	else
	{
		// �e�N�X�`�����W�̐ݒ�
		ranking->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		ranking->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		ranking->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		ranking->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexRanking(int no)
{
	RANKING *ranking = &rankingWk[no];

	ranking->vertexWk[0].vtx.x = ranking->pos.x - ranking->size.x;
	ranking->vertexWk[0].vtx.y = ranking->pos.y - ranking->size.y;
	ranking->vertexWk[0].vtx.z = 0.0f;

	ranking->vertexWk[1].vtx.x = ranking->pos.x + ranking->size.x;
	ranking->vertexWk[1].vtx.y = ranking->pos.y - ranking->size.y;
	ranking->vertexWk[1].vtx.z = 0.0f;

	ranking->vertexWk[2].vtx.x = ranking->pos.x - ranking->size.x;
	ranking->vertexWk[2].vtx.y = ranking->pos.y + ranking->size.y;
	ranking->vertexWk[2].vtx.z = 0.0f;

	ranking->vertexWk[3].vtx.x = ranking->pos.x + ranking->size.x;
	ranking->vertexWk[3].vtx.y = ranking->pos.y + ranking->size.y;
	ranking->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseRanking(int no, D3DXCOLOR color)
{
	RANKING *ranking = &rankingWk[no];

	// ���ˌ��̐ݒ�
	ranking->vertexWk[0].diffuse =
		ranking->vertexWk[1].diffuse =
		ranking->vertexWk[2].diffuse =
		ranking->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureRanking(int no, int cntPattern)
{
	RANKING *ranking = &rankingWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RANKING;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RANKING;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RANKING;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RANKING;
	ranking->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ranking->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ranking->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ranking->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarRanking(int no)
{
	RANKING *ranking = &rankingWk[no];

	switch (ranking->nCollarFlag)
	{
	case 0:
		ranking->xColor -= D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		ranking->xColor += D3DXCOLOR(0.0f, TEXTURE_RANKING_CHANGE_COLLAR, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		if (ranking->xColor.r <= 0.0f)
		{
			ranking->xColor.r = 0.0f;
			ranking->nCollarFlag++;
		}
		if (ranking->xColor.g >= 1.0f)
		{
			ranking->xColor.g = 1.0f;
		}
		if (ranking->xColor.b >= 1.0f)
		{
			ranking->xColor.b = 1.0f;
		}
		break;
	case 1:
		ranking->xColor -= D3DXCOLOR(0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f);
		ranking->xColor += D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		if (ranking->xColor.r >= 1.0f)
		{
			ranking->xColor.r = 1.0f;
		}
		if (ranking->xColor.g <= 0.0f)
		{
			ranking->xColor.g = 0.0f;
			ranking->nCollarFlag++;
		}
		if (ranking->xColor.b >= 1.0f)
		{
			ranking->xColor.b = 1.0f;
		}
		break;
	case 2:
		ranking->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		ranking->xColor += D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f);
		if (ranking->xColor.r >= 1.0f)
		{
			ranking->xColor.r = 1.0f;
		}
		if (ranking->xColor.g >= 1.0f)
		{
			ranking->xColor.g = 1.0f;
		}
		if (ranking->xColor.b <= 0.0f)
		{
			ranking->xColor.b = 0.0f;
			ranking->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �J���[�擾�֐�
//=============================================================================
D3DXCOLOR GetRankingColor(void)
{
	RANKING *ranking = &rankingWk[RANKING_FREAM];
	return(ranking->xColor);
}


//=============================================================================
// �L���ݒ�
//=============================================================================
void SetRanking(int nGage)
{
	RANKING *ranking = &rankingWk[nGage];
	ranking->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseRanking(void)
{
	RANKING *ranking = &rankingWk[0];
	ranking->bUse = false;
}

//=============================================================================
// RANKING�擾�֐�
//=============================================================================
RANKING *GetRanking(int no)
{
	return(&rankingWk[no]);
}

