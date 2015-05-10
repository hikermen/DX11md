#include "Mesh.h"

std::shared_ptr<ID3D11SamplerState> IMesh::CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode, D3D11_FILTER Filter)
{
	ID3D11SamplerState* pSamplerState = nullptr;

	D3D11_SAMPLER_DESC samplerDesc;
	::ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = Filter;					// �T���v�����O���Ɏg�p����t�B���^�B
	samplerDesc.AddressU = TextureAddressMode;		// 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressV = TextureAddressMode;		// 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
	samplerDesc.AddressW = TextureAddressMode;		// 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
	samplerDesc.MipLODBias = 0;						// �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = 1;					// �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto hr = device->CreateSamplerState(&samplerDesc, &pSamplerState);

	if (FAILED(hr))
		throw(Exception(-1, _T("IMesh::CreateSamplerState()�ŃG���[���������܂����B�T���v���[�X�e�[�g�̍쐬�Ɏ��s���܂����B")));

	return std::shared_ptr<ID3D11SamplerState>(pSamplerState, [](ID3D11SamplerState* sampler)
	{
		sampler->Release();
	});
}