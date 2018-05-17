//=============================================================================
//
// メイン処理 [main.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"

/* Camera */
#include "camera.h"

/* 2D */
#include "magic.h"
#include "magiceffect.h"
#include "minimap.h"
#include "reticle.h"
#include "pointer.h"
#include "gage.h"
#include "gagefream.h"
#include "gageback.h"
#include "gageselect.h"
#include "time.h"
#include "timefream.h"
#include "timeranking.h"
#include "ranking.h"
#include "rank.h"
#include "joyconhelp.h"
#include "damageeffect.h"

/* 3D */
#include "model.h"			// モデル
#include "enemy.h"			// エネミー（システムのみ）
#include "enemy_normal.h"	// 通常エネミー
#include "enemy_boss.h"		// ボスエネミー
#include "magiccircle.h"
#include "player.h"

/* Billnpard */
#include "bullet.h"
#include "bulletqua.h"
#include "enemybullet.h"
#include "lockon.h"
#include "cloud.h"
#include "hiteffect.h"
#include "effect.h"
#include "parameter.h"

/* Field */
#include "skydome.h"
#include "skydomeeffect.h"
#include "meshcloud.h"
#include "field.h"
#include "wall.h"
#include "cloudfield.h"

/* Shadow */
#include "shadow.h"

/* Stage */
#include "title.h"
#include "titlemenu.h"
#include "version.h"
#include "copyright.h"
#include "pause.h"
#include "pausemenu.h"
#include "result.h"
#include "game.h"
#include "fade.h"

/* System */
#include "stage.h"
#include "light.h"
#include "input.h"
#include "checkhit.h"
#include "sound.h"
#include "resource.h"

/* Debug */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void ChackHit(void);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// デバイスオブジェクト(描画に必要)

E_STAGE				g_eStage = START_STAGE;	// ステージ
int					g_nTotalCount = 0;		// トータルカウンタ
int					g_nGameCount = 0;		// トータルカウンタ
int					g_nCountFPS = 0;		// FPSカウンタ

bool				g_bEndFlag = true;		// 終了フラグ

//float g_fFogDensity = 0.8f;
//D3DXCOLOR g_xFogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


#ifdef _DEBUG
bool				g_bDispDebug = true;	// デバッグ表示ON/OFF
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),									// 構造体のサイズ
		CS_CLASSDC,											// クラスのスタイル
		WndProc,											// ウィンドプロシージャ
		0,													// 補助メモリ
		0,													// 補助メモリ
		hInstance,											// インスタンスハンドル
		LoadIcon(hInstance , MAKEINTRESOURCE(IDI_ICON1)),	// アイコン画像
		LoadCursor(NULL, IDC_ARROW),						// カーソル画像
		(HBRUSH)(COLOR_WINDOW + 1),							// 背景色
		NULL,												// メニュー名
		CLASS_NAME,											// クラス名
		LoadIcon(hInstance , MAKEINTRESOURCE(IDI_ICON2))	// アイコン16x16
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	bool flag = false; // フルスクリーンモードで初期化
	if (MessageBox(hWnd, "ウィンドウモードで起動しますか？", "起動確認", MB_YESNO) == IDYES)
	{
		flag = true;
	}

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, flag)))
	{
		return -1;
	}

	//ランダムの初期化
	srand((unsigned)time(NULL));

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// メッセージループ
	while(g_bEndFlag)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{	// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				PrintDebugProc("FPS:%d  TotalCount:%u", g_nCountFPS, g_nTotalCount);
#endif

				dwExecLastTime = dwCurrentTime;

				// ウィンドウがアクティブの場合
				if (hWnd == GetActiveWindow())
				{
					// ウィンドウの座標を取得
					RECT	lpScreen;
					GetWindowRect(hWnd, &lpScreen);

					// マウスカーソルの移動範囲を指定
					lpScreen.left = lpScreen.left + (long)MOUSE_MARGIN;
					lpScreen.top = lpScreen.bottom - (long)SCREEN_HEIGHT + (long)MOUSE_MARGIN;
					lpScreen.right = lpScreen.right - (long)MOUSE_MARGIN;
					lpScreen.bottom = lpScreen.bottom - (long)MOUSE_MARGIN;

					// マウスカーソルの移動制限
					ClipCursor(&lpScreen);

#ifdef _DEBUG
					PrintDebugProc("Screen L[%l]  T[%l]  R[%l]  B[%l]\n",	// ウィンドウ座標を表示
						lpScreen.left, lpScreen.top, lpScreen.right, lpScreen.bottom);
#endif
				}

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
				g_nTotalCount++;
			}
		}
	}
	
	DestroyWindow(hWnd);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYDOWN:
	//	switch(wParam)
	//	{
	//	case VK_ESCAPE:
			//DestroyWindow(hWnd);
	//		break;
	//	}
	//	break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;			// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;			// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// 入力の初期化処理
	InitInput(hInstance, hWnd);

	// サウンドの初期化処理
	InitSound(hWnd);


