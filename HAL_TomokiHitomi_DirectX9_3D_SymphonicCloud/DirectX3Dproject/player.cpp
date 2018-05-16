//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "player.h"
#include "model.h"
#include "Light.h"

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
		player->m_CSkinMesh.Init(pDevice, PLAYER_PLAYER);
	}

	player->m_CSkinMesh.ChangeAnim((DWORD)ANIME01);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	//���b�V���I�u�W�F�N�g�Ǘ��N���X�������Ǘ�
	player->m_CSkinMesh.Release();
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

	player->m_CSkinMesh.Update(g_mtxWorldPlayer);


#ifdef _DEBUG
	PrintDebugProc("Pos[X:%f Y:%f Z:%f]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	PrintDebugProc("Anime[%d]\n",player->m_CSkinMesh.GetAnimTrack());
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	//SetLight(LIGHT2, TRUE);
	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	player->m_CSkinMesh.Draw(pDevice, player->prs);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//SetLight(LIGHT2, FALSE);
}

//=============================================================================
// �A�j���[�V�����ݒ�֐�
//=============================================================================
void SetPlayerAnime(int nPlayer,DWORD dAnime)
{
	PLAYER *player = GetPlayer(nPlayer);
	if (dAnime != player->m_CSkinMesh.GetAnimTrack())
	{
		player->m_CSkinMesh.ChangeAnim(dAnime);
	};
}

//=============================================================================
// �擾�֐�
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}