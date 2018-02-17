//=============================================================================
//
// TIME���� [time.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "time.h"
#include "result.h"
#include "game.h"
#include "magic.h"
#include "stage.h"
#include "ranking.h"
#include "rank.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTime(void);
void SetDiffuseTime(D3DXCOLOR color);
void SetTextureTime(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTime = NULL;					// �e�N�X�`���ւ̃|���S��
VERTEX_2D				g_vertexWkTime[TIME_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N
D3DXVECTOR3				g_posTime;									// �|���S���̈ړ���

int						g_nTime;									// �^�C��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTime(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIME,				// �t�@�C���̖��O
			&g_pD3DTextureTime);				// �ǂݍ��ރ������̃|�C���^
	}

	if (GetStage() == STAGE_RESULT)
	{
		g_posTime = D3DXVECTOR3((float)TIME_POS_X, (float)TIME_RESULT_POS_Y, 0.0f);

		if (GetClearFlag())
		{
			g_nTime = GetResultTime();
		}
		else
		{
			g_nTime = TIME_MAX;
		}
	}
	else
	{
		g_posTime = D3DXVECTOR3((float)TIME_POS_X, (float)TIME_POS_Y, 0.0f);
		g_nTime = 0;
	}

	// ���_���̍쐬
	MakeVertexTime();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTime(void)
{
	if (g_pD3DTextureTime != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTime->Release();
		g_pD3DTextureTime = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTime(void)
{
	RESULT *result = GetResult(0);
	GAME_SYS *GetGameSys();

	if (GetStage() == STAGE_GAME)
	{
		int nCount = GetGameSysCount();
		SetTime(nCount / TIME_SECOND);
		SetDiffuseTime(GetMagicColor());
	}
	else if (GetStage() == STAGE_RESULT && GetClearFlag())
	{
		SetDiffuseTime(GetRankColor());
	}
	//if (g_bTimeMoveFlag && !result->bPosMoveFlag)
	//{
	//	g_posTime += result->vecPosMove;
	//	MakeVertexTime();
	//	g_bTimeMoveFlag = false;
	//}


	SetTextureTime();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (GetStage() == STAGE_RESULT && !GetClearFlag())
	{

	}
	else
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureTime);

		// �^�C��
		for (int i = 0; i < TIME_DIGIT; i++)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIME, g_vertexWkTime[i], sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTime(void)
{
	float fWidth = TEXTURE_TIME_SPAN;	// �����̉���
	int nTimeInterval = 0;

	// ��������������
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		if (i % 2 == 0 && i != 0)
		{
			nTimeInterval++;
		}
		// ���_���W�̐ݒ�
		g_vertexWkTime[i][0].vtx.x = -fWidth * i + g_posTime.x - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][0].vtx.y = g_posTime.y;
		g_vertexWkTime[i][0].vtx.z = 0.0f;
		g_vertexWkTime[i][1].vtx.x = -fWidth * i + g_posTime.x + TEXTURE_TIME_SIZE_X - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][1].vtx.y = g_posTime.y;
		g_vertexWkTime[i][1].vtx.z = 0.0f;
		g_vertexWkTime[i][2].vtx.x = -fWidth * i + g_posTime.x - (TEXTURE_TIME_INTERVAL* nTimeInterval);
		g_vertexWkTime[i][2].vtx.y = g_posTime.y + TEXTURE_TIME_SIZE_Y;
		g_vertexWkTime[i][2].vtx.z = 0.0f;
		g_vertexWkTime[i][3].vtx.x = -fWidth * i + g_posTime.x + TEXTURE_TIME_SIZE_X - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][3].vtx.y = g_posTime.y + TEXTURE_TIME_SIZE_Y;
		g_vertexWkTime[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWkTime[i][0].rhw =
		g_vertexWkTime[i][1].rhw =
		g_vertexWkTime[i][2].rhw =
		g_vertexWkTime[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkTime[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkTime[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTime[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTime[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTime[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTime(D3DXCOLOR color)
{
	// ��������������
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// ���ˌ��̐ݒ�
		g_vertexWkTime[i][0].diffuse =
			g_vertexWkTime[i][1].diffuse =
			g_vertexWkTime[i][2].diffuse =
			g_vertexWkTime[i][3].diffuse = color;
	}
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureTime(void)
{
	int number = g_nTime;

	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		g_vertexWkTime[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkTime[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkTime[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkTime[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}

}


//=============================================================================
// �^�C���̉��Z����
//=============================================================================
void SetTime(int nTime)
{
	g_nTime = nTime / TIME_SECOND * 100;
	g_nTime += nTime % TIME_SECOND;

	if (g_nTime > TIME_MAX)
	{
		g_nTime = TIME_MAX;
	}
	else if (g_nTime < 0)
	{
		g_nTime = 0;
	}

	if (GetClearFlag())
	{
		SetResultTime(g_nTime);
	}
}