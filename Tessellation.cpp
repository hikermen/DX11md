#include "Tessellation.h"


Tessellation::Tessellation()
{
	for (int i = 0; i < _countof(m_pConstantBuffers); i++)
	{
		m_pConstantBuffers[i] = nullptr;
	}
	m_pGraphicsPipeline = nullptr;

}

Tessellation::~Tessellation()
{
	for (int i = 0; i < _countof(m_pConstantBuffers); i++)
	{
		SAFE_RELEASE(m_pConstantBuffers[i]);
	}

	SAFE_DELETE(m_pGraphicsPipeline);
}

void Tessellation::Create(ID3D11Device* device)
{

	m_pGraphicsPipeline = NEW GraphicsPipeline();

	//���X�^���C�U�X�e�[�g���쐬����
	m_pGraphicsPipeline->CreateRasterizerState(device, D3D11_CULL_MODE::D3D11_CULL_BACK);
	//�u�����h�X�e�[�g���쐬����
	GraphicsPipeline::UEBLEND_STATE BlendStateType[1] = { GraphicsPipeline::UEBLEND_STATE::NONE };
	m_pGraphicsPipeline->CreateBlendState(device, BlendStateType, 1);


	// ���_�V�F�[�_�[���쐬����
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_pGraphicsPipeline->CreateVertexShaderFromFile(device, _T("Resources/Tessellation.hlsl"), "VS_Main", layout, _countof(layout));
	
	// �n���V�F�[�_�[���쐬����
	m_pGraphicsPipeline->CreateHullShaderFromFile(device, _T("Resources/Tessellation.hlsl"), "HS_Main");
	// �h���C���V�F�[�_�[���쐬����
	m_pGraphicsPipeline->CreateDomainShaderFromFile(device, _T("Resources/Tessellation.hlsl"), "DS_Main");
	// �s�N�Z���V�F�[�_�[���쐬����
	m_pGraphicsPipeline->CreatePixelShaderFromFile(device, _T("Resources/Tessellation.hlsl"), "PS_Main");


	// �萔�o�b�t�@���쐬����B
	m_pConstantBuffers[0] = m_pGraphicsPipeline->CreateConstantBuffer(device, nullptr, sizeof(Tessellation::CBUFFER0), D3D11_CPU_ACCESS_WRITE);

	m_pConstantBuffers[1] = m_pGraphicsPipeline->CreateConstantBuffer(device, nullptr, sizeof(Tessellation::CBUFFER1), D3D11_CPU_ACCESS_WRITE);

}

void Tessellation::BeginPass(ID3D11DeviceContext* context,
	DirectX::XMMATRIX* pMatView, DirectX::XMMATRIX* pMatProj, DirectX::XMMATRIX* pMatOrtho
	)
{
	m_MatView = *pMatView;
	m_MatProj = *pMatProj;
	m_MatOrtho = *pMatOrtho;


	//�e��X�e�[�g��ݒ肷��
	m_pGraphicsPipeline->SetRasterizerState(context);
	m_pGraphicsPipeline->SetDepthStencilState(context);
	m_pGraphicsPipeline->SetBlendState(context);

	//�e��O���t�B�b�N�p�C�v���C����ݒ�
	m_pGraphicsPipeline->SetVertexShader(context);
	m_pGraphicsPipeline->SetHullShader(context);
	m_pGraphicsPipeline->SetDomainShader(context);
	m_pGraphicsPipeline->SetGeometryShader(context);
	m_pGraphicsPipeline->SetPixelShader(context);
}

void Tessellation::CreateMatWVP(DirectX::XMMATRIX* pMatWorld, DirectX::XMMATRIX* pRetMatCWVP)
{
	*pRetMatCWVP = (*pMatWorld) * m_MatView * m_MatProj;
	*pRetMatCWVP = DirectX::XMMatrixTranspose(*pRetMatCWVP);
}

void Tessellation::SetConstantBuffers(ID3D11DeviceContext* context, DirectX::XMMATRIX* pMatWorld, float TessFactor, float InsideTess)
{
	HRESULT hr = E_FAIL;

	DirectX::XMMATRIX matWVP;

	CreateMatWVP(pMatWorld, &matWVP);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(hr = context->Map(m_pConstantBuffers[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		throw(Exception(-1, _T("ShadowMap::SetConstantBuffers()�ŃG���[���������܂����BMap�����s���܂����B")));

	Tessellation::CBUFFER0* cbuffer1 = (Tessellation::CBUFFER0*)mappedResource.pData;

	::CopyMemory(&cbuffer1->matWVP, &matWVP, sizeof(DirectX::XMMATRIX));

	context->Unmap(m_pConstantBuffers[0], 0);


	if (FAILED(hr = context->Map(m_pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		throw(Exception(-1, _T("ShadowMap::SetConstantBuffers()�ŃG���[���������܂����BMap�����s���܂����B")));

	Tessellation::CBUFFER1* cbuffer2 = (Tessellation::CBUFFER1*)mappedResource.pData;

	cbuffer2->TessFactor = TessFactor;
	cbuffer2->InsideTessFactor = InsideTess;

	context->Unmap(m_pConstantBuffers[1], 0);

	context->HSSetConstantBuffers(1, 1, &m_pConstantBuffers[1]);

	context->DSSetConstantBuffers(0, 1, &m_pConstantBuffers[0]);

}

void Tessellation::EndPass(ID3D11DeviceContext* context)
{
	ID3D11ShaderResourceView* pNull[1] = { nullptr };
	context->PSSetShaderResources(1, _countof(pNull), pNull);
}
