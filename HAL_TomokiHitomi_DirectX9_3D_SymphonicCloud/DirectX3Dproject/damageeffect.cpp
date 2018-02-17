//=============================================================================
//
// DAMAGEEFFECT���� [damageeffect.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "damageeffect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexDamageeffect(int no);
HRESULT ReInitDamageeffect(int nDamageeffect);
void SetTextureDamageeffect( int no, int cntPattern );	//
void SetVertexDamageeffect(int no);
void SetDiffuseDamageeffect(int no, D3DXCOLOR color);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
DAMAGEEFFECT			damageeffectWk[DAMAGEEFFECT_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureDamageeffect = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDamageeffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_DAMAGEEFFECT,				// �t�@�C���̖��O
			&pD3DTextureDamageeffect);			// �ǂݍ��ރ������̃|�C���^
	}

	// DAMAGEEFFECT�̏�����
	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		ReInitDamageeffect(i);
		damageeffect->pos = D3DXVECTOR3((float)DAMAGEEFFECT_POS_X, (float)DAMAGEEFFECT_POS_Y, 0.0f);
		damageeffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		damageeffect->xColor = D3DXCOLOR(TEXTURE_DAMAGEEFFECT_R, TEXTURE_DAMAGEEFFECT_G, TEXTURE_DAMAGEEFFECT_B, TEXTURE_DAMAGEEFFECT_A);
		damageeffect->nCountAnim = 0;
		damageeffect->nPatternAnim = 0;

		damageeffect->Texture = pD3DTextureDamageeffect;			// �e�N�X�`���ւ�DAMAGEEFFECT
		damageeffect->size = D3DXVECTOR2(TEXTURE_DAMAGEEFFECT_SIZE_X, TEXTURE_DAMAGEEFFECT_SIZE_Y);

		// ���_���̍쐬
		MakeVertexDamageeffect(i);
	}

	return S_OK;
}

//=============================================================================
// �ď���������
//=============================================================================
HRESULT ReInitDamageeffect(int nDamageeffect)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[nDamageeffect];
	damageeffect->bUse = false;
	damageeffect->bActive = false;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	// ���������
	if (pD3DTextureDamageeffect != NULL)
	{
		pD3DTextureDamageeffect->Release();
		pD3DTextureDamageeffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];
	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		if (damageeffect->bUse)
		{
			// �A�j���[�V����
			damageeffect->nCountAnim++;
			if ((damageeffect->nCountAnim % TIME_ANIMATION_DAMAGEEFFECT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				damageeffect->nPatternAnim = (damageeffect->nPatternAnim + 1) % ANIM_PATTERN_NUM_DAMAGEEFFECT;
			}

			if (damageeffect->bActive)
			{
				damageeffect->xColor.a += DAMAGEEFFECT_ALPHA_UP;
				if (damageeffect->xColor.a > 1.0f)
				{
					damageeffect->xColor.a = 1.0f;
					damageeffect->bActive = false;
				}
			}
			else
			{
				damageeffect->xColor.a -= DAMAGEEFFECT_ALPHA_DOWN;
				if (damageeffect->xColor.a < 0.0f)
				{
					damageeffect->xColor.a = 0.0f;
					ReInitDamageeffect(i);
				}
			}

			// �e�N�X�`�����W��ݒ�
			SetTextureDamageeffect(i, damageeffect->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexDamageeffect(i);
			// ���ˌ���ݒ�
			SetDiffuseDamageeffect(i, damageeffect->xColor);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDamageeffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		if (damageeffect->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, damageeffect->Texture);

			// DAMAGEEFFECT�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_DAMAGEEFFECT, damageeffect->vertexWk, sizeof(VERTEX_2D));
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
HRESULT MakeVertexDamageeffect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// ���_���W�̐ݒ�
	damageeffect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	damageeffect->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_DAMAGEEFFECT_SIZE_X, 100.0f, 0.0f);
	damageeffect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_DAMAGEEFFECT_SIZE_Y, 0.0f);
	damageeffect->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_DAMAGEEFFECT_SIZE_X, 100.0f+TEXTURE_DAMAGEEFFECT_SIZE_Y, 0.0f);
	//SetVertexDamageeffect();

	// rhw�̐ݒ�
	damageeffect->vertexWk[0].rhw =
	damageeffect->vertexWk[1].rhw =
	damageeffect->vertexWk[2].rhw =
	damageeffect->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	damageeffect->vertexWk[0].diffuse =
	damageeffect->vertexWk[1].diffuse =
	damageeffect->vertexWk[2].diffuse =
	damageeffect->vertexWk[3].diffuse = damageeffect->xColor;

	// �e�N�X�`�����W�̐ݒ�
	damageeffect->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	damageeffect->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT, 0.0f );
	damageeffect->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT );
	damageeffect->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT );

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseDamageeffect(int no, D3DXCOLOR color)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// ���ˌ��̐ݒ�
	damageeffect->vertexWk[0].diffuse =
		damageeffect->vertexWk[1].diffuse =
		damageeffect->vertexWk[2].diffuse =
		damageeffect->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureDamageeffect( int no, int cntPattern )
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT;
	damageeffect->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	damageeffect->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	damageeffect->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	damageeffect->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexDamageeffect(int no)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	damageeffect->vertexWk[0].vtx.x = damageeffect->pos.x - TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[0].vtx.y = damageeffect->pos.y - TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[0].vtx.z = 0.0f;

	damageeffect->vertexWk[1].vtx.x = damageeffect->pos.x + TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[1].vtx.y = damageeffect->pos.y - TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[1].vtx.z = 0.0f;

	damageeffect->vertexWk[2].vtx.x = damageeffect->pos.x - TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[2].vtx.y = damageeffect->pos.y + TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[2].vtx.z = 0.0f;

	damageeffect->vertexWk[3].vtx.x = damageeffect->pos.x + TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[3].vtx.y = damageeffect->pos.y + TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// Damageeffect�̐ݒu�֐�
//=============================================================================
void SetDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];
	damageeffect->bUse = true;
	damageeffect->bActive = true;
}

//=============================================================================
// DAMAGEEFFECT�擾�֐�
//=============================================================================
DAMAGEEFFECT *GetDamageeffect(int no)
{
	return(&damageeffectWk[no]);
}

