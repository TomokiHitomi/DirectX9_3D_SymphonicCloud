//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "player.h"
#include "model.h"

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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER				playerWk[PLAYER_MAX];
D3DXMATRIX			g_mtxWorldPlayer;
CSkinMesh			m_CSkinMeshPlayer;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int nType)
{
	PLAYER *player = &playerWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̐ݒ�
	player->prs.pos = D3DXVECTOR3(PLAYER_POS_X, 0.0f, PLAYER_POS_Z);
	player->prs.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	player->prs.scl = D3DXVECTOR3(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);

	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	if (nType == 0)
	{
		m_CSkinMeshPlayer.Init(pDevice, PLAYER_PLAYER);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//���b�V���I�u�W�F�N�g�Ǘ��N���X�������Ǘ�
	m_CSkinMeshPlayer.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = GetPlayer(0);
#ifdef _DEBUG
	PrintDebugProc("�y PLAYER �z\n");
#endif
	MODEL *model = GetModel(0);


	player->prs.pos = model->posModel;
	player->prs.rot = model->rotModel;

	m_CSkinMeshPlayer.Update(g_mtxWorldPlayer);


#ifdef _DEBUG
	PrintDebugProc("PlayerPos[X:%f Y:%f Z:%f]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_CSkinMeshPlayer.Draw(pDevice, player->prs);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// �擾�֐�
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}