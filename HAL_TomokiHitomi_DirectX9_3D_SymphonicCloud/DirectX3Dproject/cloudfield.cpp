//=============================================================================
//
// モデル処理 [cloudfield.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "cloudfield.h"
#include "main.h"
#include "input.h"
#include "camera.h"
#include "Light.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureCloudfield;		// テクスチャへのポインタ

LPD3DXMESH			g_pD3DXMeshCloudfield;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatCloudfield;	// マテリアル情報へのポインタ
DWORD				g_nNumMatCloudfield;			// マテリアル情報の数

CLOUDFIELD				cloudfieldWk[CLOUDFIELD_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCloudfield(int nType)
{
	CLOUDFIELD *cloudfield = &cloudfieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// 位置・回転・スケールの設定
	cloudfield->posCloudfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cloudfield->rotCloudfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cloudfield->sclCloudfield = D3DXVECTOR3(CLOUDFIELD_SCALE, CLOUDFIELD_SCALE, CLOUDFIELD_SCALE);

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureCloudfield = NULL;
		g_pD3DXMeshCloudfield = NULL;
		g_pD3DXBuffMatCloudfield = NULL;
		g_nNumMatCloudfield = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(CLOUDFIELD_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatCloudfield,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatCloudfield,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshCloudfield)))		// メッシュデータへのポインタ
		{
			MessageBox(NULL, "cloudfieldのモデルがありません。", NULL, MB_OK);
			return E_FAIL;
		}

		// テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice,				// デバイス
			CLOUDFIELD_TEXTURE,			// ファイル名
			&g_pD3DTextureCloudfield)))	// 読み込むメモリ（複数なら配列に）
		{
			MessageBox(NULL, "cloudfieldのテクスチャがありません。", NULL, MB_OK);
			return E_FAIL;
		};
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCloudfield(void)
{
	if(g_pD3DTextureCloudfield != NULL)
	{// テクスチャの開放
		g_pD3DTextureCloudfield->Release();
		g_pD3DTextureCloudfield = NULL;
	}

	if(g_pD3DXMeshCloudfield != NULL)
	{// メッシュの開放
		g_pD3DXMeshCloudfield->Release();
		g_pD3DXMeshCloudfield = NULL;
	}

	if(g_pD3DXBuffMatCloudfield != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatCloudfield->Release();
		g_pD3DXBuffMatCloudfield = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCloudfield(void)
{
	//CLOUDFIELD *cloudfield = &cloudfieldWk[0];
	//int pCameraMode = *GetCameraMode();
	//CAMERA *camera = GetCamera(pCameraMode);

	//if (pCameraMode == CAMERA_GAME)
	//{
	//	cloudfield->posCloudfield.x = camera->posCameraAt.x;
	//	cloudfield->posCloudfield.y = camera->posCameraAt.y + CLOUDFIELD_HEIGHT;
	//	cloudfield->posCloudfield.z = camera->posCameraAt.z;
	//}
	////cloudfield->rotCloudfield.y += CLOUDFIELD_ROT_SPEED;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCloudfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	CLOUDFIELD *cloudfield = &cloudfieldWk[0];

	//// αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//// ラインティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	SetLight(LIGHT2, TRUE);

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&cloudfield->mtxWorld);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, cloudfield->sclCloudfield.x, cloudfield->sclCloudfield.y, cloudfield->sclCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, cloudfield->rotCloudfield.y, cloudfield->rotCloudfield.x, cloudfield->rotCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, cloudfield->posCloudfield.x, cloudfield->posCloudfield.y, cloudfield->posCloudfield.z);
	D3DXMatrixMultiply(&cloudfield->mtxWorld, &cloudfield->mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &cloudfield->mtxWorld);


	/******************** ビューポート変換 ********************/
	// 現在のマテリアルを保存
	pDevice->GetMaterial(&matDef);
	// マテリアル情報に対するポインタの取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatCloudfield->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatCloudfield; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// テクスチャの設定（NULL:テクスチャ無し）
		pDevice->SetTexture(0, g_pD3DTextureCloudfield);
		// 描画
		g_pD3DXMeshCloudfield->DrawSubset(i);
	}


	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	//// ラインティングを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// αテストを無効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	SetLight(LIGHT2, FALSE);
}

//=============================================================================
// 取得関数
//=============================================================================
CLOUDFIELD *GetCloudfield(int no)
{
	return(&cloudfieldWk[no]);
}

//=============================================================================
//// メッシュ取得関数
////=============================================================================
//LPD3DXMESH *GetCloudfieldMesh(void)
//{
//	return(&g_pD3DXMeshCloudfield);
//	g_pD3DXMeshCloudfield
//}

//=============================================================================
// メッシュ取得関数
//=============================================================================
LPD3DXBASEMESH  GetCloudfieldMesh(void)
{
	LPD3DXBASEMESH lpBasemesh;
	lpBasemesh = g_pD3DXMeshCloudfield;
	return(lpBasemesh);
}