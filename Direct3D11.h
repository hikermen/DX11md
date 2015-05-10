#ifndef DIRECT3D11_H
#define DIRECT3D11_H

#include "Common.h"
#include <d3d11.h>
#include "Exception.h"

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")//HLSLをコンパイルする

#define UD3D11_FORMAT		DXGI_FORMAT_R8G8B8A8_UNORM

//震度ステンシルとシェーダーリソースビューのどちらでも使用できるように型を指定しない
#define UD3D11_DEPTH_FORMAT DXGI_FORMAT_R32_TYPELESS

namespace maps
{
	//レンダーターゲットビューを作成する
	void CreateRenderTargetView(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView,
		UINT ArraySize = 1);
	//ボリュームテクスチャ用のレンダーターゲットビューを作成する
	void CreateRenderTargetViewOfVolumeTexture(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height, UINT Depth,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView);

	//レンダーターゲットビューからシェーダーリソースビューを作成する
	void CreateSRViewFromRTView(ID3D11Device* device, ID3D11RenderTargetView* pRTView, ID3D11ShaderResourceView** ppSRView);

	//アンオーダドアクセスビューを作成する
	void CreateRenderTargetViewOfRWTexture(ID3D11Device* device, DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11UnorderedAccessView** ppUAView, ID3D11ShaderResourceView** ppSRView, UINT ArraySize = 1);

	//アンオーダードアクセスビューからシェーダーリソースを作成する
	void CreateSRViewFromUAView(ID3D11Device* device, ID3D11UnorderedAccessView* pUAView, ID3D11ShaderResourceView** ppSRView);

	//深度ステンシルビューを作成する
	void CreateDepthStencilView(ID3D11Device* device, UINT Width, UINT Height,
		ID3D11DepthStencilView** ppDSView, ID3D11ShaderResourceView** ppSRView, UINT ArraySize = 1);

	//深度ステンシルビューからシェーダリソースビューを作成する
	void CreateSRViewFromDSView(ID3D11Device* device, ID3D11DepthStencilView* pDSView, ID3D11ShaderResourceView** ppSRView);

	//ノイズマップを作成する
	void CreateSRViewOfNoiseMap(ID3D11Device* device, UINT Width, UINT Height, DXGI_FORMAT format, ID3D11ShaderResourceView** ppSRView);
};

namespace buffers
{
	//バッファを作成する共通の関数
	ID3D11Buffer* CreateBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);

	//頂点バッファを作成する
	inline ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag)
	{
		return CreateBuffer(device, pData, size, CPUAccessFlag, D3D11_BIND_VERTEX_BUFFER);
	}

	//インデックスバッファを作成する
	inline ID3D11Buffer* CreateIndexBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag)
	{
		return CreateBuffer(device, pData, size, CPUAccessFlag, D3D11_BIND_INDEX_BUFFER);
	}
};

class UCDirect3D11
{
public:
	HWND					m_hWnd;
	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pD3DDeviceContext;

private:
	IDXGIAdapter*			m_pAdapter;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	BOOL m_UseDepthBuffer, m_UseMultiSample;

	void CreateAdapter();

	void InitWindow(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode);

	void CreateDevice();
	void CreateSwapChain(DXGI_MODE_DESC* pDisplayMode);
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateViewport();

	void SetWindowAssociation();
	void SetDefaultRasterize();
	void SetDefaultDepthStencilState();

public:
	UCDirect3D11();
	virtual ~UCDirect3D11();
	//メモリ解放
	void Invalidate();
	//ディスプレイモード一覧を作成
	void GetDisplayMode(DXGI_MODE_DESC* pModeDesc, UINT* pNum);

	void CreateDirect3D11(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode,
		BOOL UseMultisample, BOOL UseDepthBuffer);

	inline void ClearBackBuffer(const FLOAT ColorRGBA[4])
	{
		m_pD3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ColorRGBA);
	}
	inline void ClearDepthStencilView(UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
	{
		if (m_pDepthStencilView) m_pD3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, ClearFlags, Depth, Stencil);
	}

	HRESULT Present(UINT SyncInterval, UINT Flags);

};

#endif