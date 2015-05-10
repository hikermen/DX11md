#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>

#include "Common.h"
#include "Direct3D11.h"

#include <DirectXMath.h>

namespace model
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 uv0;

		bool operator < (const Vertex& v)const {
			return std::memcmp(this, &v, sizeof(Vertex)) < 0;
		}
	};

	struct Mesh
	{
		std::string nodeName;
		std::string materialName;

		std::vector<Vertex> vertexList;
		std::vector<uint16_t> indexList;

		DirectX::XMFLOAT4X4 invMeshBaseposeMatrix;
		DirectX::XMFLOAT4X4 meshBaseposeMatrix;
		std::vector<std::string> boneNodeNameList;
		std::vector<DirectX::XMFLOAT4X4> invBoneBaseposeMatrixList;
	};

	struct Material
	{
		std::string materialName;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 emissive;
		double transparency;

		DirectX::XMFLOAT3 specular;
		double shininess;

		std::string diffuseTextureName;
		std::string normalTextureName;
		std::string specularTextureName;
		std::string falloffTextureName;
		std::string reflectionMapTextureName;
	};


	//汎用モデルデータ
	class UCModel
	{
	private:
		std::vector<Mesh> m_meshList;
		std::map<std::string, int> m_nodeIdDictionary;

		std::vector<Material> m_materialList;
		std::map<std::string, int> m_materialIdDictionary;
	public:
		UCModel(
			std::vector<Mesh> meshList,
			std::vector<Material> materialList,

			std::map<std::string, int> nodeIdDictionary,
			std::map<std::string, int> materialIdDictionary
			) : m_meshList(meshList),
			m_materialList(materialList),
			m_nodeIdDictionary(nodeIdDictionary),
			m_materialIdDictionary(materialIdDictionary) {};

		const Mesh & getMesh(int index) const { return m_meshList[index]; };
		int getMeshCount() const { return m_meshList.size(); }
		const Material & getMaterial(int index) const { return m_materialList[index]; };
		int getMaterialID(const std::string & materialName) const { return m_materialIdDictionary.at(materialName); }
		int getMaterialCount() const { return m_materialList.size(); }
	};

}
#endif