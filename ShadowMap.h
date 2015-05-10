#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "Common.h"
#include "Exception.h"
#include "GraphicsPipeline.h"

#include <DirectXMath.h>

class ShadowMap
{
private:
	typedef struct _CBUFFER_PASS0
	{
		DirectX::XMMATRIX matLWVP;
	}CBUFFER_PASS0;

	typedef struct _CBUFFER_PASS1
	{
		DirectX::XMMATRIX matCWVP;
		DirectX::XMMATRIX matLWVP;
		DirectX::XMFLOAT4 vecLightDir;
		DirectX::XMFLOAT4 vecShadowMapSize;
	}CBUFFER_PASS1;

	ID3D11DepthStencilView* m_pDSView;
	ID3D11ShaderResourceView* m_pSRView;

	ID3D11RenderTargetView* m_pOldRTView;
	ID3D11DepthStencilView* m_pOldDSView;
	D3D11_VIEWPORT m_pOldViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT m_ViewportCount;

	//シェーダー用定数バッファ
	ID3D11Buffer* m_pConstantBuffers[2];

	GraphicsPipeline* m_pGraphicsPipeline[2];

	DWORD m_Width, m_Height;
	DirectX::XMMATRIX m_MatView;
	DirectX::XMMATRIX m_MatProj;
	DirectX::XMMATRIX m_MatOrtho;
	DirectX::XMFLOAT4 m_VecLightPos;

	void CreateMatWVP(DirectX::XMMATRIX* pMatWorld, DirectX::XMMATRIX* pRetMatCWVP, DirectX::XMMATRIX* pRetMatLWVP);

public:
	ShadowMap();
	virtual ~ShadowMap();
	
	void Invalidate();
	void Create(ID3D11Device* device, DWORD Width, DWORD Height);
	void BeginPass(ID3D11DeviceContext* context,
		DirectX::XMMATRIX* pMatView, DirectX::XMMATRIX* pMatProj, DirectX::XMMATRIX* pMatOrtho, DirectX::XMFLOAT4* pVecLightPos, UINT Pass);
	void SetConstantBuffers(ID3D11DeviceContext* context, DirectX::XMMATRIX* pMatWorld, UINT Pass);
	void EndPass(ID3D11DeviceContext* context, UINT Pass);
	
	inline UINT GetMaxPass(){ return 2; }
	inline const ID3D11ShaderResourceView* GetDepthMap(){ return m_pSRView; }
};


#endif