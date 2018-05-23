//=============================================================================
//
// �^�C�g������ [result.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(int no);
void SetTextureResult( int no, int cntPattern );	//
void SetVertexResult(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
RESULT					resultWk[RESULT_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureResult = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[0];

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_RESULT00,				// �t�@�C���̖��O
			&pD3DTextureResult);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����
	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		result->bUse = true;
		result->bPosMoveFlag = true;
		result->pos = D3DXVECTOR3((float)RESULT_POS_X, (float)RESULT_POS_Y, 0.0f);
		result->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		result->vecPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		result->nCountAnim = 0;
		result->nPatternAnim = 0;

		result->Texture = pD3DTextureResult;			// �e�N�X�`���ւ̃G�l�~�[
		result->size = D3DXVECTOR2(TEXTURE_RESULT00_SIZE_X, TEXTURE_RESULT00_SIZE_Y);

		// ���_���̍쐬
		MakeVertexResult(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	RESULT *result = &resultWk[0];

	// ���������
	if (pD3DTextureResult != NULL)
	{
		pD3DTextureResult->Release();
		pD3DTextureResult = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	RESULT *result = &resultWk[0];

	// �X�e�[�W�J�ڃe�X�g
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, R_BUTTON_A))
	{
		SetFade(FADE_OUT, STAGE_TITLE);
	}

	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		if (result->bUse)
		{
			SetSoundBgm(SOUND_BGM_RESULT);
			if (result->bPosMoveFlag)
			{
				result->vecPosMove = D3DXVECTOR3(0.0f, (float)RESULT_MOVE_UI, 0.0f);
				result->bPosMoveFlag = false;
			}

			// �A�j���[�V����
			result->nCountAnim++;
			if ((result->nCountAnim % TIME_ANIMATION_RESULT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				result->nPatternAnim = (result->nPatternAnim + 1) % ANIM_PATTERN_NUM_RESULT;
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureResult(i, result->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexResult(i);

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		if (result->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, result->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, result->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[no];

	// ���_���W�̐ݒ�
	result->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	result->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_RESULT00_SIZE_X, 100.0f, 0.0f);
	result->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_RESULT00_SIZE_Y, 0.0f);
	result->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_RESULT00_SIZE_X, 100.0f+TEXTURE_RESULT00_SIZE_Y, 0.0f);
	//SetVertexResult();

	// rhw�̐ݒ�
	result->vertexWk[0].rhw =
	result->vertexWk[1].rhw =
	result->vertexWk[2].rhw =
	result->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	result->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	result->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	result->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_RESULT, 0.0f );
	result->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_RESULT );
	result->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_RESULT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_RESULT );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureResult( int no, int cntPattern )
{
	RESULT *result = &resultWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RESULT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RESULT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RESULT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RESULT;
	result->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	result->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	result->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	result->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexResult(int no)
{
	RESULT *result = &resultWk[no];

	result->vertexWk[0].vtx.x = result->pos.x - TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[0].vtx.y = result->pos.y - TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[0].vtx.z = 0.0f;

	result->vertexWk[1].vtx.x = result->pos.x + TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[1].vtx.y = result->pos.y - TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[1].vtx.z = 0.0f;

	result->vertexWk[2].vtx.x = result->pos.x - TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[2].vtx.y = result->pos.y + TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[2].vtx.z = 0.0f;

	result->vertexWk[3].vtx.x = result->pos.x + TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[3].vtx.y = result->pos.y + TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �^�C�g���擾�֐�
//=============================================================================
RESULT *GetResult(int no)
{
	return(&resultWk[no]);
}

