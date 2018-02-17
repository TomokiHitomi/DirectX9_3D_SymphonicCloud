//=============================================================================
//
// TIMERANKING���� [timeranking.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "timeranking.h"
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
HRESULT MakeVertexTimeranking(int nNum);
void SetDiffuseTimeranking(int nNum,D3DXCOLOR color);
void SetTextureTimeranking(int nNum);
void SetTimerankingFile(void);
void CreateTimerankingFile(void);
void UpdateTimerankingFile(FILE *pfp);
void WriteTimerankingFile(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTimeranking = NULL;					// �e�N�X�`���ւ̃|���S��
TIMERANKING				timerankingWk[TIMERANKING_MAX];
int						g_nTimeranking[TIMERANKING_MAX] = {	TIMERANKING_NUM1,
															TIMERANKING_NUM2,
															TIMERANKING_NUM3,
															TIMERANKING_NUM4,
															TIMERANKING_NUM5 };
int						g_nTimerankingFp[TIMERANKING_MAX] = {0, 0, 0, 0, 0};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimeranking(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMERANKING *timeranking = &timerankingWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIMERANKING,				// �t�@�C���̖��O
			&g_pD3DTextureTimeranking);				// �ǂݍ��ރ������̃|�C���^
	}

	if (GetStage() == STAGE_RESULT)
	{
		SetTimerankingFile();
		for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
		{
			timeranking->posTimeranking = D3DXVECTOR3(
				(float)TIMERANKING_POS_X,
				(float)TIMERANKING_POS_Y + (float)TEXTURE_TIMERANKING_INTERVAL_Y * i,
				0.0f);
			timeranking->nTimeranking = g_nTimerankingFp[i];
			// ���_���̍쐬
			MakeVertexTimeranking(i);
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimeranking(void)
{
	if (g_pD3DTextureTimeranking != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTimeranking->Release();
		g_pD3DTextureTimeranking = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimeranking(void)
{
	TIMERANKING *timeranking = &timerankingWk[0];
	RESULT *result = GetResult(0);
	GAME_SYS *GetGameSys();

	for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
	{
		if (GetStage() == STAGE_GAME)
		{
			int nCount = GetGameSysCount();
			SetTimeranking(i, nCount / TIMERANKING_SECOND);
			SetDiffuseTimeranking(i, GetMagicColor());
		}
		else if (GetStage() == STAGE_RESULT)
		{
			SetDiffuseTimeranking(i , GetRankColor());
		}
		//if (g_bTimerankingMoveFlag && !result->bPosMoveFlag)
		//{
		//	g_posTimeranking += result->vecPosMove;
		//	MakeVertexTimeranking();
		//	g_bTimerankingMoveFlag = false;
		//}
		SetTextureTimeranking(i);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimeranking(void)
{
	TIMERANKING *timeranking = &timerankingWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureTimeranking);

		// �^�C��
		for (int j = 0; j < TIMERANKING_DIGIT; j++)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIMERANKING, timeranking->vertexWkTimeranking[j], sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTimeranking(int nNum)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	float fWidth = TEXTURE_TIMERANKING_SPAN;	// �����̉���
	int nTimerankingInterval = 0;

	// ��������������
	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		if (i % 2 == 0 && i != 0)
		{
			nTimerankingInterval++;
		}
		// ���_���W�̐ݒ�
		timeranking->vertexWkTimeranking[i][0].vtx.x = -fWidth * i + timeranking->posTimeranking.x - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][0].vtx.y = timeranking->posTimeranking.y;
		timeranking->vertexWkTimeranking[i][0].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][1].vtx.x = -fWidth * i + timeranking->posTimeranking.x + TEXTURE_TIMERANKING_SIZE_X - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][1].vtx.y = timeranking->posTimeranking.y;
		timeranking->vertexWkTimeranking[i][1].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][2].vtx.x = -fWidth * i + timeranking->posTimeranking.x - (TEXTURE_TIMERANKING_INTERVAL_X* nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][2].vtx.y = timeranking->posTimeranking.y + TEXTURE_TIMERANKING_SIZE_Y;
		timeranking->vertexWkTimeranking[i][2].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][3].vtx.x = -fWidth * i + timeranking->posTimeranking.x + TEXTURE_TIMERANKING_SIZE_X - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][3].vtx.y = timeranking->posTimeranking.y + TEXTURE_TIMERANKING_SIZE_Y;
		timeranking->vertexWkTimeranking[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		timeranking->vertexWkTimeranking[i][0].rhw =
		timeranking->vertexWkTimeranking[i][1].rhw =
		timeranking->vertexWkTimeranking[i][2].rhw =
		timeranking->vertexWkTimeranking[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		timeranking->vertexWkTimeranking[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		timeranking->vertexWkTimeranking[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		timeranking->vertexWkTimeranking[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		timeranking->vertexWkTimeranking[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		timeranking->vertexWkTimeranking[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTimeranking(int nNum,D3DXCOLOR color)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	// ��������������
	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		// ���ˌ��̐ݒ�
		timeranking->vertexWkTimeranking[i][0].diffuse =
		timeranking->vertexWkTimeranking[i][1].diffuse =
		timeranking->vertexWkTimeranking[i][2].diffuse =
		timeranking->vertexWkTimeranking[i][3].diffuse = color;
	}
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureTimeranking(int nNum)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	int number = timeranking->nTimeranking;

	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		timeranking->vertexWkTimeranking[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		timeranking->vertexWkTimeranking[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		timeranking->vertexWkTimeranking[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		timeranking->vertexWkTimeranking[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}

}


//=============================================================================
// �^�C���̉��Z����
//=============================================================================
void SetTimeranking(int nNum,int nTimeranking)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	//CLOSE *close = GetClose(0);
	//RESULT *result = GetResult(0);

	//if (close->bUse)
	//{
	//	g_bTimerankingStop = true;
	//}
	//if (!g_bTimerankingStop)
	//{

	timeranking->nTimeranking = nTimeranking / TIMERANKING_SECOND * 100;
	timeranking->nTimeranking += nTimeranking % TIMERANKING_SECOND;

	if (timeranking->nTimeranking > TIMERANKING_SECOND_MAX)
	{
		timeranking->nTimeranking = TIMERANKING_SECOND_MAX;
	}
	else if (timeranking->nTimeranking < 0)
	{
		timeranking->nTimeranking = 0;
	}
	SetResultTime(timeranking->nTimeranking);
	//}
}

//=============================================================================
// �^�C���̃t�@�C������������
//=============================================================================
void SetTimerankingFile(void)
{
	FILE *fp = fopen(TIMERANKING_NUM_FILE, "rb");
	
	if (fp == NULL)				// �t�@�C���I�[�v���G���[�̓t�@�C������
	{
		CreateTimerankingFile();
		fp = fopen(TIMERANKING_NUM_FILE, "rb");
		if (fp == NULL)				// �t�@�C���I�[�v���G���[�̓��^�[��
		{
			return;
		}
		else
		{
			UpdateTimerankingFile(&*fp);
			fclose(fp);										// �t�@�C������I��
			WriteTimerankingFile();
		}
	}
	else							// �t�@�C���I�[�v��
	{
		UpdateTimerankingFile(&*fp);	
		fclose(fp);										// �t�@�C������I��
		WriteTimerankingFile();
	}
}

//=============================================================================
// �^�C���̃t�@�C������������
//=============================================================================
void CreateTimerankingFile(void)
{
	FILE *fpc = fopen(TIMERANKING_NUM_FILE, "wb");	// �t�@�C���쐬
	if (fpc == NULL)									// �t�@�C������ꂽ���`�F�b�N
	{
		return;
	}

	fprintf(fpc, "%d,%d,%d,%d,%d",
		g_nTimeranking[0], g_nTimeranking[1], g_nTimeranking[2], g_nTimeranking[3], g_nTimeranking[4]);

	fclose(fpc);										// �t�@�C������I��
}

//=============================================================================
// �^�C���̃t�@�C���X�V����
//=============================================================================
void UpdateTimerankingFile(FILE *fp)
{
	int data[TIMERANKING_MAX + 1];

	fscanf(fp, "%d,%d,%d,%d,%d",					// �t�@�C���ǂݏo��
		&data[0], &data[1], &data[2], &data[3], &data[4]);
	if (GetClearFlag())
	{
		data[5] = GetResultTime();

		// �o�u���\�[�g
		for (int i = 0; i < TIMERANKING_MAX; i++)
		{
			for (int j = TIMERANKING_MAX; j > i; j--)
			{
				if (data[j] < data[j - 1])
				{
					int nTemp = data[j];
					data[j] = data[j - 1];
					data[j - 1] = nTemp;
				}
			}
		}
	}

	for (int i = 0; i < TIMERANKING_MAX; i++)
	{
		g_nTimerankingFp[i] = data[i];
	}
}

//=============================================================================
// �^�C���̃t�@�C������������
//=============================================================================
void WriteTimerankingFile(void)
{
	if (GetClearFlag())
	{
		FILE *fpw = fopen(TIMERANKING_NUM_FILE, "wb");	// �t�@�C����������
		if (fpw == NULL)									// �t�@�C�����J�������`�F�b�N
		{
			return;
		}

		// �\�[�g�f�[�^���t�@�C���Ɋi�[
		fprintf(fpw, "%d,%d,%d,%d,%d",
			g_nTimerankingFp[0], g_nTimerankingFp[1], g_nTimerankingFp[2], g_nTimerankingFp[3], g_nTimerankingFp[4]);

		fclose(fpw);										// �t�@�C������I��
	}
}