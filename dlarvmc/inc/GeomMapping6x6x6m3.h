/**
 * \file GeomMapping6x6x6m3.h
 * \author Devis Lussi
 * \date 20-Oct-2012
 */

#ifndef _GEOMMAPPING6x6x6m3_H_
#define _GEOMMAPPING6x6x6m3_H_

#include "BasicGeomMapping.h"

// simple geometry for 6x6x6

class GeomMapping6x6x6m3 : public BasicGeomMapping
{
 public:
  GeomMapping6x6x6m3(); 
  ~GeomMapping6x6x6m3();

  void CreateGeometry();

  //maximum drift length in cm 
  double geomDrift() { return 600.; };  //maximum drift length in cm 
  
 protected:
  //void ConstructMaterials();
  //void ConstructAllMaterial(); 
};

#endif
