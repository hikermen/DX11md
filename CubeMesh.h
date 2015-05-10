#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"

#include <vector>
#include <map>
#include <DirectXMath.h>



class CubeMesh : public IMesh
{
private:

	std::vector<std::shared_ptr<ID3D11Buffer>> m_pVertexBuffer;

	std::vector<std::shared_ptr<ID3D11Buffer>> m_pIndexBuffer;

	std::vector<std::string> m_MeshMatrialList;

	std::shared_ptr<ID3D11SamplerState> m_pSamplerState;

	DirectX::XMMATRIX m_matWorld;

	std::vector<ULONG> m_VertexCount;

	std::map<std::string, std::shared_ptr<ID3D11ShaderResourceView>> m_pSRViews;

public:
	CubeMesh();
	virtual ~CubeMesh();

	void Invalidate();
	void CreateMesh(ID3D11Device* device);
	void Update();
	inline DirectX::XMMATRIX* GetMatWorld(){ return &m_matWorld; }
	void Render(ID3D11DeviceContext* context);

};


#endif