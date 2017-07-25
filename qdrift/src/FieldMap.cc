///////////////////////////////////////////////////////////////////////////
//
//  Field map definitions
//
///////////////////////////////////////////////////////////////////////////

#include "FieldMap.h"

// base class to define field map
EFieldMap::EFieldMap()
{
  m_mapname = "zero_efield";
}

EFieldMap::~EFieldMap()
{;}


// uniform field
UniformEField::UniformEField(double efield)
{
  m_mapname = "uniform_efield";
 
  m_efield  = efield;
}

UniformEField::~UniformEField()
{;}

// more complicated field maps ...
