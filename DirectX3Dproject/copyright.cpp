//=============================================================================
//
// COPYRIGHT���� [copyright.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "copyright.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCopyright(int no);
void SetTextureCopyright( int no, int cntPattern );	//
void SetVertexCopyright(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
COPYRIGHT					copyrightWk[COPYRIGHT_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureCopyright = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCopyright(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_COPYRIGHT,				// �t�@�C���̖��O
			&pD3DTextureCopyright);			// �ǂݍ��ރ������̃|�C���^
	}

	// COPYRIGHT�̏�����
	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		copyright->bUse = true;
		copyright->pos = D3DXVECTOR3((float)COPYRIGHT_POS_X, (float)COPYRIGHT_POS_Y, 0.0f);
		copyright->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		copyright->nCountAnim = 0;
		copyright->nPatternAnim = 0;

		copyright->Texture = pD3DTextureCopyright;			// �e�N�X�`���ւ�COPYRIGHT
		copyright->size = D3DXVECTOR2(TEXTURE_COPYRIGHT_SIZE_X, TEXTURE_COPYRIGHT_SIZE_Y);

		// ���_���̍쐬
		MakeVertexCopyright(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCopyright(void)
{
	COPYRIGHT *copyright = &copyrightWk[0];

	// ���������
	if (pD3DTextureCopyright != NULL)
	{
		pD3DTextureCopyright->Release();
		pD3DTextureCopyright = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCopyright(void)
{
	COPYRIGHT *copyright = &copyrightWk[0];
	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		if (copyright->bUse == true)
		{
			// �A�j���[�V����
			copyright->nCountAnim++;
			if ((copyright->nCountAnim % TIME_ANIMATION_COPYRIGHT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				copyright->nPatternAnim = (copyright->nPatternAnim + 1) % ANIM_PATTERN_NUM_COPYRIGHT;
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureCopyright(i, copyright->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexCopyright(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCopyright(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		if (copyright->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, copyright->Texture);

			// COPYRIGHT�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_COPYRIGHT, copyright->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCopyright(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[no];

	// ���_���W�̐ݒ�
	copyright->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	copyright->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_COPYRIGHT_SIZE_X, 100.0f, 0.0f);
	copyright->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	copyright->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_COPYRIGHT_SIZE_X, 100.0f+TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	//SetVertexCopyright();

	// rhw�̐ݒ�
	copyright->vertexWk[0].rhw =
	copyright->vertexWk[1].rhw =
	copyright->vertexWk[2].rhw =
	copyright->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	copyright->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);

	// �e�N�X�`�����W�̐ݒ�
	copyright->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	copyright->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT, 0.0f );
	copyright->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT );
	copyright->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureCopyright( int no, int cntPattern )
{
	COPYRIGHT *copyright = &copyrightWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT;
	copyright->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	copyright->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	copyright->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	copyright->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexCopyright(int no)
{
	COPYRIGHT *copyright = &copyrightWk[no];

	copyright->vertexWk[0].vtx.x = copyright->pos.x - TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[0].vtx.y = copyright->pos.y - TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[0].vtx.z = 0.0f;

	copyright->vertexWk[1].vtx.x = copyright->pos.x + TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[1].vtx.y = copyright->pos.y - TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[1].vtx.z = 0.0f;

	copyright->vertexWk[2].vtx.x = copyright->pos.x - TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[2].vtx.y = copyright->pos.y + TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[2].vtx.z = 0.0f;

	copyright->vertexWk[3].vtx.x = copyright->pos.x + TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[3].vtx.y = copyright->pos.y + TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// COPYRIGHT�擾�֐�
//=============================================================================
COPYRIGHT *GetCopyright(int no)
{
	return(&copyrightWk[no]);
}