#ifdef _DEBUG
	// デバッグ表示処理の初期化
	InitDebugProc();
#endif

	InitFade();			// フェード

	// 雲の初期化
	InitMeshcloud(D3DXVECTOR3(MESHCLOUD_POS_X, 0.0f, MESHCLOUD_POS_Z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), MESHCLOUD_X, MESHCLOUD_Z, MESHCLOUD_SIZE_X, MESHCLOUD_SIZE_Z, 0);

	// 各種初期化処理（ステージ）
	InitSystem(STAGE_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスオブジェクトの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力の終了処理
	UninitInput();

#ifdef _DEBUG
	// デバッグ表示処理の終了処理
	UninitDebugProc();
#endif

	UninitCamera();			// カメラ
	UninitSkydome();		// スカイドーム
	UninitSkydomeeffect();	// スカイドームエフェクト
	UninitEnemy();			// エネミー
	UninitEnemy_normal();	// エネミーノーマル
	UninitEnemy_boss();		// エネミーボス
	//UninitField();		// 地面
	//UninitWall();			// 壁
	UninitMeshcloud();		// 雲海
	UninitCloud();			// 雲
	UninitCloudfield();		// 雲フィールド
	UninitEnemybullet();	// エネミーバレット
	UninitBullet();			// バレット
	UninitBulletQua();		// バレットクォータニオン（エネミー）
	UninitHiteffect();		// ヒットエフェクト
	UninitModel();			// モデル
	UninitPlayer();			// プレイヤー
	UninitShadow();			// 影
	UninitEffect();			// エフェクト
	UninitReticle();		// レティクル
	UninitMagic();			// 魔法陣
	UninitMagiccircle();	// 魔法サークル
	UninitPointer();		// ポイント
	UninitMinimap();		// ミニマップ
	UninitLockon();			// ロックオン
	UninitTitle();			// タイトル
	UninitTitlemenu();		// タイトルメニュー
	UninitVersion();		// バージョン
	UninitCopyright();		// コピーライト
	UninitResult();			// リザルト
	UninitParameter();		// エネミーHPゲージ
	UninitDamageeffect();	// ダメージエフェクト
	UninitGage();			// ゲージ
	UninitGagefream();		// ゲージフレーム
	UninitGageback();		// ゲージバック
	UninitGageselect();		// ゲージセレクト
	UninitMagiceffect();	// マジックエフェクト
	UninitRanking();		// ランキング
	UninitRank();			// ランク
	UninitJoyconhelp();		// Joyconヘルプ
	UninitTimefream();		// タイムフレーム
	UninitTime();			// タイム
	UninitTimeranking();	// タイムランキング
	UninitPause();			// ポーズ
	UninitPausemenu();		// ポーズメニュー
	UninitFade();			// フェード
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	PAUSE *pause = GetPause(0);

	// 入力の更新処理
	UpdateInput();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}
	UpdateDebugProc();
