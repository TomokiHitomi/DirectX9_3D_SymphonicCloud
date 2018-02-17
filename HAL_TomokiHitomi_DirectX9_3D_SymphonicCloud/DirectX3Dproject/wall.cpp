//=============================================================================
//
// ウォール処理 [wall.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "camera.h"
#include "wall.h"
#include "input.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureWall = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;	// 頂点バッファへのポインタ

WALL					wallWk[WALL_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWall(int nType)
{
	WALL *wall = &wallWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_WALL,
			&g_pD3DTextureWall);
	}

	for (int i = 0; i < WALL_Y; i++)
	{
		for (int j = 0; j < WALL_X; j++, wall++)
		{
			// 位置・回転・スケールの設定
			wall->posWall = D3DXVECTOR3(0.0f + j * WALL_SIZE_X * 2, WALL_SIZE_Z + (i * WALL_SIZE_Z * 2), -WALL_SIZE_Z);
			wall->rotWall = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
			wall->sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexWall(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	if(g_pD3DTextureWall != NULL)
	{// テクスチャの開放
		g_pD3DTextureWall->Release();
		g_pD3DTextureWall = NULL;
	}

	if(g_pD3DVtxBuffWall != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffWall->Release();
		g_pD3DVtxBuffWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{
	WALL *wall = &wallWk[0];
	//for (int i = 0; i < WALL_MAX; i++, wall++)
	//{
	//	// 高さ移動
	//	if (GetKeyboardPress(DIK_SPACE))
	//	{
	//		wall->posWall.y += 5;
	//	}

	//	if (GetKeyboardPress(DIK_LSHIFT))
	//	{
	//		wall->posWall.y -= 5;
	//	}


	//	// Y左回転
	//	if (GetKeyboardPress(DIK_Q))
	//	{
	//		wall->rotWall.y += 0.05;
	//	}
	//	// Y右移動
	//	else if (GetKeyboardPress(DIK_E))
	//	{
	//		wall->rotWall.y -= 0.05;
	//	}


	//	// 拡大
	//	if (GetKeyboardPress(DIK_Z))
	//	{
	//		wall->sclWall += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//	// 縮小
	//	else if (GetKeyboardPress(DIK_X))
	//	{
	//		wall->sclWall -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	WALL *wall = &wallWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);


	for (int i = 0; i < WALL_MAX; i++, wall++)
	{
		/******************** ワールド変換 ********************/
		// ワールドマトリクスの初期化
		D3DXMatrixIdentity(&wall->mtxWorld);

		// 【S】スケールを反映(Multiplyは行列計算)
		D3DXMatrixScaling(&mtxScl, wall->sclWall.x, wall->sclWall.y, wall->sclWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxScl);

		// 【R】回転を反映(YawPitchRollはy,x,z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, wall->rotWall.y, wall->rotWall.x, wall->rotWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxRot);

		// 【T】平行移動を反映(オブジェクトを配置している）
		D3DXMatrixTranslation(&mtxTranslate, wall->posWall.x, wall->posWall.y, wall->posWall.z);
		D3DXMatrixMultiply(&wall->mtxWorld, &wall->mtxWorld, &mtxTranslate);

		// ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &wall->mtxWorld);


		/******************** ビューポート変換 ********************/
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureWall);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	WALL *wall = &wallWk[0];

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffWall,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-WALL_SIZE_X, 0.0f, WALL_SIZE_Z);
		pVtx[1].vtx = D3DXVECTOR3(WALL_SIZE_X, 0.0f, WALL_SIZE_Z);
		pVtx[2].vtx = D3DXVECTOR3(-WALL_SIZE_X, 0.0f, -WALL_SIZE_Z);
		pVtx[3].vtx = D3DXVECTOR3(WALL_SIZE_X, 0.0f, -WALL_SIZE_Z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffWall->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 取得関数
//=============================================================================
WALL *GetWall(int no)
{
	return(&wallWk[no]);
}