#ifndef _VMCTrack
#define _VMCTrack

#include "TGeoTrack.h"
//#include "TPolyline3D.h"
#include "TVirtualGeoTrack.h"

class VMCTrack : public TGeoTrack
{
public:

private :
   Int_t             fPointsAttachmentSize;     // capacity of pointsAdd array
   Int_t             fNpointsAttachment;    // number of stored additional information (Edep, step)
   Double_t         *fPointsAttachment;    //[fNpointsAttachment] array of values (edep,step) belonging to this track
 
protected:
   VMCTrack(const VMCTrack&);

public:
   VMCTrack();
   VMCTrack(Int_t id, Int_t pdgcode, TVirtualGeoTrack *parent=0, TObject *particle=0);
   virtual ~VMCTrack();
 
   virtual Int_t       AddPointAttachment(Int_t i, Double_t edep, Double_t step);
   virtual Int_t       GetPointAttachment(Int_t i, Double_t &edep, Double_t &step) const;

   //ClassDef(VMCTrack, 1)     // geometry tracks class with edep and step information
};

#endif
