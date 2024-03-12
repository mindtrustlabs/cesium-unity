#include "CesiumFeatureIdTextureImpl.h"

#include <CesiumGltf/AccessorUtility.h>

#include <DotNet/CesiumForUnity/CesiumFeatureIdTexture.h>
#include <DotNet/CesiumForUnity/CesiumFeatureIdTextureStatus.h>
#include <DotNet/UnityEngine/RaycastHit.h>
#include <DotNet/UnityEngine/Vector2.h>
#include <DotNet/UnityEngine/Vector3.h>

using namespace DotNet::CesiumForUnity;

namespace CesiumForUnityNative {
/*static*/ DotNet::CesiumForUnity::CesiumFeatureIdTexture
CesiumFeatureIdTextureImpl::CreateTexture(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const int64_t featureCount,
    const CesiumGltf::FeatureIdTexture& featureIdTexture) {
  CesiumFeatureIdTexture texture;
  CesiumFeatureIdTextureImpl& textureImpl = texture.NativeImplementation();
  textureImpl._featureIdTextureView =
      CesiumGltf::FeatureIdTextureView(model, featureIdTexture);
  textureImpl._texCoordAccessor = CesiumGltf::getTexCoordAccessorView(
      model,
      primitive,
      featureIdTexture.texCoord);
  textureImpl._indexAccessor =
      CesiumGltf::getIndexAccessorView(model, primitive);
  textureImpl._primitiveMode = primitive.mode;

  switch (textureImpl._featureIdTextureView.status()) {
  case CesiumGltf::FeatureIdTextureViewStatus::Valid:
    texture.status(CesiumFeatureIdTextureStatus::Valid);
    texture.featureCount(featureCount);
    break;
  case CesiumGltf::FeatureIdTextureViewStatus::ErrorInvalidChannels:
    texture.status(CesiumFeatureIdTextureStatus::ErrorInvalidTextureAccess);
    texture.featureCount(0);
    break;
  default:
    // Error with the texture or image. The status is already set by the C#
    // constructor.
    texture.featureCount(0);
    break;
  }

  return texture;
}

std::int64_t CesiumFeatureIdTextureImpl::GetFeatureIdForUV(
    const CesiumFeatureIdTexture& featureIdTexture,
    const DotNet::UnityEngine::Vector2& uv) {
  return this->_featureIdTextureView.getFeatureID(uv.x, uv.y);
}

std::int64_t CesiumFeatureIdTextureImpl::GetFeatureIdForVertex(
    const CesiumFeatureIdTexture& featureIdTexture,
    const int64_t vertexIndex) {
  const std::optional<glm::dvec2> maybeTexCoords = std::visit(
      CesiumGltf::TexCoordFromAccessor{vertexIndex},
      this->_texCoordAccessor);
  if (!maybeTexCoords) {
    return -1;
  }

  auto texCoords = *maybeTexCoords;
  return this->_featureIdTextureView.getFeatureID(texCoords[0], texCoords[1]);
}

std::int64_t CesiumFeatureIdTextureImpl::GetFeatureIdFromRaycastHit(
    const CesiumFeatureIdTexture& featureIdTexture,
    const DotNet::UnityEngine::RaycastHit& hitInfo) {
  int64_t vertexCount =
      std::visit(CesiumGltf::CountFromAccessor{}, this->_texCoordAccessor);

  std::array<int64_t, 3> vertexIndices = std::visit(
      CesiumGltf::IndicesForFaceFromAccessor{
          hitInfo.triangleIndex(),
          vertexCount,
          this->_primitiveMode},
      this->_indexAccessor);

  std::array<glm::dvec2, 3> UVs;
  for (size_t i = 0; i < UVs.size(); i++) {
    auto maybeTexCoord = std::visit(
        CesiumGltf::TexCoordFromAccessor{vertexIndices[i]},
        this->_texCoordAccessor);
    if (!maybeTexCoord) {
      return -1;
    }

    const glm::dvec2& texCoord = *maybeTexCoord;
    UVs[i] = glm::dvec2(texCoord[0], texCoord[1]);
  }

  DotNet::UnityEngine::Vector3 barycentricCoords =
      hitInfo.barycentricCoordinate();

  glm::dvec2 UV = (static_cast<double>(barycentricCoords.x) * UVs[0]) +
                  (static_cast<double>(barycentricCoords.y) * UVs[1]) +
                  (static_cast<double>(barycentricCoords.z) * UVs[2]);

  return this->_featureIdTextureView.getFeatureID(UV.x, UV.y);
}

} // namespace CesiumForUnityNative
