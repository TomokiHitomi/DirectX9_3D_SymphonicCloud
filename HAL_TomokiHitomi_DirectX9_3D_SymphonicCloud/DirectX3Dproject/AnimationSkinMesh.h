//=============================================================================
//
// �A�j���[�V�������� [AnimationSkinMesh.h]
// Author : Midorikawa Ryo
//
//=============================================================================
#ifndef ANIMATIONSKINMESH_H
#define ANIMATIONSKINMESH_H

#include "main.h"
#include <map>
#include <string>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIMATION_FRAME_SPEED				(60.0f/4000.0f)		// �A�j���[�V������4800�t���[����1�b�炵��(��������4000�t���[��)
#define ANIMATION_SPEED						(3.0f)				// �A�j���[�V�����̃X�s�[�h

enum ANIMATION_PATTERN
{
	ANIM_FLOAT,
	ANIM_JUMP,
	ANIM_SKIRTUP,
	ANIM_UP,
	ANIM_BLOCK,
	ANIM_DOWN,
	ANIM_DASH,
	ANIM_DEFAULT,
	ANIM_WALK,
	ANIM_MAX
};

//*****************************************************************************
// �\���̐錾
//*****************************************************************************	
// D3DXFRAME�̔h���N���X�ł���OXD3DXFRAME�N���X�̐���
namespace OX {

	// D3DXFRAME:�{�[���̍\���ł���t���[���c���[.Name�����o�͎g�p����Ȃ�
	struct OXD3DXFRAME : public D3DXFRAME
	{

		// �h���N���X�ɂ�id��offsetMatrix��2�̃����o��ǉ�
		// �{�[��ID�̓I�t�Z�b�g�s����w�肷�鎞��{�[���̎p�����X�V���鎞�A�����ĕ`��f�o�C�X�Ɋe�{�[���̃��[���h�ϊ��s���n�����ȂǂŎg�����؂ȃL�[�i���j
		DWORD id;							// id�͂���D3DXFRAME���S������{�[��ID.ID3DXSkinInfo�Ƃ����I�u�W�F�N�g���m���Ă���

		D3DXMATRIX offsetMatrix;			// �{�[���I�t�Z�b�g�s����i�[.�t���[���̏����p���ƂȂ��؂ȍs��

		// X�t�@�C���̃t���[���\���ɂ͑Ή�����{�[��������NULL�t���[�����������Ă���B���������Ή��{�[���������t���[����ID��0xffffffff�Ɠ���
		OXD3DXFRAME() : id(0xffffffff)
		{
			D3DXMatrixIdentity( &offsetMatrix );
		}
	};

	// ����ɂ���ă��b�V���̓X�L�����b�V���Ƃ��ē���??
	struct OXD3DXMESHCONTAINER : public D3DXMESHCONTAINER
	{
		// maxFaceInfl:�{�[���e����, numBoneCombinations:�{�[���R���r�l�[�V�����\���̐�, boneCombinationTable:�{�[���R���r�l�[�V�����\���̃e�[�u��
		// boneCombinationTable:�T�u���b�V���`�掞�ɕ`��f�o�C�X�ɓn��1���b�V���ɉe������{�[���Q��ID���i�[.���̍\���̂���肾���̂�getMeshContainer�֐�
		DWORD maxFaceInfl;
		DWORD numBoneCombinations;
		ID3DXBuffer *boneCombinationTable;

		OXD3DXMESHCONTAINER() : maxFaceInfl(1), numBoneCombinations(0), boneCombinationTable(0) {}
	};

	// D3DXFRAME�̐����Ɣj��,D3DXMESHCONTAINER�̐����Ɣj����4�̃��\�b�h����\��
	class AllocateHierarchy : public ID3DXAllocateHierarchy
	{
		char *copyName( const char* name )
		{
			char *n = 0;

			if ( !name || name[0] == '\0' )
			{
				n = new char[1];

				n[0] = '\0';
			}
			else
			{
				size_t len = strlen(name);

				n = new char[ strlen(name) + 1 ];

				strcpy_s( n, strlen(name) + 1, name );
			}
			return n;
		}

	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
		{
			OXD3DXFRAME *newFrame = new OXD3DXFRAME;

			newFrame->Name = copyName( Name );

			newFrame->pFrameFirstChild = 0;

			newFrame->pFrameSibling = 0;

			newFrame->pMeshContainer = 0;

			D3DXMatrixIdentity( &newFrame->TransformationMatrix );

			*ppNewFrame = newFrame;

			return D3D_OK;
		}

		// �m�ۂ�������S�����ꂢ�����ς�����Ă���
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree)
		{
			if ( pFrameToFree->pFrameFirstChild )
				DestroyFrame( pFrameToFree->pFrameFirstChild );

			if ( pFrameToFree->pFrameSibling )
				DestroyFrame( pFrameToFree->pFrameSibling );

			if ( pFrameToFree->pMeshContainer )
				DestroyMeshContainer( pFrameToFree->pMeshContainer );

			delete[] pFrameToFree->Name;

			delete pFrameToFree;

			return D3D_OK;
		}

