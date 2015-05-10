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

//���_�V�F�[�_�[���t�@�C������쐬����
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

//���_�V�F�[�_������������쐬����
void GraphicsPipeline::CreateVertexShaderFromMemory(
	ID3D11Device* device,
	BYTE* pShader, size_t size,
	D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
	)
{
	m_pVertexShader = dx11::d3::shader::detail::CreateVertexShader(device, pShader, size);

	m_pLayout = dx11::d3::shader::detail::CreateInputLayout(device, pLayoutDesc, NumElements, pShader, size);

}

// �W�I���g���V�F�[�_�[���t�@�C������쐬����
void GraphicsPipeline::CreateGeometryShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR  pFunctionName[])
{
	auto gs = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, GEOMETORY_SHADER_PROFILE);

	m_pGeometryShader = dx11::d3::shader::CreateGeometryShader(device, gs.get());
}

// �W�I���g���V�F�[�_�[������������쐬����
void GraphicsPipeline::CreateGeometryShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pGeometryShader = dx11::d3::shader::detail::CreateGeometryShader(device, pShader, size);

}

// �s�N�Z���V�F�[�_�[���t�@�C������쐬����
void GraphicsPipeline::CreatePixelShaderFromFile(ID3D11Device* device,
	TCHAR pSrcFile[],
	CHAR  pFunctionName[]
	)
{
	auto ps = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, PIXEL_SHADER_PROFILE);

	m_pPixelShader = dx11::d3::shader::CreatePixelShader(device, ps.get());
}

// �s�N�Z���V�F�[�_�[������������쐬����
void GraphicsPipeline::CreatePixelShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pPixelShader = dx11::d3::shader::detail::CreatePixelShader(device, pShader, size);
}


// �n���V�F�[�_�[���t�@�C������쐬����
void GraphicsPipeline::CreateHullShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[])
{
	auto hs = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, "hs_5_0");

	m_pHullShader = dx11::d3::shader::CreatePixelShader(device, hs.get());
}

// �n���V�F�[�_�[������������쐬����
void GraphicsPipeline::CreateHullShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pHullShader = dx11::d3::shader::detail::CreateHullShader(device, pShader, size);
}

// �h���C���V�F�[�_�[���t�@�C������쐬����
void GraphicsPipeline::CreateDomainShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[])
{
	auto ds = dx11::d3::shader::CompileShader(pSrcFile, pFunctionName, "ds_5_0");

	m_pDomainShader = dx11::d3::shader::CreateDomainShader(device, ds.get());
}

// �h���C���V�F�[�_�[������������쐬����
void GraphicsPipeline::CreateDomainShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size)
{
	m_pDomainShader = dx11::d3::shader::detail::CreateDomainShader(device, pShader, size);
}


