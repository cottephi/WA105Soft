/***********************





************************/


//
#include <memory>
#include <cassert>
#include <iostream>
#include <vector> 
#include <initializer_list>

//
#include <TString.h>
#include <TFile.h>


//
#include "StringUtils.h"


#include "SpaceMap.h"
#include "SpaceMap3.h"
#include "SpaceMapSet.h"
#include "SpaceMapFolder.h"
#include "SpaceMapMgr.h"



#include "EFieldAndDriftMap.h"


using std::cout;
using std::endl;



EFieldAndDriftMap::EFieldAndDriftMap() {

  Init();
  

}




EFieldAndDriftMap::EFieldAndDriftMap(std::string& file_name) {

  Init();

  ReadFile(file_name);

}


EFieldAndDriftMap::EFieldAndDriftMap(std::string& file_name, std::string &tag_list, std::string &histo_list) {

  Init();

  SetTags(tag_list,histo_list);

  ReadFile(file_name);

}




EFieldAndDriftMap::~EFieldAndDriftMap() {

}



double EFieldAndDriftMap::EField(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }


  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagE) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();

    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagE<<" Not Found"<<endl;
    return 0;

  }//GetMap


  return 0;

}


double EFieldAndDriftMap::GetDriftTime(double x, double y, double z) { 

  //cout<<__FUNCTION__<<endl;


  if( !fIsValid ) {
    return 0;
  }

  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagDriftTime) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();


    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagDriftTime<<" Not Found"<<endl;
    return 0;

  }//GetMap



  return 0;
}


double EFieldAndDriftMap::GetDriftLength(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }


  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagDriftLen) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();

    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagDriftLen<<" Not Found"<<endl;
    return 0;

  }//GetMap

  return 0;

}


double EFieldAndDriftMap::GetEndPointDX(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagEndDx) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();

    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagEndDx<<" Not Found"<<endl;
    return 0;

  }//GetMap
  


  return 0;

}




double EFieldAndDriftMap::GetEndPointDY(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagEndDy) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();

    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagEndDx<<" Not Found"<<endl;
    return 0;

  }//GetMap
  


  return 0;

}



double EFieldAndDriftMap::GetEndPointDZ(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  if( Map::SpaceMap3 *map3 = (Map::SpaceMap3*)spaceMapSet->GetMap(fTagEndDz) ){

    map3->SetPoint(x,y,z);

    double val = map3->Evaluate();

    return val;

  }else{
	
    cout<<__PRETTY_FUNCTION__<<" Map "<< fTagEndDz<<" Not Found"<<endl;
    return 0;

  }//GetMap
  


  return 0;

}



double EFieldAndDriftMap::GetEndPointX(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  return 0;

}


double EFieldAndDriftMap::GetEndPointY(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  return 0;

}


double EFieldAndDriftMap::GetEndPointZ(double x, double y, double z) { 

  if( !fIsValid ) {
    return 0;
  }

  return 0;

}





bool EFieldAndDriftMap::ReadFile(std::string& file_name) {
  //
  //
  //

  TFile file(file_name.c_str());


 
  spaceMapFld_EField.ImportSet(file,fTagList,fHistoList);

  //
  // Checks
  
  if( spaceMapFld_EField.GetNumOfSet() != 1 ) {

    cout<<" ####### "<<__PRETTY_FUNCTION__<<" More there one MapSet found "<<endl;     

    spaceMapSet = 0;

    fIsValid = false;
    
    return fIsValid;

  }

    
  if( Map::SpaceMapSet *mapSet = (Map::SpaceMapSet*)spaceMapFld_EField.GetSet(0) ){
    
    spaceMapSet = mapSet;

    fIsValid = true;

    return fIsValid;
  }else{

    cout<<" ###### "<<__PRETTY_FUNCTION__<<" MapSet Not Found"<<endl;
      
    spaceMapSet = 0;

    fIsValid = false;

    return fIsValid;
  }

  return fIsValid;

  
}

void EFieldAndDriftMap::SetTags(std::string& tags, std::string& histos) {


  std::cout<<" -- "<<__FUNCTION__<<" -- "<<std::endl;


  TString ttags = TString(tags.c_str());

  TString thistos = TString(histos.c_str());

  if( ttags.IsWhitespace() || thistos.IsWhitespace() ){
    
    std::cout<<" ###### "<<__PRETTY_FUNCTION__<<" Invalid Input"<<std::endl;
    return;
  }

  std::vector<TString> t_list = STR_UTILS::Tokenize( TString(tags.c_str()) ,":");


  for(int i=0; i<t_list.size(); ++i) {

    if( !fTagList.Contains(t_list[i]) ){

      std::cout<<" ##### "<<__PRETTY_FUNCTION__<<std::endl;
      std::cout<<" ##### Invalid Tag: "<<t_list[i]<<std::endl;
      std::cout<<" ##### Available Tags are: "<<fTagList<<std::endl;

      assert(0);
    }
    
  }//   




    std::vector<TString> h_list = STR_UTILS::Tokenize( TString(histos.c_str()),":");


    if( h_list.size() != t_list.size() ){
      
      std::cout<<" #### Number of Tags and Histos does not match: "<<std::endl;
      std::cout<<"   "<<tags<<" : "<<t_list.size()<<std::endl;
      std::cout<<"   "<<histos<<" : "<<h_list.size()<<std::endl;
      

      exit(0);

    }else{

      
      for(int i=0; i<t_list.size(); ++i){

	std::cout<<" variable:  "<<t_list[i]<<" from histo: "<<h_list[i]<<endl;
      }

      fTagList = TString(tags.c_str());
      fHistoList = TString(histos.c_str());
	

    }


    std::cout<<" "<<std::endl;
    std::cout<<" "<<fTagList<<std::endl;
    std::cout<<" "<<fHistoList<<std::endl;
    
    std::cout<<" --- "<<std::endl;

}



void  EFieldAndDriftMap::Init() {


  spaceMapFld_EField.SetName("EField");

  InitTags();

  spaceMapSet = 0;

  fIsValid = false;
}


void EFieldAndDriftMap::InitTags() {


  //
  //
  //
  fTagE  = "E"; 
  fTagEx = "Ex"; 
  fTagEy = "Ey"; 
  fTagEz = "Ez"; 
  fTagDriftTime = "DriftTime"; 
  fTagDriftLen = "DriftLen"; 
  fTagEndX = "EndPointX";
  fTagEndY = "EndPointY";
  fTagEndZ = "EndPointZ"; 
  fTagEndDx = "EndPointDx"; 
  fTagEndDy = "EndPointDy"; 
  fTagEndDz = "EndPointDz"; 

  //c++11
  //std::vector<TString> tags_all = {fTagE,fTagEx,fTagEz,fTagEy,fTagDriftTime,fTagDriftLen
  //			       ,fTagEndX,fTagEndY,fTagEndZ
  //			       ,fTagEndDx,fTagEndDy,fTagEndDz};

  //c++11
  //std::vector<TString> tags = {fTagE,fTagEx,fTagEz,fTagEy,fTagDriftTime,fTagDriftLen,fTagEndDx,fTagEndDy,fTagEndDz};
  //tag_list = STR_UTILS::Append(tags,":");


  TString atags[]={fTagE,fTagEx,fTagEz,fTagEy,fTagDriftTime,fTagDriftLen,fTagEndDx,fTagEndDy,fTagEndDz};

  fTagList = STR_UTILS::Append(atags,9,":");
  
  fHistoList = "hEtot:hEx:hEy:hEz:hDriftTime:hDriftLen:hEndPointDX:hEndPointDY:hEndPointDZ";

}
