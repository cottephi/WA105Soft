#include <iostream>

#include "RecoObject.h"

ClassImp(LArReco::RecoObjLink)
ClassImp(LArReco::RecoObject)


View_t GetRecoObjView(int iview)
{
  switch(iview)
    {
    case 0:
      return kX;
    case 1:
      return kY;
    case 3:
      return k3D;

    default:
      return kBadView;
    }

  return kBadView;
}

// names of possible reco objects
std::string LArReco::GetRecoObjName(const RecoObj_t &objtype)
{
  std::string rstr = "";
      
  switch (objtype)
    {
    case kQHit:
      rstr = "QHit";
      break;

    case kCluster2d:
      rstr = "Cluster2d";
      break;   
    
    case kTrack2d:
      rstr = "Track2d";
      break;
    case kShower2d:
      rstr = "Shower2d";
      break;
    case kVertex2d:
      rstr = "Vertex2d";
      break;
      
    case kPoint3d:
      rstr = "Point3d";
      break;
    case kTrack3d:
      rstr = "Track3d";
      break;
    case kShower3d:
      rstr = "Shower3d";
      break;
    case kVertex3d:
      rstr = "Vertex3d";
      break;
	  
    case kPHit:
      rstr = "PHit";
      break;
    case kLRPEvent:
      rstr = "LRPEvent";
      break;

    default:
      break;
	  
    }
      
  return rstr;
}

// a very basic object for linking between different reco nodes
LArReco::RecoObjLink::RecoObjLink()
{
  ObjType = LArReco::kNoneObj; 
  ObjIdx  = -1;
}

//
LArReco::RecoObjLink::RecoObjLink(RecoObj_t objtype, int idx)
{
  ObjType = objtype; 
  ObjIdx  = idx;
}

//
LArReco::RecoObjLink::~RecoObjLink()
{;}

//
void LArReco::RecoObjLink::Print()
{
  std::cout<<"Reco obj type, idx: "<<ObjType<<", "<<ObjIdx<<std::endl;
}

//
// ctor
//
//LArReco::RecoObject::RecoObject()
//{
//
//fName  = "";
//fTitle = "";

//fThisObj = LArReco::RecoObjLink(LArReco::kNoneObj, -1);
//}


//
// dtor
//
LArReco::RecoObject::~RecoObject()
{;}


// print basic debug info
void LArReco::RecoObject::PrintNodeInfo()
{
  std::cout<<"This node "<<std::endl;
  fThisObj.Print();
  
  
  std::cout<<"Links to upper nodes "<<fLinkUpNode.size()<<std::endl;
  std::cout<<"Links to lower nodes "<<fLinkDownNode.size()<<std::endl;
  
  std::cout<<"Upper nodes : "<<std::endl;
  for(size_t i=0;i<fLinkUpNode.size();i++)
    fLinkUpNode[i].Print();

  std::cout<<"Lower nodes : "<<std::endl;
  for(size_t i=0;i<fLinkDownNode.size();i++)
    fLinkDownNode[i].Print();
}


//
//
//
bool LArReco::RecoObject::DnlinkExists(RecoObjLink &ref)
{
  if(fLinkDownNode.empty()) return false;
  
  // otherwise need to loop
  for(size_t i = 0;i<fLinkDownNode.size();i++)
    {
      if(ref.ObjType != fLinkDownNode[i].ObjType) continue;
      if(ref.ObjIdx == fLinkDownNode[i].ObjIdx) return true;
    }
  
  return false;			     
}


//
//
//
bool LArReco::RecoObject::UplinkExists(RecoObjLink &ref)
{
  if(fLinkUpNode.empty()) return false;
  
  // otherwise need to loop
  for(size_t i = 0;i<fLinkUpNode.size();i++)
    {
      if(ref.ObjType != fLinkUpNode[i].ObjType) continue;
      if(ref.ObjIdx == fLinkUpNode[i].ObjIdx) return true;
    }
  
  return false;			     
}
