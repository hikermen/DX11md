#include "GraphicsPipeline.h"
#include "DX11ThinWrapper.h"
#include "DirectXShaderLevelDefine.h"

GraphicsPipeline::GraphicsPipeline()
{
	m_pLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pHullShader = nullptr;
	m_pDomainShader = nullptr;
	m_pGeometryShader = nullptr;
	m_pPixelShader = nullptr;
	m_pRasterizerState = nullptr;
	m_pDepthStencilState = nullptr;
	m_pBlendState = nullptr;
}

GraphicsPipeline::~GraphicsPipeline()
{
	Invalidate();
}

void GraphicsPipeline::Invalidate()
{

	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pBlendState);
}

//頂点シェーダーをファイルから作成する
void GraphicsPipeline::CreateVertexShaderFromFile(
	ID3D11Device* device,
	TCHAR pSrcFile[], CHAR pFunctionName[],
	D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
	)
{
	auto vs = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, VERTEX_SHADER_PROFILE);

	m_pVertexShader = dx11::d3::shader::CreateVertexShader(device, vs.get());

	m_pLayout = dx11::d3::shader::CreateInputLayout(device, pLayoutDesc, NumElements, vs.get());
}

//頂点シェーダをメモリから作成する
void GraphicsPipeline::CreateVertexShaderFromMemory(
	ID3D11Device* device,
	BYTE* pShader, size_t size,
	D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
	)
{
	m_pVertexShader = dx11::d3::shader::detail::CreateVertexShader(device, pShader, size);

	m_pLayout = dx11::d3::shader::detail::CreateInputLayout(device, pLayoutDesc, NumElements, pShader, size);

}

// ジオメトリシェーダーをファイルから作成する
void GraphicsPipeline::CreateGeometryShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR  pFunctionName[])
{
	auto gs = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, GEOMETORY_SHADER_PROFILE);

	m_pGeometryShader = dx11::d3::shader::CreateGeometryShader(device, gs.get());
}

// ジオメトリシェーダーをメモリから作成する
void GraphicsPipeline::CreateGeometryShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pGeometryShader = dx11::d3::shader::detail::CreateGeometryShader(device, pShader, size);

}

// ピクセルシェーダーをファイルから作成する
void GraphicsPipeline::CreatePixelShaderFromFile(ID3D11Device* device,
	TCHAR pSrcFile[],
	CHAR  pFunctionName[]
	)
{
	auto ps = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, PIXEL_SHADER_PROFILE);

	m_pPixelShader = dx11::d3::shader::CreatePixelShader(device, ps.get());
}

// ピクセルシェーダーをメモリから作成する
void GraphicsPipeline::CreatePixelShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pPixelShader = dx11::d3::shader::detail::CreatePixelShader(device, pShader, size);
}


// ハルシェーダーをファイルから作成する
void GraphicsPipeline::CreateHullShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[])
{
	auto hs = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, "hs_5_0");

	m_pHullShader = dx11::d3::shader::CreateHullShader(device, hs.get());
}

// ハルシェーダーをメモリから作成する
void GraphicsPipeline::CreateHullShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pHullShader = dx11::d3::shader::detail::CreateHullShader(device, pShader, size);
}

// ドメインシェーダーをファイルから作成する
void GraphicsPipeline::CreateDomainShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[])
{
	auto ds = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, "ds_5_0");

	m_pDomainShader = dx11::d3::shader::CreateDomainShader(device, ds.get());
}

// ドメインシェーダーをメモリから作成する
void GraphicsPipeline::CreateDomainShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pDomainShader = dx11::d3::shader::detail::CreateDomainShader(device, pShader, size);
}


