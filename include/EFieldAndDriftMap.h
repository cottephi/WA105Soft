/**********************





*************************/

#ifndef EFIELDANDDRIFTMAP_H
#define EFIELDANDDRIFTMAP_H

//
#include <string>


//
#include "SpaceMap.h"
#include "SpaceMapSet.h"
#include "SpaceMapFolder.h"
#include "SpaceMapMgr.h"



//
#include "FieldMap.h"



class EFieldAndDriftMap: public EFieldMap {

  
 public: 

  EFieldAndDriftMap();
  EFieldAndDriftMap(std::string& file_name);
  EFieldAndDriftMap(std::string& file_name, std::string &tag_list, std::string &histo_list);

  ~EFieldAndDriftMap();

  virtual double EField(double x, double y, double z);

  double GetDriftTime(double x, double y, double z);
  double GetDriftLength(double x, double y, double z);
  double GetEndPointX(double x, double y, double z);
  double GetEndPointY(double x, double y, double z);
  double GetEndPointZ(double x, double y, double z);
  double GetEndPointDX(double x, double y, double z);
  double GetEndPointDY(double x, double y, double z);
  double GetEndPointDZ(double x, double y, double z);

  void SetTags(std::string& tags, std::string& histos);

  bool ReadFile(std::string& file_name);

  
  Map::SpaceMapFolder& GetSpaceMapFolder() { return spaceMapFld_EField;}
 

 private:

  void Init();
  void InitTags();

  Map::SpaceMapFolder spaceMapFld_EField;
 
  Map::SpaceMapMgr spaceMapMgr;
   
  Map::SpaceMapSet* spaceMapSet;

  bool fIsValid;

  //
  TString fTagE,fTagEx,fTagEy,fTagEz;
  TString fTagDriftTime, fTagDriftLen;
  TString fTagEndX,fTagEndY,fTagEndZ;
  TString fTagEndDx,fTagEndDy,fTagEndDz;
  
  TString fTagList;
  TString fHistoList;

};//



#endif
