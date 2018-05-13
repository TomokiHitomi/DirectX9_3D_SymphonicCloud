//=============================================================================
//
// アニメーション処理 [AnimationSkinMesh.h]
// Author : Midorikawa Ryo
//
//=============================================================================
#ifndef ANIMATIONSKINMESH_H
#define ANIMATIONSKINMESH_H

#include "main.h"
#include <map>
#include <string>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIMATION_FRAME_SPEED				(60.0f/4000.0f)		// アニメーションは4800フレームで1秒らしい(微調整で4000フレーム)
#define ANIMATION_SPEED						(3.0f)				// アニメーションのスピード

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
// 構造体宣言
//*****************************************************************************	
// D3DXFRAMEの派生クラスであるOXD3DXFRAMEクラスの生成
namespace OX {

	// D3DXFRAME:ボーンの構成であるフレームツリー.Nameメンバは使用されない
	struct OXD3DXFRAME : public D3DXFRAME
	{

		// 派生クラスにはidとoffsetMatrixの2つのメンバを追加
		// ボーンIDはオフセット行列を指定する時やボーンの姿勢を更新する時、そして描画デバイスに各ボーンのワールド変換行列を渡す時などで使われる大切なキー（鍵）
		DWORD id;							// idはそのD3DXFRAMEが担当するボーンID.ID3DXSkinInfoというオブジェクトが知っている

		D3DXMATRIX offsetMatrix;			// ボーンオフセット行列を格納.フレームの初期姿勢となる大切な行列

		// Xファイルのフレーム構造には対応するボーンが無いNULLフレームが混じっている。そういう対応ボーンが無いフレームのIDは0xffffffffと統一
		OXD3DXFRAME() : id(0xffffffff)
		{
			D3DXMatrixIdentity( &offsetMatrix );
		}
	};

	// これによってメッシュはスキンメッシュとして動く??
	struct OXD3DXMESHCONTAINER : public D3DXMESHCONTAINER
	{
		// maxFaceInfl:ボーン影響数, numBoneCombinations:ボーンコンビネーション構造体数, boneCombinationTable:ボーンコンビネーション構造体テーブル
		// boneCombinationTable:サブメッシュ描画時に描画デバイスに渡す1メッシュに影響するボーン群のIDが格納.この構造体を取りだすのがgetMeshContainer関数
		DWORD maxFaceInfl;
		DWORD numBoneCombinations;
		ID3DXBuffer *boneCombinationTable;

		OXD3DXMESHCONTAINER() : maxFaceInfl(1), numBoneCombinations(0), boneCombinationTable(0) {}
	};

	// D3DXFRAMEの生成と破棄,D3DXMESHCONTAINERの生成と破棄の4つのメソッドから構成
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

		// 確保した物を全部きれいさっぱり消している
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

		// D3DXMESHCONTAINER構造体にデータを収める
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
// 構造体宣言
//*****************************************************************************	
typedef struct
{
	// ID3DXAnimationController:FKアニメーション
	ID3DXAnimationController			*Controller = 0;
	OX::OXD3DXFRAME						*RootFrame = 0;

	// スキンメッシュ情報をXファイルから取得
	OX::AllocateHierarchy Allocater;

	// ボーンコンビネーション配列（boneCombinationTable）をとりだす
	OX::OXD3DXMESHCONTAINER				*Container;
	D3DXBONECOMBINATION					*Combs;

	LPD3DXANIMATIONSET					*Set = new LPD3DXANIMATIONSET[ANIM_MAX];			
	std::map<DWORD, D3DXMATRIX>			CombMatrixMap;

	int									Motion;												// 現在のモーション
	double								*Period = new double[ANIM_MAX];						// それぞれのモーションの1ループの時間
	double								Time;												// 停止時間
	bool								Stop;												// 停止フラグ

} ANIMATION;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT InitAnimationSkinMesh(int Type);
//void UninitAnimationSkinMesh(void);
//OX::OXD3DXMESHCONTAINER *getMeshContainer(D3DXFRAME *frame);
//void setFrameId(OX::OXD3DXFRAME *frame, ID3DXSkinInfo *info);
//void updateCombMatrix(std::map<DWORD, D3DXMATRIX> &combMatrixMap, OX::OXD3DXFRAME *frame);
//void SetAnimation(int num, int Whose);

#endif