// ラスタライザステートを作成
void GraphicsPipeline::CreateRasterizerState(ID3D11Device* pD3D11Device, D3D11_CULL_MODE CullMode)
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC RasterizerDesc;

	::ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));
	RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;    // ポリゴン面描画
	RasterizerDesc.CullMode = CullMode;            // 指定の方向を向いている三角形をカリングする
	RasterizerDesc.FrontCounterClockwise = TRUE;   // 反時計回りを表面
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.ScissorEnable = FALSE;          // シザー矩形無効
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	SAFE_RELEASE(m_pRasterizerState);

	if (FAILED(hr = pD3D11Device->CreateRasterizerState(&RasterizerDesc, &m_pRasterizerState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateRasterizerState()でエラーが発生しました。ID3D11RasterizerStateインターフェースの作成エラーです。")));
}

// 深度ステンシルステートを作成
void GraphicsPipeline::CreateDepthStencilState(ID3D11Device* device, BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled, D3D11_COMPARISON_FUNC DepthFunc)
{
	HRESULT hr;

	D3D11_DEPTH_STENCIL_DESC DSDesc;

	DSDesc.DepthEnable = DepthEnable;           // 深度テストを行うか
	DSDesc.DepthWriteMask = DepthWriteEnabled;  // 深度バッファへの書き込みを行うか
	DSDesc.DepthFunc = DepthFunc;
	DSDesc.StencilEnable = FALSE;

	SAFE_RELEASE(m_pDepthStencilState);

	if (FAILED(hr = device->CreateDepthStencilState(&DSDesc, &m_pDepthStencilState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateDepthStencilState()でエラーが発生しました。ID3D11DepthStencilStateインターフェースの作成エラーです。")));
}

// ブレンドステートを作成
void GraphicsPipeline::CreateBlendState(ID3D11Device* device, UEBLEND_STATE* BlendStateType, UINT BlendStateTypeLength, BOOL AlphaToCoverageEnable)
{
	HRESULT hr;
	D3D11_BLEND_DESC BlendDesc;

	if (BlendStateTypeLength == 0 || BlendStateTypeLength >= 8)
		throw(Exception(-1, _T("GraphicsPipeline::CreateBlendState()でエラーが発生しました。仮引数が不正です。")));

	::ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;

	if (BlendStateTypeLength == 1)
		BlendDesc.IndependentBlendEnable = FALSE;
	else
		BlendDesc.IndependentBlendEnable = TRUE;

	for (UINT i = 0; i<BlendStateTypeLength; i++)
	{
		switch (BlendStateType[i])
		{
		case UEBLEND_STATE::NONE:
			BlendDesc.RenderTarget[i].BlendEnable = FALSE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

			// 加算合成
		case UEBLEND_STATE::ADD:
			BlendDesc.RenderTarget[i].BlendEnable = TRUE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

			// 線形合成
		case UEBLEND_STATE::ALIGNMENT:
			BlendDesc.RenderTarget[i].BlendEnable = TRUE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

			// 深度テスト
		case UEBLEND_STATE::DEPTH_TEST:
			BlendDesc.RenderTarget[i].BlendEnable = TRUE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_MIN;      // 最前面の深度値で上書きするために最小値を選択
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_MIN;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;
		}
	}

	SAFE_RELEASE(m_pBlendState);

	if (FAILED(hr = device->CreateBlendState(&BlendDesc, &m_pBlendState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateBlendState()でエラーが発生しました。ID3D11BlendStateインターフェースの作成エラーです。")));
}

// 定数バッファを作成する
ID3D11Buffer* GraphicsPipeline::CreateConstantBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag)
{
	HRESULT hr = E_FAIL;

	ID3D11Buffer* pBuffer = nullptr;

	// バッファー リソース。
	D3D11_BUFFER_DESC BufferDesc = { 0 };

	// サブリソース
	D3D11_SUBRESOURCE_DATA* resource = nullptr;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = 0;

	__try
	{
		switch (CPUAccessFlag)
		{
			// CPUアクセスを許可しない
		case 0:
			Usage = D3D11_USAGE_DEFAULT;
			CPUAccessFlags = CPUAccessFlag;
			break;
			// CPUアクセスを許可する
		default:
			Usage = D3D11_USAGE_DYNAMIC;
			CPUAccessFlags = CPUAccessFlag;
			break;
		}

		// 初期値を設定する
		if (pData)
		{
			resource = new D3D11_SUBRESOURCE_DATA();
			resource->pSysMem = pData;
			resource->SysMemPitch = 0;
			resource->SysMemSlicePitch = 0;
		}

		// バッファの設定
		BufferDesc.ByteWidth = size;                       // バッファサイズ
		BufferDesc.Usage = Usage;                      // リソース使用法を特定する
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // バッファの種類
		BufferDesc.CPUAccessFlags = CPUAccessFlags;             // CPU アクセス
		BufferDesc.MiscFlags = 0;                          // その他のフラグも設定しない

		// バッファを作成する
		hr = device->CreateBuffer(&BufferDesc, resource, &pBuffer);
		if (FAILED(hr))
			throw(Exception(hr, _T("GraphicsPipeline::CreateConstantBuffer()でエラーが発生しました。バッファ作成エラーです。")));
	}
	__finally
	{
		SAFE_DELETE(resource);
	}

	return pBuffer;
}