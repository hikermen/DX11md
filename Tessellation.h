#ifndef TESSELLATION_H
#define TESSELLATION_H

#include "Common.h"
#include "Exception.h"
#include "GraphicsPipeline.h"
#include "Direct3D11.h"

#include <DirectXMath.h>

class Tessellation
{
private:
	// 定数定義
	typedef struct _CBUFFER0
	{
		DirectX::XMMATRIX  matWVP; // ワールド × ビュー × 射影 行列
	}CBUFFER0;

	typedef struct _CBUFFER1
	{
		float TessFactor;       // ポリゴンのエッジのテッセレーション係数
		float InsideTessFactor; // ポリゴン内部のテッセレーション係数
		float Dummy2;
		float Dummy3;
	}CBUFFER1;

	//シェーダー用定数バッファ
	ID3D11Buffer* m_pConstantBuffers[2];

	GraphicsPipeline* m_pGraphicsPipeline;

	DirectX::XMMATRIX m_MatView;
	DirectX::XMMATRIX m_MatProj;
	DirectX::XMMATRIX m_MatOrtho;

	void CreateMatWVP(DirectX::XMMATRIX* pMatWorld, DirectX::XMMATRIX* pRetMatCWVP);

public:
	Tessellation();
	virtual ~Tessellation();

	void Invalidate();
	void Create(ID3D11Device* device);
	void BeginPass(ID3D11DeviceContext* context,
		DirectX::XMMATRIX* pMatView, DirectX::XMMATRIX* pMatProj, DirectX::XMMATRIX* pMatOrtho);
	void SetConstantBuffers(ID3D11DeviceContext* context, DirectX::XMMATRIX* pMatWorld);
	void EndPass(ID3D11DeviceContext* context);

};

#endif