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

	//頂点シェーダ
	std::shared_ptr<ID3D11VertexShader> m_pVertexShader;
	//ハルシェーダ
	std::shared_ptr<ID3D11HullShader> m_pHullShader;
	//ドメインシェーダ
	std::shared_ptr<ID3D11DomainShader> m_pDomainShader;
	//ジオメトリシェーダ
	std::shared_ptr<ID3D11GeometryShader> m_pGeometryShader;
	//ピクセルシェーダ
	std::shared_ptr<ID3D11PixelShader> m_pPixelShader;
	//ラスタライザステート
	ID3D11RasterizerState* m_pRasterizerState;
	//深度ステンシルステート
	ID3D11DepthStencilState* m_pDepthStencilState;
	//ブレンドステート
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

	//頂点シェーダーをファイルから作成する
	void CreateVertexShaderFromFile(
		ID3D11Device* device,
		TCHAR pSrcFile[], CHAR pFunctionName[],
		D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
		);

	//頂点シェーダをメモリから作成する
	void CreateVertexShaderFromMemory(
		ID3D11Device* device,
		BYTE* pShader, size_t size,
		D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements
		);

	//ジオメトリシェーダをファイルから作成する
	void CreateGeometryShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// ジオメトリシェーダーをメモリから作成する
	void CreateGeometryShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// ピクセルシェーダーをファイルから作成する
	void CreatePixelShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// ピクセルシェーダーをメモリから作成する
	void CreatePixelShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// ハルシェーダーをファイルから作成する
	void CreateHullShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// ハルシェーダーをメモリから作成する
	void CreateHullShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);

	// ドメインシェーダーをファイルから作成する
	void CreateDomainShaderFromFile(ID3D11Device* device, TCHAR pSrcFile[], CHAR pFunctionName[]);

	// ドメインシェーダーをメモリから作成する
	void CreateDomainShaderFromMemory(ID3D11Device* device, BYTE* pShader, size_t size);


	// ラスタライザステートを作成
	void CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE CullMode);

	// 深度ステンシルステートを作成
	void CreateDepthStencilState(ID3D11Device* device, BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled, D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS);

	// ブレンドステートを作成
	void CreateBlendState(ID3D11Device* device, UEBLEND_STATE* BlendStateType, UINT BlendStateTypeLength, BOOL AlphaToCoverageEnable = FALSE);

	// 定数バッファを作成する
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, void* pData, size_t size, UINT CPUAccessFlag);

	// 頂点シェーダーをデバイスに設定する
	inline void SetVertexShader(ID3D11DeviceContext* context)
	{
		// 入力アセンブラー ステージを設定
		context->IASetInputLayout(m_pLayout.get());
		// 頂点シェーダーをデバイスに設定する。
		context->VSSetShader(m_pVertexShader.get(), nullptr, 0);
	}

	// ハルシェーダーをデバイスに設定する
	inline void SetHullShader(ID3D11DeviceContext* context)
	{
		context->HSSetShader(m_pHullShader.get(), nullptr, 0);
	}

	// ドメインシェーダーをデバイスに設定する
	inline void SetDomainShader(ID3D11DeviceContext* context)
	{
		context->DSSetShader(m_pDomainShader.get(), nullptr, 0);
	}

	// ジオメトリシェーダーをデバイスに設定する
	inline void SetGeometryShader(ID3D11DeviceContext* context)
	{
		context->GSSetShader(m_pGeometryShader.get(), nullptr, 0);
	}

	// ピクセルシェーダーをデバイスに設定する
	inline void SetPixelShader(ID3D11DeviceContext* context)
	{
		context->PSSetShader(m_pPixelShader.get(), nullptr, 0);
	}

	// ラスタライザステートをデバイスに設定する
	inline void SetRasterizerState(ID3D11DeviceContext* context)
	{
		context->RSSetState(m_pRasterizerState);
	}

	// 深度ステンシルステートをデバイスに設定する
	inline void SetDepthStencilState(ID3D11DeviceContext* context)
	{
		context->OMSetDepthStencilState(m_pDepthStencilState, 0);
	}

	// ブレンドステートをデバイスに設定する
	inline void SetBlendState(ID3D11DeviceContext* context)
	{
		context->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);
	}
};

#endif
