#ifndef __EVTRECOOBJECT_H__
#define __EVTRECOOBJECT_H__

#include <vector>
#include <string>

#include <TObject.h>

enum View_t
  {
    kX = 0,   //x view
    kY = 1,   //y view
    k3D = 3,  //3D view
    kBadView = 5
  };

// integer to view
View_t GetRecoObjView(int iview);

namespace LArReco
{
  // possible reco objects
  enum RecoObj_t
  {
    kNoneObj,          // should not be used

    kQHit,             // Basic q hit
    
    kCluster2d,        // 2D cluster (only a part of track or shower segment)
    kTrack2d,          // 2D track
    kShower2d,         // 2D shower
    kVertex2d,         // 2D vertex
    
    kPoint3d,          // 3D point
    kTrack3d,          // 3D track
    kShower3d,         // 3D shower
    kVertex3d,         // 3D vertex
    
    kPHit,             // Bacic PMT hit
    kLRPEvent          // Event from a light readout
  };


  // names of possible reco objects
  std::string GetRecoObjName(const RecoObj_t &objtype);


  ////////////////////////////////////////////////////////////////////
  
  //
  //
  //  simple object refence 
  //
  class RecoObjLink : public TObject
  {
  public:
    RecoObjLink();
    RecoObjLink(RecoObj_t objtype, int idx);
    
    virtual ~RecoObjLink();
    
    bool IsTransient() const { return (ObjIdx == -1); }
    
    RecoObj_t ObjType;
    
    int ObjIdx;
    
    void Print();

  public:
    ClassDef(RecoObjLink, 1);
  };



  //
  // basic reconstruction object
  //
  class RecoObject : public TObject
  {
  public:
    //RecoObject();
    virtual ~RecoObject();
    
    // clone has to be implemented for each derived object, 
    // did not bother with templates, but it is the CRTP 
    virtual RecoObject* clone() const = 0;
    
    // set reference index for this object
    void SetObjIndex(int idx){ fThisObj.ObjIdx = idx; }
    void SetView(View_t val){ fViewType = val; }
    
    // reco obj properties
    RecoObj_t GetObjType() const { return fThisObj.ObjType; }
    View_t GetView() const { return fViewType; }
    int GetObjIndex() const { return fThisObj.ObjIdx; }
    
    // get basic link to this object
    const RecoObjLink& GetRecoObjLink() const { return fThisObj; }
    
    void PrintNodeInfo();
    
    void ClearUplinks() { fLinkUpNode.clear(); }
    void ClearDownlinks() { fLinkDownNode.clear(); }

    size_t GetNUplinks() const { return fLinkUpNode.size(); }
    size_t GetNDownlinks() const { return fLinkDownNode.size(); }

    // add link to upper nodes
    virtual void LinkToUpperNode(const RecoObjLink &ref)
    {
      if(CheckUplinkPolicy(ref))
	fLinkUpNode.push_back(ref);
    }

    // add link to lower nodes
    virtual void LinkToLowerNode(const RecoObjLink &ref)
    {
      if(CheckDnlinkPolicy(ref))
	fLinkDownNode.push_back(ref);
    }

    // get references
    const std::vector<LArReco::RecoObjLink>& GetUpNodes() const { return fLinkUpNode; }
    const std::vector<LArReco::RecoObjLink>& GetDownNodes() const { return fLinkDownNode; }

  protected:
    // by default we don't care about uplinks
    virtual bool CheckUplinkPolicy(const RecoObjLink &ref){ return false; }
    
    // link only to objects written to file
    virtual bool CheckDnlinkPolicy(const RecoObjLink &ref)
    { 
      if(ref.IsTransient()) return false;
      return true; 
    }

    virtual bool UplinkExists(RecoObjLink &ref);
    virtual bool DnlinkExists(RecoObjLink &ref);

    //protected:
    RecoObjLink fThisObj; //link to this object

    // readout view
    View_t  fViewType;
    
    std::vector<RecoObjLink> fLinkUpNode;    // link to upper nodes
    std::vector<RecoObjLink> fLinkDownNode;  // link to lower nodes
    //std::vector<RecoObjLink> fLinkAssociate; // link associates 

  public:
    ClassDef(RecoObject, 2);
  };
}



#endif
