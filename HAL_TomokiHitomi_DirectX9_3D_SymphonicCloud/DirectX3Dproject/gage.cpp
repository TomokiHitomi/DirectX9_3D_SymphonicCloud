//=============================================================================
//
// パラメータ処理 [gage.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "gage.h"
#include "sound.h"
#include "model.h"
#include "calculate.h"
#include "gagefream.h"
#include "gageback.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGage(int no, D3DXCOLOR xColor);
void SetVertexGage(int no);
void SetPointGage(int nGage, float fGageSizeDef, float fStatusDef, float fStatus);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAGE					gageWk[GAGE_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureGage = NULL;
LPDIRECT3DTEXTURE9		pD3DTextureModelmp = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGE00,				// ファイルの名前
			&pD3DTextureGage);			// 読み込むメモリのポインタ
	}

	// パラメータの初期化
	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{

		InitStatusGage(i);
	}

	return S_OK;
}

//=============================================================================
// ステータスの初期化処理
//=============================================================================
void InitStatusGage(int nGage)
{
	GAGE *gage = &gageWk[nGage];

	gage->Texture = pD3DTextureGage;			// テクスチャへのエネミー

	//// パラメータステータスの初期化
	//if (nGage != GAGE_BOSS_HP)
	//{
	//	gage->bUse = true;
	//}
	//else
	//{
	//	gage->bUse = false;
	//}

	gage->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gage->fPointGauge = 0;
	gage->bStandby = false;
	gage->nTarget = 0;

	// 頂点情報の作成
	switch (nGage)
	{
	case GAGE_PLAYER_HP:
		gage->pos = D3DXVECTOR3(HP_GAGE_POS_X, HP_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(HP_GAGE_SIZE_X, HP_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_GREEN));
		gage->bUse = true;
		break;
	case GAGE_PLAYER_MP:
		gage->pos = D3DXVECTOR3(MP_GAGE_POS_X, MP_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(MP_GAGE_SIZE_X, MP_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BLUE));
		gage->bUse = true;
		break;
	case GAGE_BULLET_NORMAL:
		gage->pos = D3DXVECTOR3(BULLET_GAGE_POS_X, BULLET_GAGE_POS_Y , 0.0f);
		gage->vecSize = D3DXVECTOR2(BULLET_GAGE_SIZE_X, BULLET_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_MAGENTA));
		gage->bUse = true;
		break;
	case GAGE_BULLET_SPECIAL_FIRE:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_ORANGE));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_THUNDER:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_YELLOW));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_ICE:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_CYAN));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_WIND:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_LIGHTGREEN));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_WATER:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BLUE));
		gage->bUse = false;
		break;
	case GAGE_BULLET_SPECIAL_EARTH:
		gage->pos = D3DXVECTOR3(MAGIC_GAGE_POS_X, MAGIC_GAGE_POS_Y + (MAGIC_GAGE_INTERVAL * (nGage - 3)), 0.0f);
		gage->vecSize = D3DXVECTOR2(MAGIC_GAGE_SIZE_X, MAGIC_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_BROWN));
		gage->bUse = false;
		break;
	case GAGE_BOSS_HP:
		gage->pos = D3DXVECTOR3(BOSS_GAGE_POS_X, BOSS_GAGE_POS_Y, 0.0f);
		gage->vecSize = D3DXVECTOR2(BOSS_GAGE_SIZE_X, BOSS_GAGE_SIZE_Y);
		MakeVertexGage(nGage, SetColorPallet(COLOR_PALLET_RED));
		gage->bUse = false;
		break;
	}




	return;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGage(void)
{
	GAGE *gage = &gageWk[0];

	// メモリ解放
	if (pD3DTextureGage != NULL)
	{
		pD3DTextureGage->Release();
		pD3DTextureGage = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGage(void)
{
	GAGE *gage = &gageWk[0];
	MODEL *model = GetModel(0);
	ENEMY *enemy;

	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{
		if (gage->bUse)
		{
			switch (i)
			{
			case GAGE_PLAYER_HP:
				SetPointGage(GAGE_PLAYER_HP, HP_GAGE_SIZE_X, MODEL_STATUS_HP, model->fStatusHP);
				break;
			case GAGE_PLAYER_MP:
				SetPointGage(GAGE_PLAYER_MP, MP_GAGE_SIZE_X, MODEL_STATUS_MP, model->fStatusMP);
				break;
			case GAGE_BULLET_NORMAL:
				SetPointGage(GAGE_BULLET_NORMAL, BULLET_GAGE_SIZE_X, MODEL_STATUS_NORMAL, model->fStatusNormal);

				break;
			case GAGE_BULLET_SPECIAL_FIRE:
				SetPointGage(GAGE_BULLET_SPECIAL_FIRE, MAGIC_GAGE_SIZE_X, MODEL_STATUS_FIRE, model->fStatusFire);
				break;
			case GAGE_BULLET_SPECIAL_THUNDER:
				SetPointGage(GAGE_BULLET_SPECIAL_THUNDER, MAGIC_GAGE_SIZE_X, MODEL_STATUS_THUNDER, model->fStatusThunder);
				break;
			case GAGE_BULLET_SPECIAL_ICE:
				SetPointGage(GAGE_BULLET_SPECIAL_ICE, MAGIC_GAGE_SIZE_X, MODEL_STATUS_ICE, model->fStatusIce);
				break;
			case GAGE_BULLET_SPECIAL_WIND:
				SetPointGage(GAGE_BULLET_SPECIAL_WIND, MAGIC_GAGE_SIZE_X, MODEL_STATUS_WIND, model->fStatusWind);
				break;
			case GAGE_BULLET_SPECIAL_WATER:
				SetPointGage(GAGE_BULLET_SPECIAL_WATER, MAGIC_GAGE_SIZE_X, MODEL_STATUS_WARER, model->fStatusWater);
				break;
			case GAGE_BULLET_SPECIAL_EARTH:
				SetPointGage(GAGE_BULLET_SPECIAL_EARTH, MAGIC_GAGE_SIZE_X, MODEL_STATUS_EARTH, model->fStatusEarth);
				break;
			case GAGE_BOSS_HP:
				enemy = GetEnemy(gage->nTarget);
				SetPointGage(GAGE_BOSS_HP, BOSS_GAGE_SIZE_X, ENEMY_STATUS_BOSS_HP, enemy->fStatusHP);
				break;
			default:
				break;
			}

			// 頂点を更新
			SetVertexGage(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGE_MAX; i++, gage++)
	{
		if (gage->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, gage->Texture);

			// パラメータの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGE, gage->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGage(int no, D3DXCOLOR xColor)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGE *gage = &gageWk[no];

	// 頂点座標の設定
	SetVertexGage(no);

	// rhwの設定
	gage->vertexWk[0].rhw =
	gage->vertexWk[1].rhw =
	gage->vertexWk[2].rhw =
	gage->vertexWk[3].rhw = 1.0f;

	xColor.a -= GAGE_ALPHA;

	// 反射光の設定
	gage->vertexWk[0].diffuse =
	gage->vertexWk[1].diffuse =
	gage->vertexWk[2].diffuse =
	gage->vertexWk[3].diffuse = xColor;

	// テクスチャ座標の設定
	gage->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	gage->vertexWk[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
	gage->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
	gage->vertexWk[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexGage(int nGage)
{
	GAGE *gage = &gageWk[nGage];
	gage->vertexWk[0].vtx.x = gage->pos.x;
	gage->vertexWk[0].vtx.y = gage->pos.y - gage->vecSize.y;
	gage->vertexWk[0].vtx.z = 0.0f;

	gage->vertexWk[1].vtx.x = gage->pos.x + gage->fPointGauge;
	gage->vertexWk[1].vtx.y = gage->pos.y - gage->vecSize.y;
	gage->vertexWk[1].vtx.z = 0.0f;

	gage->vertexWk[2].vtx.x = gage->pos.x;
	gage->vertexWk[2].vtx.y = gage->pos.y + gage->vecSize.y;
	gage->vertexWk[2].vtx.z = 0.0f;

	gage->vertexWk[3].vtx.x = gage->pos.x + gage->fPointGauge;
	gage->vertexWk[3].vtx.y = gage->pos.y + gage->vecSize.y;
	gage->vertexWk[3].vtx.z = 0.0f;
	if (nGage == GAGE_BULLET_NORMAL)
	{
		gage->vertexWk[0].vtx.x = gage->pos.x - gage->fPointGauge;
		gage->vertexWk[1].vtx.x = gage->pos.x;
		gage->vertexWk[2].vtx.x = gage->pos.x - gage->fPointGauge;
		gage->vertexWk[3].vtx.x = gage->pos.x;
	}
}

//=============================================================================
// セット関数
//=============================================================================
void SetPointGage(int nGage, float fGageSizeDef, float fStatusDef, float fStatus)
{
	GAGE *gage = &gageWk[nGage];
	if (!gage->bStandby)
	{
		gage->fPointGauge += GAGE_STANDBY_SPEED * fGageSizeDef;
		if (gage->fPointGauge > fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			gage->bStandby = true;
		}
	}
	else
	{
		if (gage->fPointGauge > fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge -= GAGE_CHANGE_SPEED * fGageSizeDef;
			if (gage->fPointGauge < fGageSizeDef / fStatusDef * fStatus)
			{
				gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			}
		}
		else if (gage->fPointGauge < fGageSizeDef / fStatusDef * fStatus)
		{
			gage->fPointGauge += GAGE_CHANGE_SPEED * fGageSizeDef;
			if (gage->fPointGauge >fGageSizeDef / fStatusDef * fStatus)
			{
				gage->fPointGauge = fGageSizeDef / fStatusDef * fStatus;
			}
		}
	}
}

//=============================================================================
// セット関数（通常の第二引数は0、BOSSセットの場合はnEnemy）
//=============================================================================
void SetGage(int nGage, int nBoss)
{
	GAGE *gage = &gageWk[nGage];
	gage->bUse = true;
	if (nGage == GAGE_BOSS_HP)
	{
		gage->nTarget = nBoss;
		SetGagefream(GAGEFREAM_TYPE_BOSS);
		SetGageback(GAGEBACK_TYPE_BOSS);
	}
}

//=============================================================================
// パラメータ取得関数
//=============================================================================
GAGE *GetGage(int no)
{
	return(&gageWk[no]);
}

