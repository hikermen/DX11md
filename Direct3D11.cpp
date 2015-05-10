#include "Direct3D11.h"



namespace maps
{

	void CreateRenderTargetView(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView,
		UINT ArraySize)
	{
		HRESULT hr = E_FAIL;
		ID3D11Texture2D* pTex2D = nullptr;

		__try
		{
			D3D11_TEXTURE2D_DESC Tex2DDesc;
			::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
			Tex2DDesc.ArraySize = ArraySize;
			Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
			if (ppSRView) Tex2DDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
			Tex2DDesc.Format = format;
			Tex2DDesc.Width = Width;
			Tex2DDesc.Height = Height;
			Tex2DDesc.MipLevels = 1;
			Tex2DDesc.SampleDesc.Count = 1;
			if (FAILED(hr = device->CreateTexture2D(&Tex2DDesc, nullptr, &pTex2D)))
				throw(Exception(hr, _T("CreateRenderTarget()�ŃG���[���������܂����B�e�N�X�`�����쐬�ł��܂���")));
			if (FAILED(hr = device->CreateRenderTargetView(pTex2D, nullptr, ppRTView)))
				throw(Exception(hr, _T("CreateRenderTargetView()�ŃG���[���������܂����B�����_�[�^�[�Q�b�g�r���[���쐬�ł��܂���")));
			if (ppSRView != nullptr)
				maps::CreateSRViewFromRTView(device, *ppRTView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex2D);
		}

	}

