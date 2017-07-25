/**
 * \file BasicGeomMapping.h
 * \author Andre Rubbia
 * \date 7-Mar-2010
 */

#ifndef _BASICGEOMMAPPING_H_
#define _BASICGEOMMAPPING_H_

#include <iostream>

#include <TGeoManager.h>


// basic geometry class
class BasicGeomMapping
{
 public:

  BasicGeomMapping();
  virtual ~BasicGeomMapping();
 
  // to be implemented by derived classes
  virtual void CreateGeometry() = 0;
  
  // drift length (to be implemented in derived classes)
  virtual double geomDrift(){ return 0; }
  
  TGeoManager *GetGeoManager(void) { return gGeoManager;};
  TGeoVolume  *GetGeoTopVolume(void) { return aGeoTopVolume;};
  
  void SetGeoTopVolume(TGeoVolume *vol) { aGeoTopVolume = vol; };  

  void CheckGeometry(double x, double y, double z);

  //tracking media
  TGeoMedium *vacuum_med;
  TGeoMedium *air_med;
  TGeoMedium *largon_tracking_med;
  TGeoMedium *largon_med;
  TGeoMedium *gargon_med;
  TGeoMedium *stainless_med;
  TGeoMedium *concrete_med;
  TGeoMedium *foam_med;

  //fiducial region dimension
  double heightFiducial;  
  double widthFiducial;             
  double lengthFiducial;  
  
 protected:
  // material definition
  virtual void ConstructMaterials();
  
  // construct all membrane tank materials and dlar tpc
  virtual void ConstructAllMaterial();
 
  TGeoVolume *aGeoTopVolume;

};

#endif
