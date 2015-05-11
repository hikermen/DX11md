#ifndef PLANE_MESH_H
#define PLANE_MESH_H

#include "Mesh.h"

#include <DirectXMath.h>



class PlaneMesh : public IMesh
{
private:

	std::shared_ptr<ID3D11Buffer> m_pVertexBuffer;

	std::shared_ptr<ID3D11Buffer> m_pIndexBuffer;

	std::shared_ptr<ID3D11ShaderResourceView> m_pSRView;

	std::shared_ptr<ID3D11SamplerState> m_pSamplerState;

	DirectX::XMMATRIX m_matWorld;

	ULONG m_VertexCount;

public:
	PlaneMesh();
	virtual ~PlaneMesh();

	void Invalidate();
	void CreateMesh(ID3D11Device* device);
	void Update();
	inline DirectX::XMMATRIX* GetMatWorld(){ return &m_matWorld; }
	void Render(ID3D11DeviceContext* context);
	
};

class TestMesh : public IMesh
{
private:

	struct TVERTEX
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texcoord;
	};

	std::shared_ptr<ID3D11Buffer> m_pVertexBuffer;

	std::shared_ptr<ID3D11Buffer> m_pIndexBuffer;

	std::shared_ptr<ID3D11ShaderResourceView> m_pSRView;

	std::shared_ptr<ID3D11SamplerState> m_pSamplerState;

	DirectX::XMMATRIX m_matWorld;

	ULONG m_VertexCount;

	DirectX::XMFLOAT2 m_tessellation;

public:
	TestMesh();
	virtual ~TestMesh();

	void Invalidate();
	void CreateMesh(ID3D11Device* device);
	void Update();
	inline DirectX::XMMATRIX* GetMatWorld(){ return &m_matWorld; }
	inline DirectX::XMFLOAT2* GetTessArg() { return &m_tessellation; }
	void Render(ID3D11DeviceContext* context);

};

#endif