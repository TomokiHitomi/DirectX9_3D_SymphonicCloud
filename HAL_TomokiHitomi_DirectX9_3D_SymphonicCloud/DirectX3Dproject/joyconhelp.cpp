//=============================================================================
//
// JOYCONHELP���� [joyconhelp.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "joyconhelp.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexJoyconhelp(int no);
void SetTextureJoyconhelp( int no, int cntPattern );	//
void SetVertexJoyconhelp(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
JOYCONHELP					joyconhelpWk[JOYCONHELP_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureJoyconhelp = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitJoyconhelp(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_JOYCONHELP,				// �t�@�C���̖��O
			&pD3DTextureJoyconhelp);			// �ǂݍ��ރ������̃|�C���^
	}

	// JOYCONHELP�̏�����
	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		joyconhelp->bUse = true;
		joyconhelp->pos = D3DXVECTOR3((float)JOYCONHELP_POS_X, (float)JOYCONHELP_POS_Y, 0.0f);
		joyconhelp->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		joyconhelp->nCountAnim = 0;
		joyconhelp->nPatternAnim = 0;

		joyconhelp->Texture = pD3DTextureJoyconhelp;			// �e�N�X�`���ւ�JOYCONHELP
		joyconhelp->size = D3DXVECTOR2(TEXTURE_JOYCONHELP_SIZE_X, TEXTURE_JOYCONHELP_SIZE_Y);

		// ���_���̍쐬
		MakeVertexJoyconhelp(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitJoyconhelp(void)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	// ���������
	if (pD3DTextureJoyconhelp != NULL)
	{
		pD3DTextureJoyconhelp->Release();
		pD3DTextureJoyconhelp = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateJoyconhelp(void)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_CAP))
	{
		joyconhelp->bUse = joyconhelp->bUse ? false : true;
	}

	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		if (joyconhelp->bUse)
		{
			// �A�j���[�V����
			joyconhelp->nCountAnim++;
			if ((joyconhelp->nCountAnim % TIME_ANIMATION_JOYCONHELP) == 0)
			{
				// �p�^�[���̐؂�ւ�
				joyconhelp->nPatternAnim = (joyconhelp->nPatternAnim + 1) % ANIM_PATTERN_NUM_JOYCONHELP;
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureJoyconhelp(i, joyconhelp->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexJoyconhelp(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawJoyconhelp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		if (joyconhelp->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, joyconhelp->Texture);

			// JOYCONHELP�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_JOYCONHELP, joyconhelp->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexJoyconhelp(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	// ���_���W�̐ݒ�
	joyconhelp->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	joyconhelp->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_JOYCONHELP_SIZE_X, 100.0f, 0.0f);
	joyconhelp->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_JOYCONHELP_SIZE_Y, 0.0f);
	joyconhelp->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_JOYCONHELP_SIZE_X, 100.0f+TEXTURE_JOYCONHELP_SIZE_Y, 0.0f);
	//SetVertexJoyconhelp();

	// rhw�̐ݒ�
	joyconhelp->vertexWk[0].rhw =
	joyconhelp->vertexWk[1].rhw =
	joyconhelp->vertexWk[2].rhw =
	joyconhelp->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	joyconhelp->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);

	// �e�N�X�`�����W�̐ݒ�
	joyconhelp->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	joyconhelp->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP, 0.0f );
	joyconhelp->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP );
	joyconhelp->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureJoyconhelp( int no, int cntPattern )
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP;
	joyconhelp->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	joyconhelp->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	joyconhelp->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	joyconhelp->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexJoyconhelp(int no)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	joyconhelp->vertexWk[0].vtx.x = joyconhelp->pos.x - TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[0].vtx.y = joyconhelp->pos.y - TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[0].vtx.z = 0.0f;

	joyconhelp->vertexWk[1].vtx.x = joyconhelp->pos.x + TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[1].vtx.y = joyconhelp->pos.y - TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[1].vtx.z = 0.0f;

	joyconhelp->vertexWk[2].vtx.x = joyconhelp->pos.x - TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[2].vtx.y = joyconhelp->pos.y + TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[2].vtx.z = 0.0f;

	joyconhelp->vertexWk[3].vtx.x = joyconhelp->pos.x + TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[3].vtx.y = joyconhelp->pos.y + TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// JOYCONHELP�擾�֐�
//=============================================================================
JOYCONHELP *GetJoyconhelp(int no)
{
	return(&joyconhelpWk[no]);
}

