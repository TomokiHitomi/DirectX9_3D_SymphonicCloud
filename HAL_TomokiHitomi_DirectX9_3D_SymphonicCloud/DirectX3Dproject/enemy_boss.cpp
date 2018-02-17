//=============================================================================
//
// エネミーモデル（ボス）処理 [enemy_boss.cpp]
// Author : GP11B243 24 人見友基 
//
//=============================================================================
#include "enemy.h"
#include "enemy_boss.h"
#include "main.h"
#include "calculate.h"

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
//**********************************2*******************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureEnemy_boss;		// テクスチャへのポインタ
D3DMATERIAL9		*g_pD3DMatMeshEnemy_boss;

LPD3DXMESH			g_pD3DXMeshEnemy_boss;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatEnemy_boss;	// マテリアル情報へのポインタ
DWORD				g_nNumMatEnemy_boss;			// マテリアル情報の数
D3DXMATRIX			g_mtxWorldEnemy_boss;			// ワールドマトリックス

ENEMY_BOSS		enemy_bossWk[ENEMY_BOSS_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy_boss(int nType)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		enemy_boss->bUse = false;
		enemy_boss->posEnemy = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		enemy_boss->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss->sclEnemy = D3DXVECTOR3(ENEMY_BOSS_SCALE, ENEMY_BOSS_SCALE, ENEMY_BOSS_SCALE);
	}

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureEnemy_boss = NULL;
		g_pD3DXMeshEnemy_boss = NULL;
		g_pD3DXBuffMatEnemy_boss = NULL;
		g_nNumMatEnemy_boss = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(ENEMY_BOSS_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatEnemy_boss,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatEnemy_boss,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshEnemy_boss)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// マテリアル情報を取り出す
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_boss->GetBufferPointer();
		g_pD3DMatMeshEnemy_boss = new D3DMATERIAL9[g_nNumMatEnemy_boss];		// メッシュ情報を確保
		g_pD3DTextureEnemy_boss = new LPDIRECT3DTEXTURE9[g_nNumMatEnemy_boss];// テクスチャを確保
		for (int i = 0; i < g_nNumMatEnemy_boss; i++)
		{
			g_pD3DMatMeshEnemy_boss[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
			g_pD3DMatMeshEnemy_boss[i].Ambient = g_pD3DMatMeshEnemy_boss[i].Diffuse;// 環境光初期化
			g_pD3DTextureEnemy_boss[i] = NULL;	// テクスチャ初期化

			// 使用しているテクスチャがあれば読み込む
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// テクスチャ読み込み
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureEnemy_boss[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy_boss(void)
{
	for (int i = 0; i < g_nNumMatEnemy_boss; i++)
	{	// テクスチャの開放
		if (g_pD3DTextureEnemy_boss[i] != NULL)
		{
			g_pD3DTextureEnemy_boss[i]->Release();
			g_pD3DTextureEnemy_boss[i] = NULL;
		}
	}

	if(g_pD3DXMeshEnemy_boss != NULL)
	{// メッシュの開放
		g_pD3DXMeshEnemy_boss->Release();
		g_pD3DXMeshEnemy_boss = NULL;
	}

	if(g_pD3DXBuffMatEnemy_boss != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatEnemy_boss->Release();
		g_pD3DXBuffMatEnemy_boss = NULL;
	}

	delete[]g_pD3DMatMeshEnemy_boss;
	delete[]g_pD3DTextureEnemy_boss;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy_boss(void)
{
	//ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	//for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	//{
	//	if (enemy_boss->bUse)
	//	{

	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy_boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		if (enemy_boss->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEnemy_boss);

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, enemy_boss->sclEnemy.x, enemy_boss->sclEnemy.y, enemy_boss->sclEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxScl);

			// 【R】回1転を反映(YawPitchRollはy,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_boss->rotEnemy.y, enemy_boss->rotEnemy.x, enemy_boss->rotEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, enemy_boss->posEnemy.x, enemy_boss->posEnemy.y, enemy_boss->posEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_boss, &g_mtxWorldEnemy_boss, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy_boss);

			/******************** ビューポート変換 ********************/
			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);
			//// マテリアル情報に対するポインタの取得
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_boss->GetBufferPointer();

			for (int j = 0; j < g_nNumMatEnemy_boss; j++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&g_pD3DMatMeshEnemy_boss[j]);
				// テクスチャの設定（NULL:テクスチャ無し）
				pDevice->SetTexture(0, g_pD3DTextureEnemy_boss[j]);
				// 描画
				g_pD3DXMeshEnemy_boss->DrawSubset(j);
			}
			// マテリアルを元に戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 出現処理
//=============================================================================
int SetEnemy_boss(void)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[0];

	for (int i = 0; i < ENEMY_BOSS_MAX; i++, enemy_boss++)
	{
		if (!enemy_boss->bUse)
		{
			enemy_boss->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void SetEnemy_bossPos(int nIndex, D3DXVECTOR3 pos, float rot)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[nIndex];
	enemy_boss->posEnemy = pos;
	enemy_boss->rotEnemy.y = rot;
}

//=============================================================================
// 解放処理
//=============================================================================
void ReleaseEnemy_boss(int nIndex)
{
	ENEMY_BOSS *enemy_boss = &enemy_bossWk[nIndex];
	enemy_boss->bUse = false;
}

//=============================================================================
// 取得関数
//=============================================================================
ENEMY_BOSS *GetEnemy_boss(int no)
{
	return(&enemy_bossWk[no]);
}
