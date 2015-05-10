#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	m_pDSView = nullptr;
	m_pSRView = nullptr;
	for (int i = 0; i < _countof(m_pGraphicsPipeline); i++)
	{
		m_pGraphicsPipeline[i] = nullptr;
		m_pConstantBuffers[i] = nullptr;
	}
}

ShadowMap::~ShadowMap()
{
	for (int i = 0; i < _countof(m_pGraphicsPipeline); i++)
	{
		SAFE_DELETE(m_pGraphicsPipeline[i]);
		SAFE_RELEASE(m_pConstantBuffers[i]);
	}

	SAFE_RELEASE(m_pSRView);
	SAFE_RELEASE(m_pDSView);
}

void ShadowMap::Create(ID3D11Device* device, DWORD Width, DWORD Height)
{
	m_Width = Width;
	m_Height = Height;

	//---------------------------------------------------------------------------
	//�p�X0�p�̃p�C�v���C���̐ݒ�
	//---------------------------------------------------------------------------

	m_pGraphicsPipeline[0] = NEW GraphicsPipeline();

	//���X�^���C�U�X�e�[�g���쐬����
	m_pGraphicsPipeline[0]->CreateRasterizerState(device, D3D11_CULL_MODE::D3D11_CULL_BACK);
	//�[�x�X�e���V���X�e�[�g���쐬����
	m_pGraphicsPipeline[0]->CreateDepthStencilState(device, TRUE, D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL);
	//�u�����h�X�e�[�g���쐬����
	GraphicsPipeline::UEBLEND_STATE BlendStateType[1] = { GraphicsPipeline::UEBLEND_STATE::NONE };
	m_pGraphicsPipeline[0]->CreateBlendState(device, BlendStateType, 1);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//�V�F�[�_���쐬����
#if defined(DEBUG) || defined(_DEBUG)
	m_pGraphicsPipeline[0]->CreateVertexShaderFromFile(device, _T("Resources/ShadowMap_Pass0.hlsl"), "ShadowMap_Pass0_VS_Main", layout, _countof(layout));
#else
	m_pGraphicsPipeline[0]->CreateVertexShaderMemory(device, (LPBYTE)g_pShadowMap_Pass0_VS_Main, sizeof(g_ShadowMap_Pass0_VS_Main), layout, _countof(layout));
#endif

	maps::CreateDepthStencilView(device, m_Width, m_Height, &m_pDSView, &m_pSRView);

	m_pConstantBuffers[0] = m_pGraphicsPipeline[0]->CreateConstantBuffer(device, nullptr, sizeof(ShadowMap::CBUFFER_PASS0), D3D11_CPU_ACCESS_WRITE);


	//---------------------------------------------------------------------------
	//�p�X1�p�̃p�C�v���C���̐ݒ�
	//�V�F�[�_�[�ƒ萔�o�b�t�@�ȊO�͎g���܂킷
	//---------------------------------------------------------------------------

	m_pGraphicsPipeline[1] = NEW GraphicsPipeline();

#if defined(DEBUG) || defined(_DEBUG)
	m_pGraphicsPipeline[1]->CreateVertexShaderFromFile(device, _T("Resources/ShadowMap_Pass1.hlsl"), "ShadowMap_Pass1_VS_Main", layout, _countof(layout));
	m_pGraphicsPipeline[1]->CreatePixelShaderFromFile(device, _T("Resources/ShadowMap_Pass1.hlsl"), "ShadowMap_Pass1_PS_Main");
#else
	m_pGraphicsPipeline[1]->CreateVertexShaderMemory(device, (LPBYTE)g_pShadowMap_Pass1_VS_Main, sizeof(g_ShadowMap_Pass1_VS_Main), layout, _countof(layout));
	m_pGraphicsPipeline[1]->CreatePixelShaderMemory(device, (LPBYTE)g_pShadowMap_Pass1_PS_Main, sizeof(g_ShadowMap_Pass1_PS_Main));
#endif

	m_pConstantBuffers[1] = m_pGraphicsPipeline[1]->CreateConstantBuffer(device, nullptr, sizeof(ShadowMap::CBUFFER_PASS1), D3D10_CPU_ACCESS_WRITE);

}

