#include "DirectXTexWrapper.h"



namespace dxtex
{
	std::shared_ptr<ID3D11ShaderResourceView> CreateTexture(ID3D11Device* device, const std::string filename)
	{
		DirectX::ScratchImage image;

		HRESULT hr;
		if (filename.find(".dds") != std::string::npos)
		{
			hr = DirectX::LoadFromDDSFile(widen(filename).c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
		}
		else if (filename.find(".tga") != std::string::npos)
		{
			hr = DirectX::LoadFromTGAFile(widen(filename).c_str(), nullptr, image);
		}
		else
		{
			hr = DirectX::LoadFromWICFile(widen(filename).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
		}

		if (SUCCEEDED(hr))
		{
			ID3D11Resource* pResource = nullptr;
			hr = DirectX::CreateTexture(device,
				image.GetImages(), image.GetImageCount(),
				image.GetMetadata(), &pResource);
			if (FAILED(hr)) throw;

			ID3D11ShaderResourceView* srv;

			device->CreateShaderResourceView(pResource, nullptr, &srv);

			pResource->Release();

			return std::shared_ptr<ID3D11ShaderResourceView>(srv, [](ID3D11ShaderResourceView* srv)
			{
				srv->Release();
			});

		}
		else
		{
			throw;
		}


	}
}