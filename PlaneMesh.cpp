#include "PlaneMesh.h"

#include "DX11ThinWrapper.h"
#include "DXGI_FORMAT.h"
#include "DirectXTexWrapper.h"

PlaneMesh::PlaneMesh():
	m_VertexCount(0)
{
}

PlaneMesh::~PlaneMesh()
{
	Invalidate();
}

void PlaneMesh::Invalidate()
{
}

void PlaneMesh::CreateMesh(ID3D11Device* device)
{
	IMesh::VERTEX vertex[4];

	vertex[0].Position = DirectX::XMFLOAT3(100.0f, -2.0f, 100.0f);
	vertex[0].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = DirectX::XMFLOAT3(-100.0f, -2.0f, 100.0f);
	vertex[1].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = DirectX::XMFLOAT3(100.0f, -2.0f, -100.0f);
	vertex[2].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = DirectX::XMFLOAT3(-100.0f, -2.0f, -100.0f);
	vertex[3].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	m_VertexCount = _countof(vertex);

	m_pVertexBuffer = dx11::d3::buffer::CreateVertexBuffer(device, vertex, sizeof(IMesh::VERTEX)*m_VertexCount, 0);

	// �C���f�b�N�X�o�b�t�@��ݒ�
	INDEX_FORMAT_TYPE Index[] = { 0, 1, 2, 3 };
	// �C���f�b�N�X�o�b�t�@���쐬����B
	m_pIndexBuffer = dx11::d3::buffer::CreateIndexBuffer(device, Index, sizeof(UINT) * m_VertexCount, 0);

	//�T���v���[�X�e�[�g�̍쐬
	m_pSamplerState = CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP);

	m_pSRView = dxtex::CreateTexture(device, "Resources/icon.png");

	
	m_matWorld = DirectX::XMMatrixTranslation(0.0f, -20.0f, 0.0f);

}

void PlaneMesh::Update()
{
	
	if (GetKeyState('R') & 0x8000)
		m_matWorld *= DirectX::XMMatrixRotationY(-0.002f);
	if (GetKeyState('T') & 0x8000)
		m_matWorld *= DirectX::XMMatrixRotationY(0.002f);

}

void PlaneMesh::Render(ID3D11DeviceContext* context)
{

	auto vb = m_pVertexBuffer.get();
	auto ib = m_pIndexBuffer.get();
	auto srv = m_pSRView.get();
	auto sampler = m_pSamplerState.get();

	UINT stride = sizeof(IMesh::VERTEX);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	context->IASetIndexBuffer(ib, INDEX_FORMAT_ENUM, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	context->PSSetSamplers(0, 1, &sampler);
	context->PSSetShaderResources(0, 1, &srv);

	context->DrawIndexed(m_VertexCount, 0, 0);
}


TestMesh::TestMesh() :
m_VertexCount(0)
{
}

TestMesh::~TestMesh()
{
	Invalidate();
}

void TestMesh::Invalidate()
{
}

void TestMesh::CreateMesh(ID3D11Device* device)
{
	TVERTEX vertex[4];

	vertex[0].Position = DirectX::XMFLOAT3(50.0f, 50.0f, 0.0f);
	vertex[0].Texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = DirectX::XMFLOAT3(-50.0f, 50.0f, 0.0f);
	vertex[1].Texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = DirectX::XMFLOAT3(50.0f, -50.0f, 0.0f);
	vertex[2].Texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = DirectX::XMFLOAT3(-50.0f, -50.0f, 0.0f);
	vertex[3].Texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	m_VertexCount = _countof(vertex);

	m_pVertexBuffer = dx11::d3::buffer::CreateVertexBuffer(device, vertex, sizeof(TVERTEX)*m_VertexCount, 0);

	// �C���f�b�N�X�o�b�t�@��ݒ�
	INDEX_FORMAT_TYPE Index[] = { 0, 1, 2, 3 };
	// �C���f�b�N�X�o�b�t�@���쐬����B
	m_pIndexBuffer = dx11::d3::buffer::CreateIndexBuffer(device, Index, sizeof(UINT) * m_VertexCount, 0);

	//�T���v���[�X�e�[�g�̍쐬
	m_pSamplerState = CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP);

	m_pSRView = dxtex::CreateTexture(device, "Resources/icon.png");


	m_matWorld = DirectX::XMMatrixTranslation(0.0f, -20.0f, 100.0f);

	m_tessellation = DirectX::XMFLOAT2(1.0f, 1.0f);

}

void TestMesh::Update()
{

	if (GetKeyState('Q') & 0x8000)
		m_matWorld *= DirectX::XMMatrixRotationY(-0.001f);
	if (GetKeyState('A') & 0x8000)
		m_matWorld *= DirectX::XMMatrixRotationY(0.001f);
	if (GetKeyState('Z') & 0x8000)
		m_tessellation = DirectX::XMFLOAT2(1.0f, 1.0f);
	if (GetKeyState('X') & 0x8000)
		m_tessellation = DirectX::XMFLOAT2(1.0f, 3.0f);

}

void TestMesh::Render(ID3D11DeviceContext* context)
{

	auto vb = m_pVertexBuffer.get();
	auto ib = m_pIndexBuffer.get();
	auto srv = m_pSRView.get();
	auto sampler = m_pSamplerState.get();

	UINT stride = sizeof(TVERTEX);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	context->IASetIndexBuffer(ib, INDEX_FORMAT_ENUM, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->PSSetSamplers(0, 1, &sampler);
	context->PSSetShaderResources(0, 1, &srv);

	context->DrawIndexed(m_VertexCount, 0, 0);
}