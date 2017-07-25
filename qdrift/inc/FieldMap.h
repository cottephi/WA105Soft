///////////////////////////////////////////////////////////////////////////
//
//  Field map definitions
//
///////////////////////////////////////////////////////////////////////////

#ifndef __FIELDMAP_H__
#define __FIELDMAP_H__

#include <string>

/////////////////////////////////////////////////////////////////////
// zero field
class EFieldMap // base class
{
 public:
  EFieldMap();
  virtual ~EFieldMap();

  // to be overwritten by derived classes
  virtual double EField(double x, double y, double z){ return 0; }

  // name of the map
  std::string GetEFieldMapName(){ return m_mapname; }

 protected:
  std::string m_mapname;
};


/////////////////////////////////////////////////////////////////////
// uniform field definition
class UniformEField : public EFieldMap 
{
 public:
  UniformEField(double efield); //in V/cm
  ~UniformEField();

  double EField(double x=0, double y=0, double z=0){ return m_efield; }
  void SetEField(double val){ m_efield = val; }
  
 private:
  double m_efield; // V/cm
};

/////////////////////////////////////////////////////////////////////
// more complicated field maps ...
//class ComsolEField // 
#endif