void ShadowMap::BeginPass(ID3D11DeviceContext* context,
	DirectX::XMMATRIX* pMatView, DirectX::XMMATRIX* pMatProj, DirectX::XMMATRIX* pMatOrtho, DirectX::XMFLOAT4* pVecLightPos, UINT Pass
	)
{
	switch (Pass)
	{
	case 0:
	{
		context->OMGetRenderTargets(1, &m_pOldRTView, &m_pOldDSView);
		context->OMSetRenderTargets(0, nullptr, m_pDSView);

		context->RSGetViewports(&m_ViewportCount, nullptr);
		context->RSGetViewports(&m_ViewportCount, &m_pOldViewport[0]);

		D3D11_VIEWPORT Viewport[1];
		Viewport[0].TopLeftX = 0;
		Viewport[0].TopLeftY = 0;
		Viewport[0].Width = (FLOAT)m_Width;
		Viewport[0].Height = (FLOAT)m_Height;
		Viewport[0].MinDepth = 0.0f;
		Viewport[0].MaxDepth = 1.0f;
		context->RSSetViewports(1, Viewport);

		//�����_�[�^�[�Q�b�g�͎g�p���Ȃ��̂ŃN���A����K�v�Ȃ�

		//�[�x�o�b�t�@���N���A����
		context->ClearDepthStencilView(m_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_MatView = *pMatView;
		m_MatProj = *pMatProj;
		m_MatOrtho = *pMatOrtho;
		m_VecLightPos = *pVecLightPos;
	}
	break;
	case 1:
	{
		//�[�x�}�b�v���V�F�[�_�ɐݒ�
		context->PSSetShaderResources(1, 1, &m_pSRView);
	}
	break;
	}

	//�e��X�e�[�g��ݒ肷��
	m_pGraphicsPipeline[0]->SetRasterizerState(context);
	m_pGraphicsPipeline[0]->SetDepthStencilState(context);
	m_pGraphicsPipeline[0]->SetBlendState(context);

	//�e��O���t�B�b�N�p�C�v���C����ݒ�
	m_pGraphicsPipeline[Pass]->SetVertexShader(context);
	m_pGraphicsPipeline[Pass]->SetHullShader(context);
	m_pGraphicsPipeline[Pass]->SetDomainShader(context);
	m_pGraphicsPipeline[Pass]->SetGeometryShader(context);
	m_pGraphicsPipeline[Pass]->SetPixelShader(context);
}

void ShadowMap::CreateMatWVP(DirectX::XMMATRIX* pMatWorld, DirectX::XMMATRIX* pRetMatCWVP, DirectX::XMMATRIX* pRetMatLWVP)
{
	// ���C�g�r���[���쐬
	DirectX::XMMATRIX matLightView = 
		DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&m_VecLightPos),
		DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 0, 0)),
		DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 1, 0))
		);

	*pRetMatLWVP = (*pMatWorld) * matLightView * m_MatOrtho;
	*pRetMatLWVP = DirectX::XMMatrixTranspose(*pRetMatLWVP);

	*pRetMatCWVP = (*pMatWorld) * m_MatView * m_MatProj;
	*pRetMatCWVP = DirectX::XMMatrixTranspose(*pRetMatCWVP);
}

void ShadowMap::SetConstantBuffers(ID3D11DeviceContext* context, DirectX::XMMATRIX* pMatWorld, UINT Pass)
{
	HRESULT hr = E_FAIL;

	switch (Pass)
	{
	case 0:
	{
		DirectX::XMMATRIX matCWVP, matLWVP;

		CreateMatWVP(pMatWorld, &matCWVP, &matLWVP);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(hr = context->Map(m_pConstantBuffers[Pass], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			throw(Exception(-1, _T("ShadowMap::SetConstantBuffers()�ŃG���[���������܂����BMap�����s���܂����B")));

		ShadowMap::CBUFFER_PASS0* cbuffer = (ShadowMap::CBUFFER_PASS0*)mappedResource.pData;

		::CopyMemory(&cbuffer->matLWVP, &matLWVP, sizeof(DirectX::XMMATRIX));

		context->Unmap(m_pConstantBuffers[Pass], 0);

		context->VSSetConstantBuffers(0, 1, &m_pConstantBuffers[Pass]);
		context->PSSetConstantBuffers(0, 1, &m_pConstantBuffers[Pass]);

	}
	break;
	case 1:
	{
		DirectX::XMMATRIX matCWVP, matLWVP;

		CreateMatWVP(pMatWorld, &matCWVP, &matLWVP);

		//���b�V����̃��[�J�����W�n��ł̕��s�����̕����x�N�g�����v�Z����

		DirectX::XMVECTOR Determinant;
		DirectX::XMMATRIX matInv = XMMatrixInverse(&Determinant, *pMatWorld);
		DirectX::XMVECTOR v = XMVector4Transform(XMLoadFloat4(&m_VecLightPos), matInv);
		DirectX::XMVECTOR nv = XMVector3Normalize(v);
		DirectX::XMFLOAT4 localLightPos;
		DirectX::XMStoreFloat4(&localLightPos, nv);

		DirectX::XMFLOAT4 shadowMapSize = DirectX::XMFLOAT4((float)m_Width, (float)m_Height, 0, 0);

		//�萔�o�b�t�@�̍쐬
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(hr = context->Map(m_pConstantBuffers[Pass], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			throw(Exception(hr, _T("ShadowMap::SetConstantBuffers()�ŃG���[���������܂����BMap()�����s���܂����B")));

		ShadowMap::CBUFFER_PASS1* cbuffer = (ShadowMap::CBUFFER_PASS1*)mappedResource.pData;


		::CopyMemory(&cbuffer->matLWVP, &matLWVP, sizeof(DirectX::XMMATRIX));
		::CopyMemory(&cbuffer->matCWVP, &matCWVP, sizeof(DirectX::XMMATRIX));
		::CopyMemory(&cbuffer->vecLightDir, &localLightPos, sizeof(DirectX::XMFLOAT4));
		::CopyMemory(&cbuffer->vecShadowMapSize, &shadowMapSize, sizeof(DirectX::XMFLOAT4));

		context->Unmap(m_pConstantBuffers[Pass], 0);

		context->VSSetConstantBuffers(0, 1, &m_pConstantBuffers[Pass]);
		context->PSSetConstantBuffers(0, 1, &m_pConstantBuffers[Pass]);

	}
	break;
	}
}

void ShadowMap::EndPass(ID3D11DeviceContext* context, UINT Pass)
{
	switch (Pass)
	{
	case 0:
		context->OMSetRenderTargets(1, &m_pOldRTView, m_pOldDSView);
		SAFE_RELEASE(m_pOldRTView);
		SAFE_RELEASE(m_pOldDSView);

		context->RSSetViewports(m_ViewportCount, m_pOldViewport);
		break;

	case 1:
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		context->PSSetShaderResources(1, _countof(pNull), pNull);
		break;
	}
}