// ���X�^���C�U�X�e�[�g���쐬
void GraphicsPipeline::CreateRasterizerState(ID3D11Device* pD3D11Device, D3D11_CULL_MODE CullMode)
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC RasterizerDesc;

	::ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;    // �|���S���ʕ`��
	RasterizerDesc.CullMode = CullMode;            // �w��̕����������Ă���O�p�`���J�����O����
	RasterizerDesc.FrontCounterClockwise = TRUE;   // �����v����\��
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.ScissorEnable = FALSE;          // �V�U�[��`����
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	SAFE_RELEASE(m_pRasterizerState);

	if (FAILED(hr = pD3D11Device->CreateRasterizerState(&RasterizerDesc, &m_pRasterizerState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateRasterizerState()�ŃG���[���������܂����BID3D11RasterizerState�C���^�[�t�F�[�X�̍쐬�G���[�ł��B")));
}

// �[�x�X�e���V���X�e�[�g���쐬
void GraphicsPipeline::CreateDepthStencilState(ID3D11Device* device, BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled, D3D11_COMPARISON_FUNC DepthFunc)
{
	HRESULT hr;

	D3D11_DEPTH_STENCIL_DESC DSDesc;

	DSDesc.DepthEnable = DepthEnable;           // �[�x�e�X�g���s����
	DSDesc.DepthWriteMask = DepthWriteEnabled;  // �[�x�o�b�t�@�ւ̏������݂��s����
	DSDesc.DepthFunc = DepthFunc;
	DSDesc.StencilEnable = FALSE;

	SAFE_RELEASE(m_pDepthStencilState);

	if (FAILED(hr = device->CreateDepthStencilState(&DSDesc, &m_pDepthStencilState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateDepthStencilState()�ŃG���[���������܂����BID3D11DepthStencilState�C���^�[�t�F�[�X�̍쐬�G���[�ł��B")));
}

// �u�����h�X�e�[�g���쐬
void GraphicsPipeline::CreateBlendState(ID3D11Device* device, UEBLEND_STATE* BlendStateType, UINT BlendStateTypeLength, BOOL AlphaToCoverageEnable)
{
	HRESULT hr;
	D3D11_BLEND_DESC BlendDesc;

	if (BlendStateTypeLength == 0 || BlendStateTypeLength >= 8)
		throw(Exception(-1, _T("GraphicsPipeline::CreateBlendState()�ŃG���[���������܂����B���������s���ł��B")));

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

			// ���Z����
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

			// ���`����
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

			// �[�x�e�X�g
		case UEBLEND_STATE::DEPTH_TEST:
			BlendDesc.RenderTarget[i].BlendEnable = TRUE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_MIN;      // �őO�ʂ̐[�x�l�ŏ㏑�����邽�߂ɍŏ��l��I��
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_MIN;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;
		}
	}

	SAFE_RELEASE(m_pBlendState);

	if (FAILED(hr = device->CreateBlendState(&BlendDesc, &m_pBlendState)))
		throw(Exception(hr, _T("GraphicsPipeline::CreateBlendState()�ŃG���[���������܂����BID3D11BlendState�C���^�[�t�F�[�X�̍쐬�G���[�ł��B")));
}

// �萔�o�b�t�@���쐬����
ID3D11Buffer* GraphicsPipeline::CreateConstantBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag)
{
	HRESULT hr = E_FAIL;

	ID3D11Buffer* pBuffer = nullptr;

	// �o�b�t�@�[ ���\�[�X�B
	D3D11_BUFFER_DESC BufferDesc = { 0 };

	// �T�u���\�[�X
	D3D11_SUBRESOURCE_DATA* resource = nullptr;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = 0;

	__try
	{
		switch (CPUAccessFlag)
		{
			// CPU�A�N�Z�X�������Ȃ�
		case 0:
			Usage = D3D11_USAGE_DEFAULT;
			CPUAccessFlags = CPUAccessFlag;
			break;
			// CPU�A�N�Z�X��������
		default:
			Usage = D3D11_USAGE_DYNAMIC;
			CPUAccessFlags = CPUAccessFlag;
			break;
		}

		// �����l��ݒ肷��
		if (pData)
		{
			resource = new D3D11_SUBRESOURCE_DATA();
			resource->pSysMem = pData;
			resource->SysMemPitch = 0;
			resource->SysMemSlicePitch = 0;
		}

		// �o�b�t�@�̐ݒ�
		BufferDesc.ByteWidth = size;                       // �o�b�t�@�T�C�Y
		BufferDesc.Usage = Usage;                      // ���\�[�X�g�p�@����肷��
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �o�b�t�@�̎��
		BufferDesc.CPUAccessFlags = CPUAccessFlags;             // CPU �A�N�Z�X
		BufferDesc.MiscFlags = 0;                          // ���̑��̃t���O���ݒ肵�Ȃ�

		// �o�b�t�@���쐬����
		hr = device->CreateBuffer(&BufferDesc, resource, &pBuffer);
		if (FAILED(hr))
			throw(Exception(hr, _T("GraphicsPipeline::CreateConstantBuffer()�ŃG���[���������܂����B�o�b�t�@�쐬�G���[�ł��B")));
	}
	__finally
	{
		SAFE_DELETE(resource);
	}

	return pBuffer;
}