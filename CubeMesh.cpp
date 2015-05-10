#include "CubeMesh.h"

#include "FBXLoader.h"
#include "DX11ThinWrapper.h"
#include "DXGI_FORMAT.h"

#include "DirectXTexWrapper.h"

CubeMesh::CubeMesh() :
m_VertexCount(0)
{
}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::CreateMesh(ID3D11Device* device)
{
	auto loader = std::make_shared<FBXLoader>();

	loader->Initialize("Resources/unitychan.fbx");

	auto numMesh = loader->GetMeshCount();

	m_pVertexBuffer.reserve(numMesh);
	m_pIndexBuffer.reserve(numMesh);
	m_VertexCount.reserve(numMesh);
	m_MeshMatrialList.reserve(numMesh);

	
	for (int i = 0; i < numMesh; ++i)
	{
		auto mesh = loader->GetMesh(i);
		auto numVertex = mesh.vertexList.size();
		auto numIndex = mesh.indexList.size();

		m_pVertexBuffer.push_back(dx11::d3::buffer::CreateVertexBuffer(device, mesh.vertexList.data(), sizeof(ModelVertex)  * numVertex, 0));
		
		m_pIndexBuffer.push_back(dx11::d3::buffer::CreateIndexBuffer(device, mesh.indexList.data(), sizeof(INDEX_FORMAT_TYPE) * numIndex, 0));

		m_VertexCount.push_back(numIndex);

		m_MeshMatrialList.push_back(mesh.materialName);
	}

	//サンプラーステートの作成
	m_pSamplerState = CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP);

	auto numMaterial = loader->GetMaterialCount();
	std::string path = "Resources/Textures/";

	for (int i = 0; i < numMaterial; ++i)
	{
		auto material = loader->GetMaterial(i);
		auto filename = material.diffuseTextureName;

		m_pSRViews[material.materialName] = dxtex::CreateTexture(device, path + filename);
	}
	

	loader->Finalize();
}

void CubeMesh::Update()
{
	DirectX::XMMATRIX matTranslate;

	matTranslate = DirectX::XMMatrixTranslation(0.0f, 100.0f, 0.0f);
	m_matWorld = matTranslate;
}

void CubeMesh::Render(ID3D11DeviceContext* context)
{
	for (int i = 0; i < (int)m_pVertexBuffer.size(); ++i)
	{

		auto vb = m_pVertexBuffer[i].get();
		auto ib = m_pIndexBuffer[i].get();
		auto srv = m_pSRViews[m_MeshMatrialList[i]].get();
		auto sampler = m_pSamplerState.get();

		UINT stride = sizeof(ModelVertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		context->IASetIndexBuffer(ib, INDEX_FORMAT_ENUM, 0);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->PSSetSamplers(0, 1, &sampler);
		context->PSSetShaderResources(0, 1, &srv);

		context->DrawIndexed(m_VertexCount[i], 0, 0);
	}
}