		// D3DXMESHCONTAINER�\���̂Ƀf�[�^�����߂�
		STDMETHOD(CreateMeshContainer)(THIS_ 
			LPCSTR Name, 
			CONST D3DXMESHDATA *pMeshData, 
			CONST D3DXMATERIAL *pMaterials, 
			CONST D3DXEFFECTINSTANCE *pEffectInstances, 
			DWORD NumMaterials, 
			CONST DWORD *pAdjacency, 
			LPD3DXSKININFO pSkinInfo, 
			LPD3DXMESHCONTAINER *ppNewMeshContainer
		) {
			OXD3DXMESHCONTAINER *newCont = new OXD3DXMESHCONTAINER;

			newCont->Name = copyName( Name );

			newCont->pAdjacency = new DWORD[ pMeshData->pMesh->GetNumFaces() * 3 ];

			memset( newCont->pAdjacency, 0, pMeshData->pMesh->GetNumFaces() * 3 * sizeof(DWORD) );

			newCont->MeshData.Type = pMeshData->Type;

			pSkinInfo->ConvertToBlendedMesh(
				pMeshData->pMesh, 0, pAdjacency, newCont->pAdjacency, 0, 0, &newCont->maxFaceInfl,
				&newCont->numBoneCombinations, &newCont->boneCombinationTable, &newCont->MeshData.pMesh
			);

			newCont->NumMaterials = NumMaterials;

			newCont->pMaterials = new D3DXMATERIAL[ NumMaterials ];

			memcpy( newCont->pMaterials, pMaterials, NumMaterials * sizeof(D3DXMATERIAL) );

			newCont->pEffects = 0;

			if ( pEffectInstances )
			{
				newCont->pEffects = new D3DXEFFECTINSTANCE;

				newCont->pEffects->pEffectFilename = copyName( pEffectInstances->pEffectFilename );

				newCont->pEffects->NumDefaults = pEffectInstances->NumDefaults;

				newCont->pEffects->pDefaults = new D3DXEFFECTDEFAULT[ pEffectInstances->NumDefaults ];

				for ( DWORD i = 0; i < pEffectInstances->NumDefaults; i++ )
				{
					D3DXEFFECTDEFAULT *src  = pEffectInstances->pDefaults + i;

					D3DXEFFECTDEFAULT *dest = newCont->pEffects->pDefaults + i;

					dest->NumBytes = src->NumBytes;

					dest->Type = src->Type;

					dest->pParamName = copyName( src->pParamName );

					dest->pValue = new char[src->NumBytes];

					memcpy( dest->pValue, src->pValue, src->NumBytes );
				}
			}

			newCont->pSkinInfo = pSkinInfo;

			pSkinInfo->AddRef();

			*ppNewMeshContainer = newCont;

			return D3D_OK;
		}

		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
		{
			OXD3DXMESHCONTAINER *m = (OXD3DXMESHCONTAINER*)pMeshContainerToFree;

			m->MeshData.pMesh->Release();

			delete[] m->Name;

			delete[] m->pAdjacency;

			if ( m->pEffects )
			{
				for ( DWORD i = 0; i < m->pEffects->NumDefaults; i++ )
				{
					D3DXEFFECTDEFAULT *d = m->pEffects->pDefaults + i;

					delete[] d->pParamName;

					delete[] d->pValue;
				}
				delete[] m->pEffects->pDefaults;

				delete[] m->pEffects->pEffectFilename;

				delete m->pEffects;
			}

			delete[] m->pMaterials;

			if (m->pSkinInfo)
			{
				m->pSkinInfo->Release();
			}

			if (m->boneCombinationTable)
			{
				m->boneCombinationTable->Release();
			}

			delete m;

			return D3D_OK;
		}
	};
}

//*****************************************************************************
// �\���̐錾
//*****************************************************************************	
typedef struct
{
	// ID3DXAnimationController:FK�A�j���[�V����
	ID3DXAnimationController			*Controller = 0;
	OX::OXD3DXFRAME						*RootFrame = 0;

	// �X�L�����b�V������X�t�@�C������擾
	OX::AllocateHierarchy Allocater;

	// �{�[���R���r�l�[�V�����z��iboneCombinationTable�j���Ƃ肾��
	OX::OXD3DXMESHCONTAINER				*Container;
	D3DXBONECOMBINATION					*Combs;

	LPD3DXANIMATIONSET					*Set = new LPD3DXANIMATIONSET[ANIM_MAX];			
	std::map<DWORD, D3DXMATRIX>			CombMatrixMap;

	int									Motion;												// ���݂̃��[�V����
	double								*Period = new double[ANIM_MAX];						// ���ꂼ��̃��[�V������1���[�v�̎���
	double								Time;												// ��~����
	bool								Stop;												// ��~�t���O

} ANIMATION;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT InitAnimationSkinMesh(int Type);
//void UninitAnimationSkinMesh(void);
//OX::OXD3DXMESHCONTAINER *getMeshContainer(D3DXFRAME *frame);
//void setFrameId(OX::OXD3DXFRAME *frame, ID3DXSkinInfo *info);
//void updateCombMatrix(std::map<DWORD, D3DXMATRIX> &combMatrixMap, OX::OXD3DXFRAME *frame);
//void SetAnimation(int num, int Whose);

#endif
