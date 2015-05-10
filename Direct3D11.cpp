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
				throw(Exception(hr, _T("CreateRenderTarget()でエラーが発生しました。テクスチャが作成できません")));
			if (FAILED(hr = device->CreateRenderTargetView(pTex2D, nullptr, ppRTView)))
				throw(Exception(hr, _T("CreateRenderTargetView()でエラーが発生しました。レンダーターゲットビューが作成できません")));
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
				throw(Exception(hr, _T("CreateRenderTargetViewOfVolumeTexture()でエラーが発生しました。テクスチャが作成できません")));
			if (FAILED(hr = device->CreateRenderTargetView(pTex3D, nullptr, ppRTView)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfVolumeTexture()でエラーが発生しました。レンダーターゲットビューが作成できません")));
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
			//レンダーターゲットビューからテクスチャを取得する
			pRTView->GetResource(&pResource);

			if (FAILED(hr = device->CreateShaderResourceView(pResource, nullptr, ppSRView)))
				throw (Exception(hr, _T("CreateSRViewFromRTView()でエラーが発生しました。シェーダーリソースビューが作成できません")));

		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// アンオーダードアクセスビューを作成する
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
				throw(Exception(hr, _T("CreateRenderTargetViewOfRWTexture()でエラーが発生しました。テクスチャーが作成できません。")));

			if (FAILED(hr = device->CreateUnorderedAccessView(pTex2D, nullptr, ppUAView)))
				throw(Exception(hr, _T("CreateRenderTargetViewOfRWTexture()でエラーが発生しました。アンオーダードアクセスビューが作成できません。")));

			if (ppSRView != nullptr)
				maps::CreateSRViewFromUAView(device, *ppUAView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex2D);
		}
	}

	// アンオーダードアクセスビューからシェーダーリソースビューを作成する
	void CreateSRViewFromUAView(ID3D11Device* device, ID3D11UnorderedAccessView* pUAView, ID3D11ShaderResourceView** ppSRView)
	{
		HRESULT hr = E_FAIL;
		ID3D11Resource* pResource = nullptr;

		__try
		{
			// アンオーダードアクセスビューからテクスチャーを取得する
			pUAView->GetResource(&pResource);

			// シェーダーリソースビューを作成する
			if (FAILED(hr = device->CreateShaderResourceView(pResource, nullptr, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewFromUAView()でエラーが発生しました。シェーダーリソースビューが作成できません。")));
		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// 深度ステンシルビューを作成する
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

			// 深度バッファ用のテクスチャー作成
			hr = device->CreateTexture2D(&Tex2DDesc, nullptr, &pTex2D);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateDepthStencilView()でエラーが発生しました。深度バッファ用のテクスチャー作成エラーです。")));

			D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;

			::ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

			// テクスチャー作成時に指定したフォーマットと互換性があり、深度ステンシルビューとして指定できるフォーマットを指定する
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
				// 配列なし
			case 1:
				DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				DSVDesc.Texture2D.MipSlice = 0;
				break;
				// 配列あり
			default:
				DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				DSVDesc.Texture2DArray.ArraySize = ArraySize;
				DSVDesc.Texture2DArray.MipSlice = 0;
				DSVDesc.Texture2DArray.FirstArraySlice = 0;
				break;
			}
			// 深度ステンシルビューを作成
			hr = device->CreateDepthStencilView(pTex2D, &DSVDesc, ppDSView);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateDepthStencilView()でエラーが発生しました。深度ステンシルビューが作成できません。")));

			if (ppSRView != nullptr)
				maps::CreateSRViewFromDSView(device, *ppDSView, ppSRView);
		}
		__finally
		{
			SAFE_RELEASE(pTex2D);
		}
	}

	// 深度ステンシルビューからシェーダーリソースビューを作成する
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

			// 深度ステンシルビューからテクスチャーを取得する
			pDSView->GetResource(&pResource);

			// シェーダーリソースビューを作成する
			if (FAILED(hr = device->CreateShaderResourceView(pResource, &SRVDesc, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewFromDSView()でエラーが発生しました。シェーダーリソースビューが作成できません。")));
		}
		__finally
		{
			SAFE_RELEASE(pResource);
		}
	}

	// ノイズマップを作成する
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
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()でエラーが発生しました。未対応のフォーマットが指定されました。")));
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

			// テクスチャー作成
			if (FAILED(hr = device->CreateTexture2D(&Tex2DDesc, &SubResourceData, &pTexture2D)))
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()でエラーが発生しました。テクスチャーの作成に失敗しました")));

			// シェーダーリソースビューを作成
			if (FAILED(hr = device->CreateShaderResourceView(pTexture2D, nullptr, ppSRView)))
				throw(Exception(hr, _T("CreateSRViewOfNoiseMap()でエラーが発生しました。シェーダーリソースビューの作成に失敗しました")));
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
	//頂点、インデックスバッファを作成する
	ID3D11Buffer* CreateBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag)
	{
		HRESULT hr = E_FAIL;
		ID3D11Buffer* pBuffer = nullptr;

		//バッファーリソース
		D3D11_BUFFER_DESC BufferDesc = { 0 };

		//サブリソース
		D3D11_SUBRESOURCE_DATA* resource = nullptr;

		D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
		UINT CPUAccessFlags = 0;

		__try
		{
			switch (CPUAccessFlag)
			{
				//CPUアクセスを許可しない
			case 0:
				Usage = D3D11_USAGE_DEFAULT;
				CPUAccessFlags = CPUAccessFlag;
				break;
				//CPUアクセスを許可する
			default:
				Usage = D3D11_USAGE_DYNAMIC;
				CPUAccessFlags = CPUAccessFlag;
				break;
			}

			//初期値を設定する
			if (pData)
			{
				resource = new D3D11_SUBRESOURCE_DATA();
				resource->pSysMem = pData;
				resource->SysMemPitch = 0;
				resource->SysMemSlicePitch = 0;
			}

			//バッファの設定
			BufferDesc.ByteWidth = size;		//バッファのサイズ
			BufferDesc.Usage = Usage;			//リソースの使用法

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

			//バッファの作成
			hr = device->CreateBuffer(&BufferDesc, resource, &pBuffer);
			if (FAILED(hr))
				throw(Exception(hr, _T("CreateBuffer()でエラーが発生しました。バッファ作成エラーです。")));
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

//メモリの解放
void UCDirect3D11::Invalidate()
{
	ULONG cnt;
	TCHAR s[256];

	if (m_pDepthStencilView)
	{
		cnt = m_pDepthStencilView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***ID3D11DepthStencilViewの参照カウンタが不正です"));
			OutputDebugString(s);
		}
		m_pDepthStencilView = nullptr;
	}

	if (m_pRenderTargetView)
	{
		cnt = m_pRenderTargetView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pRenderTargetViewの参照カウンタが不正です。:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pRenderTargetView = nullptr;
	}

	if (m_pSwapChain)
	{
		// スワップ チェーンをフルスクリーン モードで解放できないことがあるらしい。
		// フルスクリーンの場合ウィンドウモードに切り替えてから解放する必要があるらしい。
		// DXGI の概要
		BOOL FullScreen;
		m_pSwapChain->GetFullscreenState(&FullScreen, nullptr);
		// フルスクリーンのとき
		if (FullScreen == TRUE)
			m_pSwapChain->SetFullscreenState(FALSE, nullptr);

		cnt = m_pSwapChain->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pSwapChainの参照カウンタが不正です。:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pSwapChain = nullptr;
	}

	if (m_pD3DDeviceContext)
	{
		cnt = m_pD3DDeviceContext->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pD3DDeviceContextの参照カウンタが不正です。:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pD3DDeviceContext = nullptr;
	}
	if (m_pD3DDevice)
	{
		cnt = m_pD3DDevice->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pD3DDeviceの参照カウンタが不正です。:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pD3DDevice = nullptr;
	}

	if (m_pAdapter)
	{
		cnt = m_pAdapter->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("***m_pAdapterの参照カウンタが不正です。:( %d )\n"), cnt);
			OutputDebugString(s);
		}
		m_pAdapter = nullptr;
	}
}

//デフォルトディスプレイのモードを列挙する
void UCDirect3D11::GetDisplayMode(DXGI_MODE_DESC* pModeDesc, UINT* pNum)
{
	HRESULT hr = E_FAIL;
	IDXGIOutput* pOutput = nullptr;

	__try
	{
		CreateAdapter();

		hr = m_pAdapter->EnumOutputs(0, &pOutput);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::GetDisplayMode()でエラーが発生しました。アダプターに接続されているプライマリディスプレイの取得に失敗しました。")));

		hr = pOutput->GetDisplayModeList(UD3D11_FORMAT, 0, pNum, pModeDesc);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::GetDisplayMode()でエラーが発生しました。ディスプレイモードの一覧の取得に失敗しました。")));

		if (pModeDesc == nullptr)
		{
			//0が返ることがあるかも
			if (*pNum == 0)
				throw(Exception(hr, _T("UDirect3D11::GetDisplayMode()でエラーが発生しました。ディスプレイモードが1つもしゅとくできませんでした。")));
		}
	}
	__finally
	{
		SAFE_RELEASE(pOutput);
	}
}

//デフォルトアダプターのインターフェースの作成
//アダプタは一つ
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
				throw(Exception(hr, _T("UCDirect3D11::CreateAdapter()でエラーが発生しました。ファクトリの作成エラーです")));

			hr = pFactory->EnumAdapters(0, &m_pAdapter);
			if (FAILED(hr))
				throw(Exception(hr, _T("UCDirect3D11::CreateAdapter()でエラーが発生しました。デフォルトアダプタの作成エラーです。")));
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

		//レンダーターゲットビューの設定
		m_pD3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		CreateViewport();

		//表示モードの自動切り替えを無効にする
		//自動切り替えを有効にしたいときはこの関数をコメントアウトする
		//しかしコメントアウトすると初期起動をフルスクリーンモードにしたいときに正しく動作しない
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
	//ウィンドウモード
	//https://msdn.microsoft.com/ja-jp/library/czada357.aspx
	DWORD WindowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU;

	//ウィンドウ詳細
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
		throw(Exception(-1, _T("UCDirect3D11::Initindow()でエラーが発生しました。ウィンドウが作成できません。")));

}

