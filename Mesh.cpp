#include "Mesh.h"

std::shared_ptr<ID3D11SamplerState> IMesh::CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode, D3D11_FILTER Filter)
{
	ID3D11SamplerState* pSamplerState = nullptr;

	D3D11_SAMPLER_DESC samplerDesc;
	::ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = Filter;					// サンプリング時に使用するフィルタ。
	samplerDesc.AddressU = TextureAddressMode;		// 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
	samplerDesc.AddressV = TextureAddressMode;		// 0 ～ 1 の範囲外にある v テクスチャー座標
	samplerDesc.AddressW = TextureAddressMode;		// 0 ～ 1 の範囲外にある w テクスチャー座標
	samplerDesc.MipLODBias = 0;						// 計算されたミップマップ レベルからのバイアス
	samplerDesc.MaxAnisotropy = 1;					// サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto hr = device->CreateSamplerState(&samplerDesc, &pSamplerState);

	if (FAILED(hr))
		throw(Exception(-1, _T("IMesh::CreateSamplerState()でエラーが発生しました。サンプラーステートの作成に失敗しました。")));

	return std::shared_ptr<ID3D11SamplerState>(pSamplerState, [](ID3D11SamplerState* sampler)
	{
		sampler->Release();
	});
}