	void CreateRenderTargetViewOfVolumeTexture(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height, UINT Depth,
		ID3D11RenderTargetView** ppRTView, ID3D11ShaderResourceView** ppSRView)
	{
		HRESULT hr = E_FAIL;
		ID3D11Texture3D* pTex3D = nullptr;

		__try
		{
			D3D11_TEXTURE3D_DESC Tex3DDesc;
			::ZeroMemory(&Tex3DDesc, sizeof(D3D11_TEXTURE3D_DESC));
			Tex3DDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
			if (ppSRView) Tex3DDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			Tex3DDesc.Usage = D3D11_USAGE_DEFAULT;
			Tex3DDesc.Format = format;
			Tex3DDesc.Width = Width;
			Tex3DDesc.Height = Height;
			Tex3DDesc.Depth = Depth;
			Tex3DDesc.MipLevels = 1;
			if (FAILED(hr = device->CreateTexture3D(&Tex3DDesc, nullptr, &pTex3D)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfVolumeTexture()�ŃG���[���������܂����B�e�N�X�`�����쐬�ł��܂���")));
			if (FAILED(hr = device->CreateRenderTargetView(pTex3D, nullptr, ppRTView)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfVolumeTexture()�ŃG���[���������܂����B�����_�[�^�[�Q�b�g�r���[���쐬�ł��܂���")));
			if (ppSRView != nullptr)
				maps::CreateSRViewFromRTView(device, *ppRTView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex3D);
		}
	}

	void CreateSRViewFromRTView(ID3D11Device* device, ID3D11RenderTargetView* pRTView, ID3D11ShaderResourceView** ppSRView)
	{

		HRESULT hr = E_FAIL;
		ID3D11Resource* pResource = nullptr;

		__try
		{
			//�����_�[�^�[�Q�b�g�r���[����e�N�X�`�����擾����
			pRTView->GetResource(&pResource);

			if (FAILED(hr = device->CreateShaderResourceView(pResource, nullptr, ppSRView)))
				throw (Exception(hr, _T("CreateSRViewFromRTView()�ŃG���[���������܂����B�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���")));

		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// �A���I�[�_�[�h�A�N�Z�X�r���[���쐬����
	void CreateRenderTargetViewOfRWTexture(ID3D11Device* device,
		DXGI_FORMAT format, UINT Width, UINT Height,
		ID3D11UnorderedAccessView** ppUAView, ID3D11ShaderResourceView** ppSRView,
		UINT ArraySize)
	{
		HRESULT hr = E_FAIL;
		ID3D11Texture2D* pTex2D = nullptr;

		__try
		{
			D3D11_TEXTURE2D_DESC Tex2DDesc;
			::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
			Tex2DDesc.ArraySize = ArraySize;
			Tex2DDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			if (ppSRView) Tex2DDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
			Tex2DDesc.Format = format;
			Tex2DDesc.Width = Width;
			Tex2DDesc.Height = Height;
			Tex2DDesc.MipLevels = 1;
			Tex2DDesc.SampleDesc.Count = 1;
			if (FAILED(hr = device->CreateTexture2D(&Tex2DDesc, nullptr, &pTex2D)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfRWTexture()�ŃG���[���������܂����B�e�N�X�`���[���쐬�ł��܂���B")));

			if (FAILED(hr = device->CreateUnorderedAccessView(pTex2D, nullptr, ppUAView)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfRWTexture()�ŃG���[���������܂����B�A���I�[�_�[�h�A�N�Z�X�r���[���쐬�ł��܂���B")));

			if (ppSRView != nullptr)
				maps::CreateSRViewFromUAView(device, *ppUAView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex2D);
		}
	}

	// �A���I�[�_�[�h�A�N�Z�X�r���[����V�F�[�_�[���\�[�X�r���[���쐬����
	void CreateSRViewFromUAView(ID3D11Device* device, ID3D11UnorderedAccessView* pUAView, ID3D11ShaderResourceView** ppSRView)
	{
		HRESULT hr = E_FAIL;
		ID3D11Resource* pResource = nullptr;

		__try
		{
			// �A���I�[�_�[�h�A�N�Z�X�r���[����e�N�X�`���[���擾����
			pUAView->GetResource(&pResource);

			// �V�F�[�_�[���\�[�X�r���[���쐬����
			if (FAILED(hr = device->CreateShaderResourceView(pResource, nullptr, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewFromUAView()�ŃG���[���������܂����B�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���B")));
		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// �[�x�X�e���V���r���[���쐬����
	void CreateDepthStencilView(ID3D11Device* device,
		UINT Width, UINT Height,
		ID3D11DepthStencilView** ppDSView, ID3D11ShaderResourceView** ppSRView,
		UINT ArraySize)
	{
		HRESULT hr = E_FAIL;
		ID3D11Texture2D* pTex2D = nullptr;

		__try
		{
			D3D11_TEXTURE2D_DESC Tex2DDesc;
			::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
			Tex2DDesc.ArraySize = ArraySize;
			Tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			if (ppSRView) Tex2DDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
			Tex2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			Tex2DDesc.Width = Width;
			Tex2DDesc.Height = Height;
			Tex2DDesc.MipLevels = 1;
			Tex2DDesc.SampleDesc.Count = 1;

			// �[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
			hr = device->CreateTexture2D(&Tex2DDesc, nullptr, &pTex2D);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateDepthStencilView()�ŃG���[���������܂����B�[�x�o�b�t�@�p�̃e�N�X�`���[�쐬�G���[�ł��B")));

			D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;

			::ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

			// �e�N�X�`���[�쐬���Ɏw�肵���t�H�[�}�b�g�ƌ݊���������A�[�x�X�e���V���r���[�Ƃ��Ďw��ł���t�H�[�}�b�g���w�肷��
			switch (Tex2DDesc.Format)
			{
			case DXGI_FORMAT_R16_TYPELESS:
				DSVDesc.Format = DXGI_FORMAT_D16_UNORM;
				break;
			case DXGI_FORMAT_R32_TYPELESS:
				DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
				break;
			case DXGI_FORMAT_R24G8_TYPELESS:
				DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				break;
			default:
				DSVDesc.Format = DSVDesc.Format;
			}

			switch (ArraySize)
			{
				// �z��Ȃ�
			case 1:
				DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				DSVDesc.Texture2D.MipSlice = 0;
				break;
				// �z�񂠂�
			default:
				DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				DSVDesc.Texture2DArray.ArraySize = ArraySize;
				DSVDesc.Texture2DArray.MipSlice = 0;
				DSVDesc.Texture2DArray.FirstArraySlice = 0;
				break;
			}
			// �[�x�X�e���V���r���[���쐬
			hr = device->CreateDepthStencilView(pTex2D, &DSVDesc, ppDSView);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateDepthStencilView()�ŃG���[���������܂����B�[�x�X�e���V���r���[���쐬�ł��܂���B")));

			if (ppSRView != nullptr)
				maps::CreateSRViewFromDSView(device, *ppDSView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex2D);
		}
	}

	// �[�x�X�e���V���r���[����V�F�[�_�[���\�[�X�r���[���쐬����
	void CreateSRViewFromDSView(ID3D11Device* device, ID3D11DepthStencilView* pDSView, ID3D11ShaderResourceView** ppSRView)
	{
		HRESULT hr = E_FAIL;
		ID3D11Resource* pResource = nullptr;

		__try
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
			pDSView->GetDesc(&DSVDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			::ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			switch (DSVDesc.ViewDimension)
			{
			case D3D11_DSV_DIMENSION_TEXTURE2D:
				SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
				SRVDesc.Texture2D.MipLevels = 1;
				break;
			case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
				SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
				SRVDesc.Texture2DArray.ArraySize = DSVDesc.Texture2DArray.ArraySize;
				SRVDesc.Texture2DArray.MipLevels = 1;
				break;
			}

			switch (DSVDesc.Format)
			{
			case DXGI_FORMAT_D16_UNORM:
				SRVDesc.Format = DXGI_FORMAT_R16_FLOAT;
				break;
			case DXGI_FORMAT_D32_FLOAT:
				SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
				SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			default:
				SRVDesc.Format = DSVDesc.Format;
			}

			// �[�x�X�e���V���r���[����e�N�X�`���[���擾����
			pDSView->GetResource(&pResource);

			// �V�F�[�_�[���\�[�X�r���[���쐬����
			if (FAILED(hr = device->CreateShaderResourceView(pResource, &SRVDesc, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewFromDSView()�ŃG���[���������܂����B�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���B")));
		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// �m�C�Y�}�b�v���쐬����
	void CreateSRViewOfNoiseMap(ID3D11Device* device, UINT Width, UINT Height, DXGI_FORMAT format, ID3D11ShaderResourceView** ppSRView)
	{
		HRESULT hr = E_FAIL;
		ID3D11Texture2D* pTexture2D = nullptr;
		float* pBits = nullptr;
		DWORD BitsCount = 0;

		__try
		{
			D3D11_SUBRESOURCE_DATA SubResourceData;
			::ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

			switch (format)
			{
			case DXGI_FORMAT_R32_FLOAT:
				BitsCount = Width * Height;
				pBits = NEW float[BitsCount];
				for (DWORD i = 0; i < BitsCount; i++)
					pBits[i] = (float)(rand() % 255) / 254.0f;
				SubResourceData.pSysMem = (LPVOID)pBits;
				SubResourceData.SysMemPitch = Width * sizeof(float);
				break;

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				BitsCount = Width * Height * 4;
				pBits = NEW float[BitsCount];
				for (DWORD i = 0; i < BitsCount; i++)
					pBits[i] = (float)(rand() % 255) / 254.0f;
				SubResourceData.pSysMem = (LPVOID)pBits;
				SubResourceData.SysMemPitch = Width * sizeof(float) * 4;
				break;
			default:
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()�ŃG���[���������܂����B���Ή��̃t�H�[�}�b�g���w�肳��܂����B")));
				break;
			}

			D3D11_TEXTURE2D_DESC Tex2DDesc;
			::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
			Tex2DDesc.ArraySize = 1;
			Tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
			Tex2DDesc.Format = format;
			Tex2DDesc.Width = Width;
			Tex2DDesc.Height = Height;
			Tex2DDesc.MipLevels = 1;
			Tex2DDesc.SampleDesc.Count = 1;

			// �e�N�X�`���[�쐬
			if (FAILED(hr = device->CreateTexture2D(&Tex2DDesc, &SubResourceData, &pTexture2D)))
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()�ŃG���[���������܂����B�e�N�X�`���[�̍쐬�Ɏ��s���܂���")));

			// �V�F�[�_�[���\�[�X�r���[���쐬
			if (FAILED(hr = device->CreateShaderResourceView(pTexture2D, nullptr, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()�ŃG���[���������܂����B�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���")));
		}
		__finally
		{
			SAFE_DELETE_ARRAY(pBits);
			SAFE_RELEASE(pTexture2D);
		}
	}
	
}

namespace buffers
{
	//���_�A�C���f�b�N�X�o�b�t�@���쐬����
	ID3D11Buffer* CreateBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag)
	{
		HRESULT hr = E_FAIL;
		ID3D11Buffer* pBuffer = nullptr;

		//�o�b�t�@�[���\�[�X
		D3D11_BUFFER_DESC BufferDesc = { 0 };

		//�T�u���\�[�X
		D3D11_SUBRESOURCE_DATA* resource = nullptr;

		D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
		UINT CPUAccessFlags = 0;

		__try
		{
			switch (CPUAccessFlag)
			{
				//CPU�A�N�Z�X�������Ȃ�
			case 0:
				Usage = D3D11_USAGE_DEFAULT;
				CPUAccessFlags = CPUAccessFlag;
				break;
				//CPU�A�N�Z�X��������
			default:
				Usage = D3D11_USAGE_DYNAMIC;
				CPUAccessFlags = CPUAccessFlag;
				break;
			}

			//�����l��ݒ肷��
			if (pData)
			{
				resource = new D3D11_SUBRESOURCE_DATA();
				resource->pSysMem = pData;
				resource->SysMemPitch = 0;
				resource->SysMemSlicePitch = 0;
			}

			//�o�b�t�@�̐ݒ�
			BufferDesc.ByteWidth = size;		//�o�b�t�@�̃T�C�Y
			BufferDesc.Usage = Usage;			//���\�[�X�̎g�p�@

			switch (BindFlag)
			{
			case D3D11_BIND_STREAM_OUTPUT:
				BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
				break;
			default:
				BufferDesc.BindFlags = BindFlag;
				break;
			}
			BufferDesc.CPUAccessFlags = CPUAccessFlags;
			BufferDesc.MiscFlags = 0;

			//�o�b�t�@�̍쐬
			hr = device->CreateBuffer(&BufferDesc, resource, &pBuffer);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateBuffer()�ŃG���[���������܂����B�o�b�t�@�쐬�G���[�ł��B")));
		}
		__finally
		{
			SAFE_DELETE(resource);
		}

		return pBuffer;
	}
}

UCDirect3D11::UCDirect3D11()
{
	m_hWnd = nullptr;
	m_pD3DDevice = nullptr;
	m_pD3DDeviceContext = nullptr;
	m_pAdapter = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	m_UseDepthBuffer = TRUE;
	m_UseMultiSample = TRUE;
}

UCDirect3D11::~UCDirect3D11()
{
	Invalidate();
}

//�������̉��
void UCDirect3D11::Invalidate()
{
	ULONG cnt;
	TCHAR s[256];

	if (m_pDepthStencilView)
	{
		cnt = m_pDepthStencilView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***ID3D11DepthStencilView�̎Q�ƃJ�E���^���s���ł�"));
			OutputDebugString(s);
		}
		m_pDepthStencilView = nullptr;
	}

	if (m_pRenderTargetView)
	{
		cnt = m_pRenderTargetView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pRenderTargetView�̎Q�ƃJ�E���^���s���ł��B:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pRenderTargetView = nullptr;
	}

	if (m_pSwapChain)
	{
		// �X���b�v �`�F�[�����t���X�N���[�� ���[�h�ŉ���ł��Ȃ����Ƃ�����炵���B
		// �t���X�N���[���̏ꍇ�E�B���h�E���[�h�ɐ؂�ւ��Ă���������K�v������炵���B
		// DXGI �̊T�v
		BOOL FullScreen;
		m_pSwapChain->GetFullscreenState(&FullScreen, nullptr);
		// �t���X�N���[���̂Ƃ�
		if (FullScreen == TRUE)
			m_pSwapChain->SetFullscreenState(FALSE, nullptr);

		cnt = m_pSwapChain->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pSwapChain�̎Q�ƃJ�E���^���s���ł��B:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pSwapChain = nullptr;
	}

	if (m_pD3DDeviceContext)
	{
		cnt = m_pD3DDeviceContext->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pD3DDeviceContext�̎Q�ƃJ�E���^���s���ł��B:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pD3DDeviceContext = nullptr;
	}
	if (m_pD3DDevice)
	{
		cnt = m_pD3DDevice->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pD3DDevice�̎Q�ƃJ�E���^���s���ł��B:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pD3DDevice = nullptr;
	}

	if (m_pAdapter)
	{
		cnt = m_pAdapter->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pAdapter�̎Q�ƃJ�E���^���s���ł��B:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pAdapter = nullptr;
	}
}

//�f�t�H���g�f�B�X�v���C�̃��[�h��񋓂���
void UCDirect3D11::GetDisplayMode(DXGI_MODE_DESC* pModeDesc, UINT* pNum)
{
	HRESULT hr = E_FAIL;
	IDXGIOutput* pOutput = nullptr;

	__try
	{
		CreateAdapter();

		hr = m_pAdapter->EnumOutputs(0, &pOutput);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::GetDisplayMode()�ŃG���[���������܂����B�A�_�v�^�[�ɐڑ�����Ă���v���C�}���f�B�X�v���C�̎擾�Ɏ��s���܂����B")));

		hr = pOutput->GetDisplayModeList(UD3D11_FORMAT, 0, pNum, pModeDesc);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::GetDisplayMode()�ŃG���[���������܂����B�f�B�X�v���C���[�h�̈ꗗ�̎擾�Ɏ��s���܂����B")));

		if (pModeDesc == nullptr)
		{
			//0���Ԃ邱�Ƃ����邩��
			if (*pNum == 0)
				throw(Exception(hr, _T("UDirect3D11::GetDisplayMode()�ŃG���[���������܂����B�f�B�X�v���C���[�h��1������Ƃ��ł��܂���ł����B")));
		}
	}
	__finally
	{
		SAFE_RELEASE(pOutput);
	}
}

//�f�t�H���g�A�_�v�^�[�̃C���^�[�t�F�[�X�̍쐬
//�A�_�v�^�͈��
void UCDirect3D11::CreateAdapter()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = nullptr;

	if (m_pAdapter == nullptr)
	{
		__try
		{
			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
			if (FAILED(hr))
				throw(Exception(hr, _T("UCDirect3D11::CreateAdapter()�ŃG���[���������܂����B�t�@�N�g���̍쐬�G���[�ł�")));

			hr = pFactory->EnumAdapters(0, &m_pAdapter);
			if (FAILED(hr))
				throw(Exception(hr, _T("UCDirect3D11::CreateAdapter()�ŃG���[���������܂����B�f�t�H���g�A�_�v�^�̍쐬�G���[�ł��B")));
		}
		__finally
		{
			SAFE_RELEASE(pFactory);
		}
	}
}

void UCDirect3D11::CreateDirect3D11(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode,
	BOOL UseMultisample, BOOL UseDepthBuffer)
{
	m_UseDepthBuffer = UseDepthBuffer;
	m_UseMultiSample = UseMultisample;

	__try
	{
		InitWindow(AppName, hInstance, WndProc, pDisplayMode);

		CreateAdapter();

		CreateDevice();

		CreateSwapChain(pDisplayMode);

		CreateRenderTargetView();

		CreateDepthStencilView();

		//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		m_pD3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		CreateViewport();

		//�\�����[�h�̎����؂�ւ��𖳌��ɂ���
		//�����؂�ւ���L���ɂ������Ƃ��͂��̊֐����R�����g�A�E�g����
		//�������R�����g�A�E�g����Ə����N�����t���X�N���[�����[�h�ɂ������Ƃ��ɐ��������삵�Ȃ�
		SetWindowAssociation();

		SetDefaultRasterize();

		SetDefaultDepthStencilState();
	}
	__finally
	{
		SAFE_RELEASE( m_pAdapter );
	}
}

void UCDirect3D11::InitWindow(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode)
{
	//�E�B���h�E���[�h
	//https://msdn.microsoft.com/ja-jp/library/czada357.aspx
	DWORD WindowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU;

	//�E�B���h�E�ڍ�
	//https://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(DWORD);
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = AppName;
	wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
	wc.hInstance = hInstance;
	::RegisterClassEx(&wc);

	RECT rc;
	::SetRect(&rc, 0, 0, pDisplayMode->Width, pDisplayMode->Height);

	::AdjustWindowRect(&rc, WindowStyle, FALSE);

	m_hWnd = ::CreateWindow(
		wc.lpszClassName,
		wc.lpszClassName,
		WindowStyle,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
		);

	if (m_hWnd == nullptr)
		throw(Exception(-1, _T("UCDirect3D11::Initindow()�ŃG���[���������܂����B�E�B���h�E���쐬�ł��܂���B")));

}

void UCDirect3D11::CreateDevice()
{
	HRESULT hr = E_FAIL;

	//�f�o�C�X�쐬���Ɏg�p����r�f�I�J�[�h�̐��\�ꗗ
	//Direct3D 10 or Direct3D 11 �݂̂�Ή�����
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,					//Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

#if defined(DEBUG) || defined(_DEBUG)
	//�f�o�b�O�R���p�C���̏ꍇ�f�o�b�O���C���[��L���ɂ���
	//�\�t�g�E�F�A���C���[
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlag = 0;
#endif

	D3D_FEATURE_LEVEL level;

	hr = D3D11CreateDevice(
		m_pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		createDeviceFlag,
		FeatureLevel,
		3,
		D3D11_SDK_VERSION,
		&m_pD3DDevice,
		&level,
		&m_pD3DDeviceContext
		);

	if (FAILED(hr))
		throw(Exception(hr, _T("UCDirect3D11::CreateDevice()�ŃG���[���������܂����B�f�o�C�X�쐬�G���[�ł��B")));

	DXGI_ADAPTER_DESC desc;
	m_pAdapter->GetDesc(&desc);

	auto version = m_pD3DDevice->GetFeatureLevel();

	//DirectX�̃o�[�W�����A�V�F�[�_���f���̃o�[�W�����ƍ̗p�����A�_�v�^�[�̖��O���f�o�b�O�E�B���h�E�ɏo��
	TCHAR s[256];
	_stprintf_s(s, _T("***�f�o�C�X�̍쐬�ɐ���!DirectX(11.0) SM(5) %s\n"), desc.Description);
	OutputDebugString(s);

}

void UCDirect3D11::CreateSwapChain(DXGI_MODE_DESC* pDisplayMode)
{
	HRESULT hr;
	TCHAR s[256];
	IDXGIFactory* pFactory = nullptr;
	DXGI_SWAP_CHAIN_DESC sd;

	__try
	{
		::ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
		::CopyMemory(&(sd.BufferDesc), pDisplayMode, sizeof(DXGI_MODE_DESC));

		sd.BufferDesc.Format = UD3D11_FORMAT;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�N�o�b�t�@�ɏ������݉\�ɂ���
		sd.BufferCount = 1;
		sd.OutputWindow = m_hWnd;
		//�o�͂��E�B���h�E���[�h�̏ꍇ��TRUE ����ȊO��FALSE
		//�����N�����̓E�B���h�E���[�h�ɌŒ肷��
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//�X���b�v�`�F�[�����@��DirectX�ɔC����
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//�t���X�N���[���ɂ����Ƃ��ɃA�v���P�[�V�����E�B���h�E�̃T�C�Y�ɍ��킹�ĉ𑜓x���ύX�����

		//�t�@�N�g���̍쐬
		//CreateDXGIFactory�ō쐬�����t�@�N�g�����g�p���邷��Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂�IDXGIAdapter����쐬����
		hr = m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateSwapChain()�ŃG���[���������܂����B�t�@�N�g���̎擾�Ɏ��s���܂����B")));

		//�}���`�T���v�����O�̐ݒ�
		//�}���`�T���v�����O���g�p���Ȃ��ꍇ�̐ݒ�
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		int startMultisample;

		//�T���v�����O���ő�̂��̂�I������
		if (m_UseMultiSample == TRUE)
		{
			startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
		}
		else
		{
			startMultisample = 1;
		}

		UINT Quality = 0;
		for (int i = startMultisample; i >= 0; i--)
		{
			hr = m_pD3DDevice->CheckMultisampleQualityLevels(pDisplayMode->Format, (UINT)i, &Quality);
			if (FAILED(hr))
				throw(Exception(hr, _T("UCDirect3D11::CreateSwapChain()�ŃG���[���������܂����B�T�|�[�g����N�I���e�B���x���̍ő�l�̎擾�G���[�ł��B")));

			//�N�I���e�B���x����0�ȊO�̂Ƃ��A�t�H�[�}�b�g�ƃT���v�����O���̑g�ݍ��킹���T�|�[�g����
			//�I�����C���w���v�ł�CheckMultisampleQualityLevels�̖߂�l��0�̂Ƃ��T�|�[�g����Ȃ��Ə�����Ă��邪
			//pNumQualityLebvels�̖߂�l�� 0 ���Ŕ��f����
			if (Quality != 0)
			{
				sd.SampleDesc.Count = i;
				sd.SampleDesc.Quality = Quality - 1;

				//�X���b�v�`�F�[�����쐬����B
				hr = pFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_pSwapChain);
				_stprintf_s(s, _T("***�}���`�T���v�� : Count(%d) Quality(%d)"), sd.SampleDesc.Count, sd.SampleDesc.Quality);
				if (SUCCEEDED(hr))
				{
					if (sd.SampleDesc.Count == 1)
						m_UseMultiSample = FALSE;

					_tcscat_s(s, _T(" => OK\n"));
					OutputDebugString(s);
					break;
				}
				else
				{
					_tcscat_s(s, _T(" => NG\n"));
					OutputDebugString(s);
				}
			}
		}
		if (m_pSwapChain == nullptr)
			throw(Exception(-1, _T("UCDirect3D11::CreateSwapChain()�ŃG���[���������܂����B�X���b�v�`�F�[�����쐬�ł��܂���ł����B")));

		_stprintf_s(s, _T("***�f�B�X�v���C���[�h : �𑜓x( %d : %d ) ���t���b�V�����[�g ( %d / %d )\n"),
			pDisplayMode->Width, pDisplayMode->Height,
			pDisplayMode->RefreshRate.Denominator, pDisplayMode->RefreshRate.Numerator
			);
		OutputDebugString(s);
	}
	__finally
	{
		SAFE_RELEASE(pFactory);
	}
}

void UCDirect3D11::CreateRenderTargetView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D* pBackBuffer = nullptr;

	__try
	{
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateRenderTargetView()�ŃG���[���������܂����B�X���b�v�`�F�[������o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B")));

		hr = m_pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateRenderTargetView()�ŃG���[���������܂����B�����_�����O�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂����B")));
	}
	__finally
	{
		SAFE_RELEASE(pBackBuffer);
	}
}

void UCDirect3D11::CreateDepthStencilView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D* pBackBuffer = nullptr;
	ID3D11Texture2D* pDepthBuffer = nullptr;
	D3D11_TEXTURE2D_DESC descDS;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	DXGI_SWAP_CHAIN_DESC chainDesc;

	if (m_UseDepthBuffer == FALSE)
		return;

	__try
	{
		hr = m_pSwapChain->GetDesc(&chainDesc);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()�ŃG���[���������܂����B�X���b�v�`�F�[���̐ݒ�̎擾�G���[�ł��B")));

		::ZeroMemory(&descDS, sizeof(D3D11_TEXTURE2D_DESC));
		descDS.Width = chainDesc.BufferDesc.Width;
		descDS.Height = chainDesc.BufferDesc.Height;
		descDS.MipLevels = 1;
		descDS.ArraySize = 1;
		descDS.Format = UD3D11_DEPTH_FORMAT;
		descDS.SampleDesc.Count = chainDesc.SampleDesc.Count;
		descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality;
		descDS.Usage = D3D11_USAGE_DEFAULT;
		descDS.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descDS.CPUAccessFlags = 0;
		descDS.MiscFlags = 0;
		//�[�x�o�b�t�@�p�e�N�X�`���̍쐬
		hr = m_pD3DDevice->CreateTexture2D(&descDS, nullptr, &pDepthBuffer);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()�G���[���������܂����B�[�x�o�b�t�@�p�̃e�N�X�`���쐬�ɂ����ς����܂����B")));

		::ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		//�e�N�X�`���쐬���Ɏw�肵���t�H�[�}�b�g�ƌ݊���������A�[�x�X�e���V���r���[�Ƃ��Ďw��ł���t�H�[�}�b�g���w�肷��
		switch (descDS.Format)
		{
		case DXGI_FORMAT_R8_TYPELESS:
			descDSV.Format = DXGI_FORMAT_R8_UNORM;
			break;
		case DXGI_FORMAT_R16_TYPELESS:
			descDSV.Format = DXGI_FORMAT_D16_UNORM;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			descDSV.Format = DXGI_FORMAT_D32_FLOAT;
			break;
		case DXGI_FORMAT_R24G8_TYPELESS:
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			descDSV.Format = descDS.Format;
		}

		//�}���`�T���v�����g�p���Ă���ꍇ
		if (chainDesc.SampleDesc.Count > 1)
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		//�}���`�T���v�����g�p���Ȃ��ꍇ
		else
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		descDSV.Texture2D.MipSlice = 0;

		//�[�x�X�e���V���r���[�̍쐬
		hr = m_pD3DDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()�ŃG���[���������܂����B�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B")));

	}
	__finally
	{
		SAFE_RELEASE(pDepthBuffer);
		SAFE_RELEASE(pBackBuffer);
	}
}

void UCDirect3D11::CreateViewport()
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC chainDesc;

	hr = m_pSwapChain->GetDesc(&chainDesc);
	if (FAILED(hr))
		throw(Exception(hr, _T("UCDirect3D11::CreateViewport()�ŃG���[���������܂����B�X���b�v�`�F�[���̐ݒ�̎擾�Ɏ��s���܂����B")));

	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = 0;
	Viewport[0].TopLeftY = 0;
	Viewport[0].Width = (FLOAT)chainDesc.BufferDesc.Width;
	Viewport[0].Height = (FLOAT)chainDesc.BufferDesc.Height;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;
	
	m_pD3DDeviceContext->RSSetViewports(1, Viewport);

}

// �E�B���h�E�A�\�V�G�[�V����
void UCDirect3D11::SetWindowAssociation()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = nullptr;

	__try
	{
		// �t�@�N�g���[���쐬����
		// CreateDXGIFactory�ō쐬�����t�@�N�g���[���g�p����Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂� IDXGIAdapter ����쐬����B
		hr = m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetWindowAssociation()�ŃG���[���������܂����B�t�@�N�g���[�̎擾�G���[�ł��B")));

		// �\�����[�h�̎����؊����𖳌��ɂ���B
		hr = pFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetWindowAssociation()�ŃG���[���������܂����B�E�B���h�E�A�\�V�G�[�V�����̍쐬�G���[�ł��B")));
	}
	__finally
	{
		SAFE_RELEASE(pFactory);
	}
}

void UCDirect3D11::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;
	ID3D11RasterizerState* pRasterState = nullptr;
	D3D11_RASTERIZER_DESC rsState;

	__try
	{
		rsState.FillMode = D3D11_FILL_SOLID;	//�|���S���ʕ`��
		rsState.CullMode = D3D11_CULL_BACK;		//���ʂ�`�悵�Ȃ�
		rsState.FrontCounterClockwise = TRUE;	//�����v����\��
		rsState.DepthBias = 0;
		rsState.DepthBiasClamp = 0;
		rsState.SlopeScaledDepthBias = 0;
		rsState.DepthClipEnable = TRUE;
		rsState.ScissorEnable = FALSE;			//�V�U�[��`����

		//�X���b�v�`�F�[���̃}���`�T���v�����O�̐ݒ�ɍ��킹��
		DXGI_SWAP_CHAIN_DESC swapDesc;
		m_pSwapChain->GetDesc(&swapDesc);
		if (swapDesc.SampleDesc.Count != 1)
			rsState.MultisampleEnable = TRUE;
		else
			rsState.MultisampleEnable = FALSE;

		rsState.AntialiasedLineEnable = FALSE;
		hr = m_pD3DDevice->CreateRasterizerState(&rsState, &pRasterState);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetDefaultRasterize()�ŃG���[���������܂����B���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂����B")));

		m_pD3DDeviceContext->RSSetState(pRasterState);

	}
	__finally
	{
		SAFE_RELEASE(pRasterState);
	}

}

//�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
void UCDirect3D11::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;
	ID3D11DepthStencilState* pDepthStencilState = nullptr;
	D3D11_DEPTH_STENCIL_DESC dsState;

	__try
	{
		::ZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsState.DepthEnable = TRUE;
		dsState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsState.DepthFunc = D3D11_COMPARISON_LESS;
		dsState.StencilEnable = FALSE;

		hr = m_pD3DDevice->CreateDepthStencilState(&dsState, &pDepthStencilState);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetDefaultDepthStencilState()�ŃG���[���������܂����B�[�x�X�e���V���X�e�[�g�̍쐬�G���[�ł��B")));

		m_pD3DDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}
	__finally
	{
		SAFE_RELEASE(pDepthStencilState);
	}
}

HRESULT UCDirect3D11::Present(UINT SyncInterval, UINT Flags)
{
	HRESULT hr = E_FAIL;
	if (FAILED(hr = m_pSwapChain->Present(SyncInterval, Flags)))
		throw(Exception(hr, _T("UCDirect3D11::Present()�ŃG���[���������܂����B")));

	return hr;
}