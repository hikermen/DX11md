#ifndef DIRECTXTEX_WRAPPER_H
#define DIRECTXTEX_WRAPPER_H


#include "DirectXTex.h"
#include <memory>


namespace dxtex
{
	namespace{
		std::wstring widen(const std::string &src) {
			// SJIS → wstring
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, src.c_str()
				, -1, (wchar_t*)NULL, 0);

			// バッファの取得
			wchar_t* cpUCS2 = new wchar_t[iBufferSize];

			// SJIS → wstring
			MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, cpUCS2
				, iBufferSize);

			// stringの生成
			std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

			// バッファの破棄
			delete[] cpUCS2;

			// 変換結果を返す
			return(oRet);
		}
	}
	
	std::shared_ptr<ID3D11ShaderResourceView> CreateTexture(ID3D11Device* device, const std::string filename);

}

#endif