#endif

	E_STAGE eStageTemp = g_eStage;
	switch (g_eStage)
	{
	case STAGE_TITLE:					// タイトル
		UpdateMeshcloud();		// 雲海
		UpdateShadow();			// 影
		UpdateModel();			// モデル
		UpdatePlayer();			// プレイヤー
		UpdateSkydome();		// スカイドーム
		UpdateSkydomeeffect();	// スカイドームエフェクト
		UpdateTitle();			// タイトル
		UpdateTitlemenu();		// タイトルメニュー
		UpdateVersion();
		UpdateCopyright();
		break;
	case STAGE_TUTORIAL:				// チュートリアル
		break;
	case STAGE_CREDIT:					// クレジット
		break;
	case STAGE_GAME:					// ゲーム
		UpdatePause();		// ポーズ
		if (!pause->bUse)	// ポーズ有効中は括弧以下をアップデートしない
		{
			//UpdateField();		// 地面
			//UpdateWall();			// 壁
			UpdateShadow();			// 影
			UpdateGame();
			UpdateMeshcloud();		// 雲海
			UpdateCloud();			// 雲
			//UpdateCloudfield();		// 雲フィールド
			UpdateEnemy();			// エネミー
			UpdateEnemy_normal();	// エネミーノーマル
			UpdateEnemy_boss();		// エネミーボス
			UpdateEnemybullet();	// バレット
			UpdateBulletQua();		// バレットクォータニオン（エネミー）
			UpdateHiteffect();		// ヒットエフェクト
			UpdateEffect();			// エフェクト
			UpdateModel();			// モデル
			UpdatePlayer();			// プレイヤー
			UpdateBullet();			// バレット
			UpdateLockon();			// ロックオン
			UpdateParameter();
			UpdateSkydome();		// スカイドーム
			UpdateSkydomeeffect();	// スカイドームエフェクト
			UpdateReticle();		// レティクル
			UpdateMagic();			// 魔法陣
			UpdateMagiccircle();	// 魔法サークル
			UpdateMinimap();		// ミニマップ
			UpdateGageback();		// ゲージバック
			UpdateGage();			// ゲージ
			UpdateGagefream();		// ゲージフレーム
			UpdateGageselect();		// ゲージセレクト
			UpdateMagiceffect();	// マジックエフェクト
			UpdateTimefream();		// タイムフレーム
			UpdateTime();			// タイム
			UpdatePointer();		// ポイント
			UpdateDamageeffect();	// ダメージエフェクト
			UpdateJoyconhelp();		// Joyconヘルプ
			ChackHit();				// 当たり判定
		}
		UpdatePausemenu();	// ポーズメニュー
		break;
	case STAGE_RESULT:					// リザルト
		UpdateMeshcloud();		// 雲海
		UpdateShadow();			// 影
		UpdateModel();			// モデル
		UpdateSkydome();		// スカイドーム
		UpdateSkydomeeffect();	// スカイドームエフェクト
		UpdateRank();			// ランク
		UpdateRanking();		// ランキング
		UpdateReticle();		// レティクル
		UpdateTimefream();		// タイムフレーム
		UpdateTimeranking();	// タイムランキング
		UpdateTime();			// タイム
		UpdateResult();			// リザルト
		break;
	}

	UpdateCamera();		// カメラ
	UpdateSound();
	UpdateFade();

	if (eStageTemp != g_eStage)
	{

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// カメラの設定処理
		SetCamera();

		switch (g_eStage)
		{
		case STAGE_TITLE:					// タイトル
			DrawSkydome();			// スカイドーム
			DrawSkydomeeffect();	// スカイドームエフェクト
			DrawMeshcloud();		// 雲海
			DrawShadow();			// 影
			//DrawModel();			// モデル
			DrawPlayer();			// プレイヤー
			DrawTitle();			// タイトル
			DrawTitlemenu();		// タイトルメニュー
			DrawVersion();
			DrawCopyright();
			DrawFade();
			break;
		case STAGE_TUTORIAL:				// チュートリアル
			break;
		case STAGE_CREDIT:					// クレジット
			break;
		case STAGE_GAME:					// ゲーム
			//DrawField();			// 地面
			//DrawWall();			// 壁

			//// フォグ設定
			//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);					// フォグブレンディング有効
			//g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, g_xFogColor);				// フォグカラー設定（白）
			//g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP2);
			//g_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, g_fFogDensity);
			DrawSkydome();			// スカイドーム
			DrawSkydomeeffect();	// スカイドームエフェクト
			DrawMeshcloud();		// 雲海
			//DrawCloudfield();		// 雲フィールド
			//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);					// フォグブレンディング有効
			DrawShadow();			// 影
			DrawEnemy();			// エネミー
			DrawEnemy_normal();		// エネミーノーマル
			DrawEnemy_boss();		// エネミーボス
			DrawEnemybullet();		// エネミーバレット
			DrawBulletQua();		// バレットクォータニオン（エネミー）
			DrawBullet();			// バレット
			DrawHiteffect();		// ヒットエフェクト
			//DrawModel();			// モデル
			DrawPlayer();			// プレイヤー
			DrawEffect();			// エフェクト
			DrawCloud();			// 雲
			DrawParameter();		// ビルボードHPバー
			DrawLockon();			// ロックオン
			DrawMagiccircle();		// 魔法サークル
			DrawReticle();			// レティクル
			DrawMagic();			// 魔法陣
			DrawMinimap();			// ミニマップ
			DrawGageback();			// ゲージバック
			DrawGage();				// ゲージ
			DrawGagefream();		// ゲージフレーム
			DrawGageselect();		// ゲージセレクト
			DrawMagiceffect();		// マジックエフェクト
			DrawPointer();			// ポイント
			DrawTimefream();		// タイムフレーム
			DrawTime();				// タイム
			DrawJoyconhelp();		// Joyconヘルプ
			DrawDamageeffect();		// ダメージエフェクト
			DrawFade();
			DrawPause();			// ポーズ
			DrawPausemenu();		// ポーズメニュー
			break;
		case STAGE_RESULT:					// リザルト
			DrawSkydome();			// スカイドーム
			DrawSkydomeeffect();	// スカイドームエフェクト
			DrawMeshcloud();		// 雲海
			DrawShadow();			// 影
			DrawModel();			// モデル
			DrawResult();			// リザルト
			DrawRanking();			// ランキング
			DrawReticle();			// レティクル
			DrawRank();				// ランク
			DrawTimefream();		// タイムフレーム
			DrawTime();				// タイム
			DrawTimeranking();		// タイムランキング
			DrawFade();
			break;
		}



#ifdef _DEBUG
		// デバッグ表示処理の描画
		if (g_bDispDebug)
		{
			DrawDebugProc();
		}
#endif

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


//=============================================================================
// トータルカウント取得関数
//=============================================================================
int *GetTotalCount(void)
{
	return(&g_nTotalCount);
}

//=============================================================================
// 初期化
//=====================asas========================================================
void InitSystem(int nType)
{
	//if (nType == 0)
	//{
	//	InitShadow(nType);		// 影
	//}
	switch (g_eStage)
	{
	case STAGE_TITLE:
		InitStage();
		break;
	}


	InitShadow(nType);			// 影
	InitGame();					// ゲーム管理
	InitMeshcloudPos();			// 雲海座標
	InitCloudfield(nType);		// 雲フィールド
	InitTimefream(nType);		// タイムフレーム
	InitTime(nType);			// タイム
	InitVersion(nType);			// バージョン
	InitCopyright(nType);		// コピーライト
	InitEnemy(nType);			// エネミー
	InitEnemy_normal(nType);	// エネミーノーマル
	InitEnemy_boss(nType);		// エネミーボス
	InitPlayer(nType);			// プレイヤー
	InitModel(nType);			// モデル
	InitTitle(nType);			// タイトル
	InitTitlemenu(nType);		// タイトルメニュー
	InitPause(nType);			// ポーズ
	InitPausemenu(nType);		// ポーズメニュー
	InitResult(nType);			// リザルト
	InitSkydome(nType);			// スカイドーム
	InitSkydomeeffect(nType);	// スカイドームエフェクト
	InitCloud(nType);			// 雲
	//InitField(nType);			// 地面
	InitWall(nType);			// 壁
	InitEnemybullet(nType);		// バレットエネミー
	InitBullet(nType);			// バレット
	InitBulletQua(nType);		// バレットクォータニオン（エネミー）
	InitHiteffect(nType);		// ヒットエフェクト
	InitLockon(nType);			// ロックオン
	InitEffect(nType);			// エフェクト
	InitParameter(nType);		// パラメーター
	InitReticle(nType);			// レティクル
	InitMagic(nType);			// マジック（スキル）
	InitMagiccircle(nType);		// マジックサークル
	InitMagiceffect(nType);		// マジックエフェクト
	InitDamageeffect(nType);	// ダメージエフェクト
	InitMinimap(nType);			// ミニマップ
	InitJoyconhelp(nType);		// Joyconヘルプ
	InitGage(nType);			// ゲージ
	InitGagefream(nType);		// ゲージフレーム
	InitGageback(nType);		// ゲージバック
	InitGageselect(nType);		// ゲージセレクト
	InitPointer(nType);			// ポイント
	InitRanking(nType);			// ランキング
	InitTimeranking(nType);		// タイムランキング
	InitRank(nType);			// ランク
	InitLight();				// ライト
	InitCamera();				// カメラ

	//if (nType == 1)
	//{
	//	InitShadow(nType);		// 影
	//}
}

//=============================================================================
// ステージ遷移処理
//=============================================================================
void SetStage(E_STAGE eStage)
{
	g_eStage = eStage;
}

//=============================================================================
// ステージ取得処理
//=============================================================================
E_STAGE GetStage(void)
{
	return g_eStage;
}

//=============================================================================
// 終了フラグ
//=============================================================================
void SetEndFlag(void)
{
	g_bEndFlag = false;
}
