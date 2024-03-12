#pragma once

#include <CesiumGltf/AccessorUtility.h>
#include <CesiumGltf/FeatureIdTextureView.h>

namespace DotNet::CesiumForUnity {
class CesiumFeatureIdTexture;
} // namespace DotNet::CesiumForUnity

namespace DotNet::UnityEngine {
class Vector2;
class RaycastHit;
} // namespace DotNet::UnityEngine

namespace CesiumGltf {
struct Model;
struct MeshPrimitive;
struct FeatureIdTexture;
} // namespace CesiumGltf

namespace CesiumForUnityNative {
class CesiumFeatureIdTextureImpl {
public:
  ~CesiumFeatureIdTextureImpl(){};
  CesiumFeatureIdTextureImpl(
      const DotNet::CesiumForUnity::CesiumFeatureIdTexture& featureIdTexture){};
  void JustBeforeDelete(
      const DotNet::CesiumForUnity::CesiumFeatureIdTexture& featureIdTexture){};

  static DotNet::CesiumForUnity::CesiumFeatureIdTexture CreateTexture(
      const CesiumGltf::Model& model,
      const CesiumGltf::MeshPrimitive& primitive,
      const int64_t featureCount,
      const CesiumGltf::FeatureIdTexture& featureIdTexture);

  std::int64_t GetFeatureIdForUV(
      const DotNet::CesiumForUnity::CesiumFeatureIdTexture& featureIdTexture,
      const DotNet::UnityEngine::Vector2& uv);

  std::int64_t GetFeatureIdForVertex(
      const DotNet::CesiumForUnity::CesiumFeatureIdTexture& featureIdTexture,
      const int64_t vertexIndex);

  std::int64_t GetFeatureIdFromRaycastHit(
      const DotNet::CesiumForUnity::CesiumFeatureIdTexture& featureIdTexture,
      const DotNet::UnityEngine::RaycastHit& hitInfo);

private:
  CesiumGltf::FeatureIdTextureView _featureIdTextureView;
  CesiumGltf::TexCoordAccessorType _texCoordAccessor;
  CesiumGltf::IndexAccessorType _indexAccessor;
  int32_t _primitiveMode;
};
} // namespace CesiumForUnityNative
