//=============================================================================
//
// VERSION���� [version.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "version.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexVersion(int no);
void SetTextureVersion( int no, int cntPattern );	//
void SetVertexVersion(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
VERSION					versionWk[VERSION_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureVersion = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitVersion(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_VERSION,				// �t�@�C���̖��O
			&pD3DTextureVersion);			// �ǂݍ��ރ������̃|�C���^
	}

	// VERSION�̏�����
	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		version->bUse = true;
		version->pos = D3DXVECTOR3((float)VERSION_POS_X, (float)VERSION_POS_Y, 0.0f);
		version->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		version->nCountAnim = 0;
		version->nPatternAnim = 0;

		version->Texture = pD3DTextureVersion;			// �e�N�X�`���ւ�VERSION
		version->size = D3DXVECTOR2(TEXTURE_VERSION_SIZE_X, TEXTURE_VERSION_SIZE_Y);

		// ���_���̍쐬
		MakeVertexVersion(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitVersion(void)
{
	VERSION *version = &versionWk[0];

	// ���������
	if (pD3DTextureVersion != NULL)
	{
		pD3DTextureVersion->Release();
		pD3DTextureVersion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateVersion(void)
{
	VERSION *version = &versionWk[0];
	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		if (version->bUse == true)
		{
			// �A�j���[�V����
			version->nCountAnim++;
			if ((version->nCountAnim % TIME_ANIMATION_VERSION) == 0)
			{
				// �p�^�[���̐؂�ւ�
				version->nPatternAnim = (version->nPatternAnim + 1) % ANIM_PATTERN_NUM_VERSION;
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureVersion(i, version->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexVersion(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawVersion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		if (version->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, version->Texture);

			// VERSION�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_VERSION, version->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexVersion(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[no];

	// ���_���W�̐ݒ�
	version->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	version->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_VERSION_SIZE_X, 100.0f, 0.0f);
	version->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_VERSION_SIZE_Y, 0.0f);
	version->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_VERSION_SIZE_X, 100.0f+TEXTURE_VERSION_SIZE_Y, 0.0f);
	//SetVertexVersion();

	// rhw�̐ݒ�
	version->vertexWk[0].rhw =
	version->vertexWk[1].rhw =
	version->vertexWk[2].rhw =
	version->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	version->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);

	// �e�N�X�`�����W�̐ݒ�
	version->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	version->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_VERSION, 0.0f );
	version->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_VERSION );
	version->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_VERSION, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_VERSION );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureVersion( int no, int cntPattern )
{
	VERSION *version = &versionWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_VERSION;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_VERSION;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_VERSION;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_VERSION;
	version->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	version->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	version->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	version->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexVersion(int no)
{
	VERSION *version = &versionWk[no];

	version->vertexWk[0].vtx.x = version->pos.x - TEXTURE_VERSION_SIZE_X;
	version->vertexWk[0].vtx.y = version->pos.y - TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[0].vtx.z = 0.0f;

	version->vertexWk[1].vtx.x = version->pos.x + TEXTURE_VERSION_SIZE_X;
	version->vertexWk[1].vtx.y = version->pos.y - TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[1].vtx.z = 0.0f;

	version->vertexWk[2].vtx.x = version->pos.x - TEXTURE_VERSION_SIZE_X;
	version->vertexWk[2].vtx.y = version->pos.y + TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[2].vtx.z = 0.0f;

	version->vertexWk[3].vtx.x = version->pos.x + TEXTURE_VERSION_SIZE_X;
	version->vertexWk[3].vtx.y = version->pos.y + TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// VERSION�擾�֐�
//=============================================================================
VERSION *GetVersion(int no)
{
	return(&versionWk[no]);
}

