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
	// �萔��`
	typedef struct _CBUFFER0
	{
		DirectX::XMMATRIX  matWVP; // ���[���h �~ �r���[ �~ �ˉe �s��
	}CBUFFER0;

	typedef struct _CBUFFER1
	{
		float TessFactor;       // �|���S���̃G�b�W�̃e�b�Z���[�V�����W��
		float InsideTessFactor; // �|���S�������̃e�b�Z���[�V�����W��
		float Dummy2;
		float Dummy3;
	}CBUFFER1;

	//�V�F�[�_�[�p�萔�o�b�t�@
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
	void SetConstantBuffers(ID3D11DeviceContext* context, DirectX::XMMATRIX* pMatWorld, float TessFactor, float InsideTess);
	void EndPass(ID3D11DeviceContext* context);

};

#endif