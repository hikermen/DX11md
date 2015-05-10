#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "Common.h"
#include <d3dcompiler.h>
#include "Exception.h"
#include "Direct3D11.h"

class GraphicsPipeline
{
private:

	std::shared_ptr<ID3D11InputLayout> m_pLayout;

	//���_�V�F�[�_
	std::shared_ptr<ID3D11VertexShader> m_pVertexShader;
	//�n���V�F�[�_
	std::shared_ptr<ID3D11HullShader> m_pHullShader;
	//�h���C���V�F�[�_
	std::shared_ptr<ID3D11DomainShader> m_pDomainShader;
	//�W�I���g���V�F�[�_
	std::shared_ptr<ID3D11GeometryShader> m_pGeometryShader;
	//�s�N�Z���V�F�[�_
	std::shared_ptr<ID3D11PixelShader> m_pPixelShader;
	//���X�^���C�U�X�e�[�g
	ID3D11RasterizerState* m_pRasterizerState;
	//�[�x�X�e���V���X�e�[�g
	ID3D11DepthStencilState* m_pDepthStencilState;
	//�u�����h�X�e�[�g
	ID3D11BlendState* m_pBlendState;


public:

	enum class UEBLEND_STATE
	{
		NONE = 0,
		ADD = 1,
		ALIGNMENT = 2,
		DEPTH_TEST = 3
	};

	enum class UEACCESS_FLAG
	{
		CPU_NONE_GPU_READWRITE = 0,
		UPDATE_SUBRESOURCE = 1,
		CPU_WRITE_GPU_READ = 2
	};

	GraphicsPipeline();
	virtual ~GraphicsPipeline();
	void Invalidate();

	//���_�V�F�[�_�[���t�@�C������쐬����
	void CreateVertexShaderFromFile(
		ID3D11Device* device,
		TCHAR pSrcFile[], CHAR pFunctionName[],
		D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
		);

	//���_�V�F�[�_������������쐬����
	void CreateVertexShaderFromMemory(
		ID3D11Device* device,
		BYTE* pShader, size_t size,
		D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
		);

	//�W�I���g���V�F�[�_���t�@�C������쐬����
	void CreateGeometryShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// �W�I���g���V�F�[�_�[������������쐬����
	void CreateGeometryShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// �s�N�Z���V�F�[�_�[���t�@�C������쐬����
	void CreatePixelShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// �s�N�Z���V�F�[�_�[������������쐬����
	void CreatePixelShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// �n���V�F�[�_�[���t�@�C������쐬����
	void CreateHullShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// �n���V�F�[�_�[������������쐬����
	void CreateHullShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// �h���C���V�F�[�_�[���t�@�C������쐬����
	void CreateDomainShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// �h���C���V�F�[�_�[������������쐬����
	void CreateDomainShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);


	// ���X�^���C�U�X�e�[�g���쐬
	void CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE CullMode);

	// �[�x�X�e���V���X�e�[�g���쐬
	void CreateDepthStencilState(ID3D11Device* device, BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled, D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS);

	// �u�����h�X�e�[�g���쐬
	void CreateBlendState(ID3D11Device* device, UEBLEND_STATE* BlendStateType, UINT BlendStateTypeLength, BOOL AlphaToCoverageEnable = FALSE);

	// �萔�o�b�t�@���쐬����
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag);

	// ���_�V�F�[�_�[���f�o�C�X�ɐݒ肷��
	inline void SetVertexShader(ID3D11DeviceContext* context)
	{
		// ���̓A�Z���u���[ �X�e�[�W��ݒ�
		context->IASetInputLayout(m_pLayout.get());
		// ���_�V�F�[�_�[���f�o�C�X�ɐݒ肷��B
		context->VSSetShader(m_pVertexShader.get(), nullptr, 0);
	}

	// �n���V�F�[�_�[���f�o�C�X�ɐݒ肷��
	inline void SetHullShader(ID3D11DeviceContext* context)
	{
		context->HSSetShader(m_pHullShader.get(), nullptr, 0);
	}

	// �h���C���V�F�[�_�[���f�o�C�X�ɐݒ肷��
	inline void SetDomainShader(ID3D11DeviceContext* context)
	{
		context->DSSetShader(m_pDomainShader.get(), nullptr, 0);
	}

	// �W�I���g���V�F�[�_�[���f�o�C�X�ɐݒ肷��
	inline void SetGeometryShader(ID3D11DeviceContext* context)
	{
		context->GSSetShader(m_pGeometryShader.get(), nullptr, 0);
	}

	// �s�N�Z���V�F�[�_�[���f�o�C�X�ɐݒ肷��
	inline void SetPixelShader(ID3D11DeviceContext* context)
	{
		context->PSSetShader(m_pPixelShader.get(), nullptr, 0);
	}

	// ���X�^���C�U�X�e�[�g���f�o�C�X�ɐݒ肷��
	inline void SetRasterizerState(ID3D11DeviceContext* context)
	{
		context->RSSetState(m_pRasterizerState);
	}

	// �[�x�X�e���V���X�e�[�g���f�o�C�X�ɐݒ肷��
	inline void SetDepthStencilState(ID3D11DeviceContext* context)
	{
		context->OMSetDepthStencilState(m_pDepthStencilState, 0);
	}

	// �u�����h�X�e�[�g���f�o�C�X�ɐݒ肷��
	inline void SetBlendState(ID3D11DeviceContext* context)
	{
		context->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);
	}
};

#endif
