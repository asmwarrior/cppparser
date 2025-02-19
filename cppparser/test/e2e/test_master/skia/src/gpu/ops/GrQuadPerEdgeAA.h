/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef GrQuadPerEdgeAA_DEFINED
#  define GrQuadPerEdgeAA_DEFINED
#  include "include/core/SkPoint.h"
#  include "include/core/SkPoint3.h"
#  include "include/private/GrTypesPriv.h"
#  include "src/gpu/GrColor.h"
#  include "src/gpu/GrGeometryProcessor.h"
#  include "src/gpu/GrSamplerState.h"
#  include "src/gpu/geometry/GrQuad.h"
#  include "src/gpu/ops/GrMeshDrawOp.h"
#  include "src/gpu/ops/GrTextureOp.h"
class GrCaps;
class GrColorSpaceXform;
class GrShaderCaps;
namespace GrQuadPerEdgeAA
{
  using Saturate = GrTextureOp::Saturate;
  enum class Domain : bool {
 kNo = false, kYes = true
  };
  enum class ColorType {
 kNone, kByte, kHalf, kLast = kHalf
  };
  static const int kColorTypeCount = static_cast<int>(ColorType::kLast) + 1;
    // Gets the minimum ColorType that can represent a color.
  ColorType MinColorType(SkPMColor4f, GrClampType, const GrCaps&);
    // Specifies the vertex configuration for an op that renders per-edge AA quads. The vertex
    // order (when enabled) is device position, color, local position, domain, aa edge equations.
    // This order matches the constructor argument order of VertexSpec and is the order that
    // GPAttributes maintains. If hasLocalCoords is false, then the local quad type can be ignored.
  struct VertexSpec
  {
  public:
    VertexSpec(GrQuad::Type deviceQuadType, ColorType colorType, GrQuad::Type localQuadType, bool hasLocalCoords, Domain domain, GrAAType aa, bool coverageAsAlpha)
      : fDeviceQuadType(static_cast<unsigned>(deviceQuadType))
      , fLocalQuadType(static_cast<unsigned>(localQuadType))
      , fHasLocalCoords(hasLocalCoords)
      , fColorType(static_cast<unsigned>(colorType))
      , fHasDomain(static_cast<unsigned>(domain))
      , fUsesCoverageAA(aa == GrAAType::kCoverage)
      , fCompatibleWithCoverageAsAlpha(coverageAsAlpha)
      , fRequiresGeometryDomain(aa == GrAAType::kCoverage && deviceQuadType > GrQuad::Type::kRectilinear)
    {
    }
    GrQuad::Type deviceQuadType() const
    {
      return static_cast<GrQuad::Type>(fDeviceQuadType);
    }
    GrQuad::Type localQuadType() const
    {
      return static_cast<GrQuad::Type>(fLocalQuadType);
    }
    bool hasLocalCoords() const
    {
      return fHasLocalCoords;
    }
    ColorType colorType() const
    {
      return static_cast<ColorType>(fColorType);
    }
    bool hasVertexColors() const
    {
      return ColorType::kNone != this->colorType();
    }
    bool hasDomain() const
    {
      return fHasDomain;
    }
    bool usesCoverageAA() const
    {
      return fUsesCoverageAA;
    }
    bool compatibleWithCoverageAsAlpha() const
    {
      return fCompatibleWithCoverageAsAlpha;
    }
    bool requiresGeometryDomain() const
    {
      return fRequiresGeometryDomain;
    }
        // Will always be 2 or 3
    int deviceDimensionality() const;
        // Will always be 0 if hasLocalCoords is false, otherwise will be 2 or 3
    int localDimensionality() const;
    int verticesPerQuad() const
    {
      return fUsesCoverageAA ? 8 : 4;
    }
  private:
    static_assert(GrQuad::kTypeCount <= 4, "GrQuad::Type doesn't fit in 2 bits");
    static_assert(kColorTypeCount <= 4, "Color doesn't fit in 2 bits");
    unsigned fDeviceQuadType;
    unsigned fLocalQuadType;
    unsigned fHasLocalCoords;
    unsigned fColorType;
    unsigned fHasDomain;
    unsigned fUsesCoverageAA;
    unsigned fCompatibleWithCoverageAsAlpha;
        // The geometry domain serves to clip off pixels touched by quads with sharp corners that
        // would otherwise exceed the miter limit for the AA-outset geometry.
    unsigned fRequiresGeometryDomain;
  };
  sk_sp<GrGeometryProcessor> MakeProcessor(const VertexSpec& spec);
  sk_sp<GrGeometryProcessor> MakeTexturedProcessor(const VertexSpec& spec, const GrShaderCaps& caps, GrTextureType textureType, const GrSamplerState& samplerState, const GrSwizzle& swizzle, uint32_t extraSamplerKey, sk_sp<GrColorSpaceXform> textureColorSpaceXform, Saturate saturate);
    // Fill vertices with the vertex data needed to represent the given quad. The device position,
    // local coords, vertex color, domain, and edge coefficients will be written and/or computed
    // based on the configuration in the vertex spec; if that attribute is disabled in the spec,
    // then its corresponding function argument is ignored.
    //
    // Tessellation is based on the quad type of the vertex spec, not the provided GrQuad's
    // so that all quads in a batch are tessellated the same.
    //
    // Returns the advanced pointer in vertices.
  void* Tessellate(void* vertices, const VertexSpec& spec, const GrQuad& deviceQuad, const SkPMColor4f& color, const GrQuad& localQuad, const SkRect& domain, GrQuadAAFlags aa);
    // The mesh will have its index data configured to meet the expectations of the Tessellate()
    // function, but it the calling code must handle filling a vertex buffer via Tessellate() and
    // then assigning it to the returned mesh.
    //
    // Returns false if the index data could not be allocated.
  bool ConfigureMeshIndices(GrMeshDrawOp::Target* target, GrMesh* mesh, const VertexSpec& spec, int quadCount);
  static constexpr int kNumAAQuadsInIndexBuffer = 512;
}
#endif
