//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2018 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// AcDbIndex is the base class for all index implementations.
//
// These indexes are associated with Block Spaces, and can be used
// to perform an efficient filtered traversal of the block.
//
#ifndef AD_DBLYINDX_H
#  define AD_DBLYINDX_H
#  include "dbindex.h"
#  pragma  pack (push, 8)
class AcDbLayerIndex : public AcDbIndex
{
public:
  ACDB_DECLARE_MEMBERS(AcDbLayerIndex);
  AcDbLayerIndex();
  virtual ~AcDbLayerIndex();
  AcDbFilteredBlockIterator* newIterator(const AcDbFilter* pFilter) const override;
  Acad::ErrorStatus rebuildFull(AcDbIndexUpdateData* pIdxData) override;
  Acad::ErrorStatus compute(AcDbLayerTable* pLT, AcDbBlockTableRecord* pBTR);
protected:
  Acad::ErrorStatus rebuildModified(AcDbBlockChangeIterator* iter) override;
};
class AcDbImpLayerIndexIterator;
class AcDbLayerFilter;
class AcDbLayerIndexIterator : public AcDbFilteredBlockIterator
{
public:
  AcDbLayerIndexIterator(const AcDbLayerIndex* pIndex, const AcDbLayerFilter* pFilter);
  virtual ~AcDbLayerIndexIterator();
  Acad::ErrorStatus start() override;
  AcDbObjectId next() override;
  AcDbObjectId id() const override;
  Acad::ErrorStatus seek(AcDbObjectId id) override;
  double estimatedHitFraction() const override;
  Acad::ErrorStatus accepts(AcDbObjectId id, Adesk::Boolean& idPassesFilter) const override;
private:
  AcDbImpLayerIndexIterator* mpImpIter;
};
#  pragma  pack (pop)
#endif
