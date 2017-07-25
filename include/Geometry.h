#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "TString.h"
#include "BasicGeomMapping.h"

using namespace std;

void InitGeometry(void);

 
class Geometry 
{

 public:


  Geometry();
  virtual ~Geometry(){;}
  
 
  
  Float_t GetGG_SRxdimension() {return GG_SRxdimension;}
  Float_t GetGG_SRydimension(){return GG_SRydimension;}
  Float_t GetGG_SRzdimension() {return GG_SRzdimension;}  
 

  
  void SetGG_SRxdimension(Float_t xdim) {GG_SRxdimension=xdim;}
  void SetGG_SRydimension(Float_t ydim){GG_SRydimension=ydim;}
  void SetGG_SRzdimension(Float_t zdim) {GG_SRzdimension=zdim;}  

  Float_t GG_SRxdimension;
  Float_t GG_SRydimension;
  Float_t GG_SRzdimension;  
  

 private:
  
  
    
};

extern Geometry *ageom;

#endif