void UCDirect3D11::CreateDevice()
{
	HRESULT hr = E_FAIL;

	//デバイス作成時に使用するビデオカードの性能一覧
	//Direct3D 10 or Direct3D 11 のみを対応する
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,					//Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

#if defined(DEBUG) || defined(_DEBUG)
	//デバッグコンパイルの場合デバッグレイヤーを有効にする
	//ソフトウェアレイヤー
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
		throw(Exception(hr, _T("UCDirect3D11::CreateDevice()でエラーが発生しました。デバイス作成エラーです。")));

	DXGI_ADAPTER_DESC desc;
	m_pAdapter->GetDesc(&desc);

	auto version = m_pD3DDevice->GetFeatureLevel();

	//DirectXのバージョン、シェーダモデルのバージョンと採用したアダプターの名前をデバッグウィンドウに出力
	TCHAR s[256];
	_stprintf_s(s, _T("***デバイスの作成に成功!DirectX(11.0) SM(5) %s\n"), desc.Description);
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
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //バックバッファに書き込み可能にする
		sd.BufferCount = 1;
		sd.OutputWindow = m_hWnd;
		//出力がウィンドウモードの場合はTRUE それ以外はFALSE
		//初期起動時はウィンドウモードに固定する
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//スワップチェーン方法をDirectXに任せる
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//フルスクリーンにしたときにアプリケーションウィンドウのサイズに合わせて解像度が変更される

		//ファクトリの作成
		//CreateDXGIFactoryで作成したファクトリを使用するすると実行できるがワーニングエラーになるのでIDXGIAdapterから作成する
		hr = m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateSwapChain()でエラーが発生しました。ファクトリの取得に失敗しました。")));

		//マルチサンプリングの設定
		//マルチサンプリングを使用しない場合の設定
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		int startMultisample;

		//サンプリングが最大のものを選択する
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
				throw(Exception(hr, _T("UCDirect3D11::CreateSwapChain()でエラーが発生しました。サポートするクオリティレベルの最大値の取得エラーです。")));

			//クオリティレベルが0以外のとき、フォーマットとサンプリング数の組み合わせをサポートする
			//オンラインヘルプではCheckMultisampleQualityLevelsの戻り値が0のときサポートされないと書かれているが
			//pNumQualityLebvelsの戻り値が 0 かで判断する
			if (Quality != 0)
			{
				sd.SampleDesc.Count = i;
				sd.SampleDesc.Quality = Quality - 1;

				//スワップチェーンを作成する。
				hr = pFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_pSwapChain);
				_stprintf_s(s, _T("***マルチサンプル : Count(%d) Quality(%d)"), sd.SampleDesc.Count, sd.SampleDesc.Quality);
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
			throw(Exception(-1, _T("UCDirect3D11::CreateSwapChain()でエラーが発生しました。スワップチェーンが作成できませんでした。")));

		_stprintf_s(s, _T("***ディスプレイモード : 解像度( %d : %d ) リフレッシュレート ( %d / %d )\n"),
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
			throw(Exception(hr, _T("UCDirect3D11::CreateRenderTargetView()でエラーが発生しました。スワップチェーンからバックバッファの取得に失敗しました。")));

		hr = m_pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateRenderTargetView()でエラーが発生しました。レンダリングターゲットビューの作成に失敗しました。")));
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
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()でエラーが発生しました。スワップチェーンの設定の取得エラーです。")));

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
		//深度バッファ用テクスチャの作成
		hr = m_pD3DDevice->CreateTexture2D(&descDS, nullptr, &pDepthBuffer);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()エラーが発生しました。深度バッファ用のテクスチャ作成にしっぱいしました。")));

		::ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		//テクスチャ作成時に指定したフォーマットと互換性があり、深度ステンシルビューとして指定できるフォーマットを指定する
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

		//マルチサンプルを使用している場合
		if (chainDesc.SampleDesc.Count > 1)
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		//マルチサンプルを使用しない場合
		else
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		descDSV.Texture2D.MipSlice = 0;

		//深度ステンシルビューの作成
		hr = m_pD3DDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::CreateDepthStencilView()でエラーが発生しました。深度ステンシルビューの作成に失敗しました。")));

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
		throw(Exception(hr, _T("UCDirect3D11::CreateViewport()でエラーが発生しました。スワップチェーンの設定の取得に失敗しました。")));

	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = 0;
	Viewport[0].TopLeftY = 0;
	Viewport[0].Width = (FLOAT)chainDesc.BufferDesc.Width;
	Viewport[0].Height = (FLOAT)chainDesc.BufferDesc.Height;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;
	
	m_pD3DDeviceContext->RSSetViewports(1, Viewport);

}

// ウィンドウアソシエーション
void UCDirect3D11::SetWindowAssociation()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = nullptr;

	__try
	{
		// ファクトリーを作成する
		// CreateDXGIFactoryで作成したファクトリーを使用すると実行できるがワーニングエラーになるので IDXGIAdapter から作成する。
		hr = m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetWindowAssociation()でエラーが発生しました。ファクトリーの取得エラーです。")));

		// 表示モードの自動切換えを無効にする。
		hr = pFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetWindowAssociation()でエラーが発生しました。ウィンドウアソシエーションの作成エラーです。")));
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
		rsState.FillMode = D3D11_FILL_SOLID;	//ポリゴン面描画
		rsState.CullMode = D3D11_CULL_BACK;		//裏面を描画しない
		rsState.FrontCounterClockwise = TRUE;	//反時計回りを表面
		rsState.DepthBias = 0;
		rsState.DepthBiasClamp = 0;
		rsState.SlopeScaledDepthBias = 0;
		rsState.DepthClipEnable = TRUE;
		rsState.ScissorEnable = FALSE;			//シザー矩形無効

		//スワップチェーンのマルチサンプリングの設定に合わせる
		DXGI_SWAP_CHAIN_DESC swapDesc;
		m_pSwapChain->GetDesc(&swapDesc);
		if (swapDesc.SampleDesc.Count != 1)
			rsState.MultisampleEnable = TRUE;
		else
			rsState.MultisampleEnable = FALSE;

		rsState.AntialiasedLineEnable = FALSE;
		hr = m_pD3DDevice->CreateRasterizerState(&rsState, &pRasterState);
		if (FAILED(hr))
			throw(Exception(hr, _T("UCDirect3D11::SetDefaultRasterize()でエラーが発生しました。ラスタライザステートの作成に失敗しました。")));

		m_pD3DDeviceContext->RSSetState(pRasterState);

	}
	__finally
	{
		SAFE_RELEASE(pRasterState);
	}

}

//デフォルトの深度ステンシルステートを設定
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
			throw(Exception(hr, _T("UCDirect3D11::SetDefaultDepthStencilState()でエラーが発生しました。深度ステンシルステートの作成エラーです。")));

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
		throw(Exception(hr, _T("UCDirect3D11::Present()でエラーが発生しました。")));

	return hr;
}