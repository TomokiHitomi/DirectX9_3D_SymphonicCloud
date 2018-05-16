//=============================================================================
//
// アニメーション処理 [AnimationSkinMesh.cpp]
// Author : Midorikawa Ryo
//
//=============================================================================
#include "AnimationSkinMesh.h"
#include "model.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAnimationSkinMesh(int Type)
{

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAnimationSkinMesh(void)
{
	//if (SE_Binta != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Binta->Release();
	//	SE_Binta = NULL;
	//}

	//if (SE_Up != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Up->Release();
	//	SE_Up = NULL;
	//}

	//if (SE_Down != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Down->Release();
	//	SE_Down = NULL;
	//}

	//if (SE_Dash != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Dash->Release();
	//	SE_Dash = NULL;
	//}

	//if (SE_Scream != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Scream->Release();
	//	SE_Scream = NULL;
	//}

	//if (SE_Jump != NULL)
	//{
	//	// テクスチャの開放
	//	SE_Jump->Release();
	//	SE_Jump = NULL;
	//}
}


/*******************************************************************************
* 関数名：OX::OXD3DXMESHCONTAINER *getMeshContainer( D3DXFRAME *frame )
* 引数  ：
* 戻り値：
* 説明  ：
*******************************************************************************/
// 幅優先探索 
OX::OXD3DXMESHCONTAINER *getMeshContainer( D3DXFRAME *frame )
{
   if ( frame->pMeshContainer )
		return (OX::OXD3DXMESHCONTAINER*)frame->pMeshContainer;
   if ( frame->pFrameSibling )
		return getMeshContainer( frame->pFrameSibling );
   if ( frame->pFrameFirstChild )
		return getMeshContainer( frame->pFrameFirstChild );
   return 0;
}

/*******************************************************************************
* 関数名：void setFrameId( OX::OXD3DXFRAME *frame, ID3DXSkinInfo *info ) 
* 引数  ：
* 戻り値：
* 説明  ：
*******************************************************************************/
void setFrameId( OX::OXD3DXFRAME *frame, ID3DXSkinInfo *info ) 
{
	std::map<std::string, DWORD> nameToIdMap;
	for ( DWORD i = 0; i < info->GetNumBones(); i++ )
		nameToIdMap[ info->GetBoneName( i ) ] = i;

	// staticメソッドfにそのトラバースをしてもらう.擬似的な関数内関数.ついでにボーンオフセット行列（offsetMatrix）も格納
	struct create {
		static void f( std::map<std::string, DWORD> nameToIdMap, ID3DXSkinInfo *info, OX::OXD3DXFRAME* frame ) {
			if ( nameToIdMap.find( frame->Name ) != nameToIdMap.end() ) {
				frame->id = nameToIdMap[ frame->Name ];
				frame->offsetMatrix = *info->GetBoneOffsetMatrix( frame->id );
			}
			if ( frame->pFrameFirstChild )
				f( nameToIdMap, info, (OX::OXD3DXFRAME*)frame->pFrameFirstChild );
			if ( frame->pFrameSibling )
				f( nameToIdMap, info, (OX::OXD3DXFRAME*)frame->pFrameSibling );
		}
	};
	create::f( nameToIdMap, info, frame );
}

/*******************************************************************************
* 関数名：void updateCombMatrix( std::map<DWORD, D3DXMATRIX> &combMatrixMap, OX::OXD3DXFRAME *frame )
* 引数  ：
* 戻り値：
* 説明  ：
*******************************************************************************/
void updateCombMatrix( std::map<DWORD, D3DXMATRIX> &combMatrixMap, OX::OXD3DXFRAME *frame )
{
	struct update {
		static void f( std::map<DWORD, D3DXMATRIX> &combMatrixMap, D3DXMATRIX &parentBoneMatrix, OX::OXD3DXFRAME *frame ) {
			D3DXMATRIX &localBoneMatrix = frame->TransformationMatrix;
			D3DXMATRIX boneMatrix = localBoneMatrix * parentBoneMatrix;
			if ( frame->id != 0xffffffff)
				combMatrixMap[ frame->id ] = frame->offsetMatrix * boneMatrix;
			if ( frame->pFrameFirstChild )
				f( combMatrixMap, boneMatrix, (OX::OXD3DXFRAME*)frame->pFrameFirstChild );
			if ( frame->pFrameSibling )
				f( combMatrixMap, parentBoneMatrix, (OX::OXD3DXFRAME*)frame->pFrameSibling );
		}
	};
	D3DXMATRIX iden;
	D3DXMatrixIdentity( &iden );
	update::f( combMatrixMap, iden, frame );
}

/*******************************************************************************
* 関数名：void SetAnimation(int num, int Whose)
* 引数  ：
* 戻り値：
* 説明  ：
*******************************************************************************/
void SetAnimation(int num, int Whose)
{
	//switch (num)
	//{
	//case ANIM_BLOCK:

	//	PlaySound(SE_Binta, E_DS8_FLAG_NONE);

	//	break;

	//case ANIM_DASH:

	//	PlaySound(SE_Dash, E_DS8_FLAG_NONE);

	//	break;

	//case ANIM_DOWN:

	//	PlaySound(SE_Down, E_DS8_FLAG_NONE);

	//	break;

	//case ANIM_UP:

	//	PlaySound(SE_Up, E_DS8_FLAG_NONE);

	//	break;

	//case ANIM_SKIRTUP:

	//	PlaySound(SE_Scream, E_DS8_FLAG_NONE);

	//	break;

	//case ANIM_JUMP:

	//	PlaySound(SE_Jump, E_DS8_FLAG_NONE);

	//	break;
	//}

	//switch (Whose)
	//{
	//case PLAYERS:
	//{
		//PLAYER *Player = Get_Player();
		//Player->Anim.Controller->SetTrackAnimationSet(0, Player->Anim.Set[num]);
		//Player->Anim.Controller->SetTrackPosition(0, 0);
		////FLOAT LoopTime = 3.0f;   // 3秒ループ
		////FLOAT AdjustSpeed = Player->Anim.Set[num]->GetPeriod() * LoopTime;
		////Player->Anim.Controller->SetTrackSpeed(num, AdjustSpeed);
		//Player->Anim.Time = 0.0f;

	//	break;
	//}

	//case ENEMYS:
	//{
	//	ENEMY *Enemy = Get_Enemy();
	//	Enemy->Anim.Controller->SetTrackAnimationSet(0, Enemy->Anim.Set[num]);
	//	Enemy->Anim.Controller->SetTrackPosition(0, 0);
	//	Enemy->Anim.Time = 0.0f;

	//	break;
	//}
	//}
}