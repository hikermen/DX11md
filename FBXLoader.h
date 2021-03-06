#ifndef FBX_IMPORTER_H
#define FBX_IMPORTER_H

#include <iostream>
#include <vector>
#include <map>
#include <fbxsdk.h>
#include <fbxsdk\fbxsdk_version.h>

#include <DirectXMath.h>

#include "DXGI_FORMAT.h"


struct ModelBoneWeight
{
  uint8_t boneIndex[4];
  DirectX::XMFLOAT4 boneWeight;
};

struct ModelVertex
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT3 normal;
  DirectX::XMFLOAT2 uv0;
  /*
  uint8_t boneIndex[4];
  DirectX::XMFLOAT4 boneWeight;
  */
  bool operator == (const ModelVertex& v) const
  {
    return std::memcmp(this, &v, sizeof(ModelVertex)) == 0;
  }
};

struct ModelMesh
{
  std::string nodeName;
  std::string materialName;
  
  std::vector<ModelVertex> vertexList;
  std::vector<INDEX_FORMAT_TYPE> indexList;
  
  DirectX::XMFLOAT4X4 invMeshBaseposeMatrix;
  std::vector<std::string> boneNodeNameList;
  std::vector<DirectX::XMFLOAT4X4> invBoneBaseposeMatrixList;
};

struct ModelMaterial
{
  std::string materialName;
  
  std::string diffuseTextureName;
  std::string normalTextureName;
  std::string specularTextureName;
  std::string falloffTextureName;
  std::string reflectionMapTextureName;
};


class FBXLoader
{
public:
  
  bool Initialize(const char* filepath);
  void Finalize();
  
  bool LoadAnimation(const char* filepath);
  
  int GetMeshCount() const
  {
    return (int)this->meshList.size();
  }
  const ModelMesh& GetMesh(int index) const
  {
    return this->meshList[index];
  }
  
  int GetMaterialCount() const
  {
    return (int)this->materialList.size();
  }
  
  const int GetMaterialId(const std::string& materialName) const
  {
    return this->materialIdDictionary.at(materialName);
  }
  
  const ModelMaterial& GetMaterial(int materialId) const
  {
    return this->materialList[materialId];
  }
  
  float GetAnimationStartFrame() const
  {
    return this->animationStartFrame;
  }
  
  float GetAnimationEndFrame() const
  {
    return this->animationEndFrame;
  }
  
  void GetMeshMatrix(float frame, int meshId, DirectX::XMFLOAT4X4* out_matrix) const;
  void GetBoneMatrix(float frame, int meshId, DirectX::XMFLOAT4X4* out_matrixList, int matrixCount) const;
  
private:
  
  ModelMesh ParseMesh(FbxMesh* mesh);
  ModelMaterial ParseMaterial(FbxSurfaceMaterial* material);
  
private:
  
  FbxManager* sdkManager;
  FbxScene* fbxScene;
  
  std::vector<ModelMesh> meshList;
  std::vector<ModelMaterial> materialList;
  std::map<std::string, int> materialIdDictionary;
  std::map<std::string, int> nodeIdDictionary;
  
  FbxScene* fbxSceneAnimation;
  std::map<std::string, int> nodeIdDictionaryAnimation;
  float animationStartFrame;
  float animationEndFrame;
};

#endif
