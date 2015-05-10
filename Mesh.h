#ifndef MESH_H
#define MESH_H

#include "Common.h"
#include "Direct3D11.h"
#include "Exception.h"

#include <DirectXMath.h>


class IMesh
{
public:
	typedef struct _VERTEX
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Texcoord;
	}VERTEX;

protected:
	//サンプラーステートを作成する
	//↓フィルタリングオプション
	//https://msdn.microsoft.com/ja-jp/library/ee416129%28v=vs.85%29.aspx
	std::shared_ptr<ID3D11SamplerState> CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

public:
	virtual ~IMesh(){};
	virtual void Render(ID3D11DeviceContext* context) = 0;
};

#endif