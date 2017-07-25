#include "VMCTrack.h"

//ClassImp(VMCTrack)

//______________________________________________________________________________
VMCTrack::VMCTrack()
{
  // Default constructor
  fPointsAttachmentSize = 0;
  fNpointsAttachment    = 0;
  fPointsAttachment     = 0;
}

//______________________________________________________________________________
VMCTrack::VMCTrack(Int_t id, Int_t pdgcode, TVirtualGeoTrack *parent, TObject *particle)
          :TGeoTrack(id,pdgcode,parent,particle)
{
  // Constructor
  fPointsAttachmentSize = 0;
  fNpointsAttachment    = 0;
  fPointsAttachment     = 0;
}

//_____________________________________________________________________________
VMCTrack::VMCTrack(const VMCTrack& other)
                 :TGeoTrack(other)
{
}

//______________________________________________________________________________
VMCTrack::~VMCTrack()
{
// Destructor.
  if (fPointsAttachment) delete [] fPointsAttachment;
}

//______________________________________________________________________________
Int_t VMCTrack::AddPointAttachment(Int_t i, Double_t edep, Double_t step)
{
  //Add extra information (edep and step) to the track point with index i
  //  printf("asdf\n");
  Int_t np = GetNpoints();
  //  printf("AddPoint att %d\n",np);
  if (i<0 || i>=np)
    {
      Error("AddPointAttachment", "point %i not allocated, max. index=%d", i, np-1);
      return -1;
    }
  // NULL pointer
  if (!fPointsAttachment)
    {
      fPointsAttachmentSize = 16;
      fPointsAttachment = new Double_t[fPointsAttachmentSize];
    }
  // allocate memory
  if (2*i+1>=fPointsAttachmentSize)
    {
      //      printf("alloc");
      Double_t *temp = new Double_t[2*2*(i+1)];
      memcpy(temp, fPointsAttachment, fNpointsAttachment*sizeof(Double_t));
      fPointsAttachmentSize *= 2;
      delete [] fPointsAttachment;
      fPointsAttachment = temp;
    }
  fPointsAttachment[2*i] = edep;
  fPointsAttachment[2*i+1] = step;
  
  if (fNpointsAttachment<2*(i+1)) fNpointsAttachment=2*(i+1);
  
  return i;
}

//______________________________________________________________________________
Int_t VMCTrack::GetPointAttachment(Int_t i, Double_t &edep, Double_t &step) const
{
// Get attachments for point i on the track.
   Int_t np = fNpointsAttachment>>1;
   if (i<0 || i>=np) {
      Error("GetPointAttachment", "no attachment for point %i, indmax=%d", i, np-1);
      return -1;
   }
   edep = fPointsAttachment[2*i];
   step = fPointsAttachment[2*i+1];

   return i;
}  
