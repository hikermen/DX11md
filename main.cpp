#include "Common.h"
#include "Exception.h"
#include "Direct3D11.h"
#include "ShadowMap.h"
#include "Camera.h"


#include "PlaneMesh.h"
#include "CubeMesh.h"

//アプリケーション名
TCHAR* AppName = _T("Direct3D11");
//Direct3D関連の自作クラス
UCDirect3D11* g_pDirect3D11 = nullptr;

ShadowMap* g_pShadowMap = nullptr;

PlaneMesh* g_pPlaneMesh = nullptr;
CubeMesh* g_pCubeMesh = nullptr;


DWORD g_Width = 640, g_Height = 480;

//Camera* g_pCamera = nullptr;

DebugCamera* g_pCamera = nullptr;


void Invalidate()
{
	SAFE_DELETE(g_pCamera);
	SAFE_DELETE(g_pDirect3D11);
	SAFE_DELETE(g_pShadowMap);
	SAFE_DELETE(g_pPlaneMesh);
	SAFE_DELETE(g_pCubeMesh);
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	switch (msg)
	{
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0L;
}

//Direct3Dの作成
void CreateDirect3D(HINSTANCE hInstance)
{
	DXGI_MODE_DESC* pModeDescArray = nullptr;

	__try
	{
		DXGI_MODE_DESC ModeDesc;
		UINT num;

		g_pDirect3D11->GetDisplayMode(nullptr, &num);

		pModeDescArray = NEW DXGI_MODE_DESC[num];

		g_pDirect3D11->GetDisplayMode(pModeDescArray, &num);

		bool find = false;
		for (UINT i = 0; i < num; i++)
		{
			if (pModeDescArray[i].Width == g_Width && pModeDescArray[i].Height == g_Height)
			{
				find = true;
				::CopyMemory(&ModeDesc, &pModeDescArray[i], sizeof(DXGI_MODE_DESC));
				break;
			}
		}

		if (find == false)
			throw(Exception(-1, _T("InitDirect3D()でエラーが発生しました。適切なディスプレイモードの解像度を取得できません。")));

		g_pDirect3D11->CreateDirect3D11(AppName, hInstance, WndProc, &ModeDesc, TRUE, TRUE);

	}
	__finally
	{
		SAFE_DELETE_ARRAY(pModeDescArray);
	}
}

void CreateGraphicsPipeline()
{
	g_pShadowMap->Create(g_pDirect3D11->m_pD3DDevice, 1024, 1024);

}

void CreateMesh()
{
	g_pPlaneMesh = NEW PlaneMesh();
	g_pPlaneMesh->CreateMesh(g_pDirect3D11->m_pD3DDevice);

	g_pCubeMesh = NEW CubeMesh();
	g_pCubeMesh->CreateMesh(g_pDirect3D11->m_pD3DDevice);
}

void CreateResource(HINSTANCE hInstance)
{
	//Direct3D 関連自作クラスのインスタンスを作成
	//CreateDirect3D()関数内でインスタンスを作成すると C2712 エラーが作成する
	g_pDirect3D11 = NEW UCDirect3D11();

	//g_pCamera = NEW Camera(g_Width, g_Height);

	g_pCamera = NEW DebugCamera(g_Width, g_Height);

	g_pShadowMap = NEW ShadowMap();

	CreateDirect3D(hInstance);

	CreateGraphicsPipeline();

	CreateMesh();
}

void Update()
{
	g_pCamera->Update();
	g_pPlaneMesh->Update();
	g_pCubeMesh->Update();
}

HRESULT Render()
{
	HRESULT hr = E_FAIL;

	static float ClearColor[4] = { 0.3f, 0.3f, 1.0f, 1.0f };

	g_pDirect3D11->ClearBackBuffer(ClearColor);

	g_pDirect3D11->ClearDepthStencilView(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (UINT i = 0; i < g_pShadowMap->GetMaxPass(); i++)
	{
		g_pShadowMap->BeginPass(g_pDirect3D11->m_pD3DDeviceContext, g_pCamera->GetView(), g_pCamera->GetProjection(), g_pCamera->GetOrtho(), g_pCamera->GetVecLightPos(), i);

		g_pShadowMap->SetConstantBuffers(g_pDirect3D11->m_pD3DDeviceContext, g_pPlaneMesh->GetMatWorld(), i);

		g_pPlaneMesh->Render(g_pDirect3D11->m_pD3DDeviceContext);
		g_pCubeMesh->Render(g_pDirect3D11->m_pD3DDeviceContext);

		g_pShadowMap->EndPass(g_pDirect3D11->m_pD3DDeviceContext, i);
	}

	// レンダリングされたイメージをユーザーに表示。
	if (FAILED(hr = g_pDirect3D11->Present(0, 0)))
		throw(Exception(hr, _T("IDXGISwapChain::Present()が失敗しました")));

	return hr;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE,
	LPTSTR,
	INT)
{
	//メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);

	MSG msg = { 0 };

	//メモリ確保位置の確認
	//_CrtSetBreakAlloc();

	::CoInitialize(NULL);

	try
	{
		CreateResource(hInstance);

		::ShowWindow(g_pDirect3D11->m_hWnd, SW_SHOW);
		::UpdateWindow(g_pDirect3D11->m_hWnd);

		do
		{
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				Update();
				Render();
			}
		} while (msg.message != WM_QUIT);
	}
	catch (Exception ex)
	{
		::MessageBox(nullptr, ex.m_pErrorStr, _T("エラー"), MB_OK);
	}


	::UnregisterClass(AppName, hInstance);

	Invalidate();

	::CoUninitialize();

	return msg.wParam;

}