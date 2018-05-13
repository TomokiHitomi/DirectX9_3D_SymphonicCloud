//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(int no);
void SetVertexTitle(int no);
void SetDiffuseTitle(int no, float fAlpha);
void SetTextureTitle(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TITLE					titleWk[TITLE_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureTitle[TITLE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[0];

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TITLE_00,				// �t�@�C���̖��O
			&pD3DTextureTitle[0]);			// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TITLE_01,				// �t�@�C���̖��O
			&pD3DTextureTitle[1]);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����
	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		title->bUse = true;
		title->bAlpha = true;
		title->pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		title->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		title->fAlpha = TITLE_ALPHA_MAX;

		title->Texture = pD3DTextureTitle[i];			// �e�N�X�`���ւ̃G�l�~�[
		title->size = D3DXVECTOR2(TEXTURE_TITLE_SIZE_X, TEXTURE_TITLE_SIZE_Y);

		// ���_���̍쐬
		MakeVertexTitle(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	TITLE *title = &titleWk[0];

	for (int i = 0; i < TITLE_MAX; i++)
	{
		// ���������
		if (&pD3DTextureTitle[i] != NULL)
		{
			pD3DTextureTitle[i]->Release();
			pD3DTextureTitle[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	TITLE *title = &titleWk[0];

	SetSoundBgm(SOUND_BGM_TITLE);

	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		if (title->bUse)
		{
			// �^�C�g���̊O�g�̃��l��ύX
			if (i == 0)
			{
				if (title->bAlpha)
				{
					title->fAlpha -= TITLE_ALPHA_CHANGE;
					if (title->fAlpha <= TITLE_ALPHA_MIN)
					{
						title->fAlpha = TITLE_ALPHA_MIN;
						title->bAlpha = false;
					}
				}
				else
				{
					title->fAlpha += TITLE_ALPHA_CHANGE;
					if (title->fAlpha >= TITLE_ALPHA_MAX)
					{
						title->fAlpha = TITLE_ALPHA_MAX;
						title->bAlpha = true;
					}
				}
				SetDiffuseTitle(i, title->fAlpha);
#ifdef _DEBUG
				PrintDebugProc("�y TITLE �z\n");
				PrintDebugProc("Alpha:%f\n", title->fAlpha);
#endif
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		if (title->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, title->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, title->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[no];

	// ���_���W�̐ݒ�
	SetVertexTitle(no);

	// rhw�̐ݒ�
	title->vertexWk[0].rhw =
	title->vertexWk[1].rhw =
	title->vertexWk[2].rhw =
	title->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	title->vertexWk[0].diffuse =
	title->vertexWk[1].diffuse =
	title->vertexWk[2].diffuse =
	title->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	title->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	title->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLE, 0.0f );
	title->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLE );
	title->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLE, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLE );

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexTitle(int no)
{
	TITLE *title = &titleWk[no];

	title->vertexWk[0].vtx.x = title->pos.x - TEXTURE_TITLE_SIZE_X;
	title->vertexWk[0].vtx.y = title->pos.y - TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[0].vtx.z = 0.0f;

	title->vertexWk[1].vtx.x = title->pos.x + TEXTURE_TITLE_SIZE_X;
	title->vertexWk[1].vtx.y = title->pos.y - TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[1].vtx.z = 0.0f;

	title->vertexWk[2].vtx.x = title->pos.x - TEXTURE_TITLE_SIZE_X;
	title->vertexWk[2].vtx.y = title->pos.y + TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[2].vtx.z = 0.0f;

	title->vertexWk[3].vtx.x = title->pos.x + TEXTURE_TITLE_SIZE_X;
	title->vertexWk[3].vtx.y = title->pos.y + TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTitle(int no, float fAlpha)
{
	TITLE *title = &titleWk[no];

	// ���ˌ��̐ݒ�
	title->vertexWk[0].diffuse =
	title->vertexWk[1].diffuse =
	title->vertexWk[2].diffuse =
	title->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f,1.0f, fAlpha);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTitle( int no, int cntPattern )
{
	TITLE *title = &titleWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TITLE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TITLE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TITLE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TITLE;
	title->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	title->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	title->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	title->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// �^�C�g���擾�֐�
//=============================================================================
TITLE *GetTitle(int no)
{
	return(&titleWk[no]);
}

