#ifndef DIRECT3D11_H
#define DIRECT3D11_H

#include "Common.h"
#include <d3d11.h>
#include "Exception.h"

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")//HLSL���R���p�C������

#define UD3D11_FORMAT		DXGI_FORMAT_R8G8B8A8_UNORM

//�k�x�X�e���V���ƃV�F�[�_�[���\�[�X�r���[�̂ǂ���ł��g�p�ł���悤�Ɍ^���w�肵�Ȃ�
#define UD3D11_DEPTH_FORMAT DXGI_FORMAT_R32_TYPELESS

namespace maps
{
	//�����_�[�^�[�Q�b�g�r���[���쐬����
	void CreateRenderTargetView(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView,
		UINT ArraySize = 1);
	//�{�����[���e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[���쐬����
	void CreateRenderTargetViewOfVolumeTexture(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height, UINT Depth,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView);

	//�����_�[�^�[�Q�b�g�r���[����V�F�[�_�[���\�[�X�r���[���쐬����
	void CreateSRViewFromRTView(ID3D11Device* device, ID3D11RenderTargetView* pRTView, ID3D11ShaderResourceView** ppSRView);

	//�A���I�[�_�h�A�N�Z�X�r���[���쐬����
	void CreateRenderTargetViewOfRWTexture(ID3D11Device* device, DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11UnorderedAccessView** ppUAView, ID3D11ShaderResourceView** ppSRView, UINT ArraySize = 1);

	//�A���I�[�_�[�h�A�N�Z�X�r���[����V�F�[�_�[���\�[�X���쐬����
	void CreateSRViewFromUAView(ID3D11Device* device, ID3D11UnorderedAccessView* pUAView, ID3D11ShaderResourceView** ppSRView);

	//�[�x�X�e���V���r���[���쐬����
	void CreateDepthStencilView(ID3D11Device* device, UINT Width, UINT Height,
		ID3D11DepthStencilView** ppDSView, ID3D11ShaderResourceView** ppSRView, UINT ArraySize = 1);

	//�[�x�X�e���V���r���[����V�F�[�_���\�[�X�r���[���쐬����
	void CreateSRViewFromDSView(ID3D11Device* device, ID3D11DepthStencilView* pDSView, ID3D11ShaderResourceView** ppSRView);

	//�m�C�Y�}�b�v���쐬����
	void CreateSRViewOfNoiseMap(ID3D11Device* device, UINT Width, UINT Height, DXGI_FORMAT format, ID3D11ShaderResourceView** ppSRView);
};

namespace buffers
{
	//�o�b�t�@���쐬���鋤�ʂ̊֐�
	ID3D11Buffer* CreateBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);

	//���_�o�b�t�@���쐬����
	inline ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag)
	{
		return CreateBuffer(device, pData, size, CPUAccessFlag, D3D11_BIND_VERTEX_BUFFER);
	}

	//�C���f�b�N�X�o�b�t�@���쐬����
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
	//���������
	void Invalidate();
	//�f�B�X�v���C���[�h�ꗗ���쐬
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