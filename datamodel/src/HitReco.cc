#include "HitReco.h"  

using namespace std;

ClassImp(HitReco) 

HitReco::HitReco() {
  
  
  for (int i=0; i<MAXHITRFLVL; i++)  HITRec_PFloat[i]=0;
  for (int i=0; i<MAXHITRINTVL; i++)  HITRec_PInt[i]=0;
  for (int i=0; i<MAXHITRSTRVL; i++)  HITRec_PString[i]="";
  
}
/*----------------------------------------------------------------------------------------*/
void HitReco::DumpHitReco() 
{
  
  cout <<"-----------Hit Reconstruction parameter ----------------"<<endl;
  
  cout<<"   "<<HITRec_PFloat[0]<<"  "<<HITRec_PFloat[1]<<endl;
  cout<<"   "<<HITRec_PFloat[2]<<"  "<<HITRec_PFloat[3]<<endl;
  cout<<"   "<<HITRec_PFloat[4]<<endl;
  cout<<"   "<<HITRec_PInt[0]<<"  "<<HITRec_PInt[1]<<endl;
  
  cout <<"ROI: "<<endl;
  cout<<"   "<<HITRec_PFloat[5]<<"  "<<HITRec_PFloat[6]<<"  "<<HITRec_PFloat[7]<<"  "<<HITRec_PFloat[8]<<endl;
  cout<<"   "<<HITRec_PInt[2]<<"  "<<HITRec_PInt[3]<<endl;
  cout<<"  if AlgoTDeconHit   "<<HITRec_PFloat[10]<<endl;
 
}
 
void HitReco::CheckHitFNum (int fnum)
{
  if( fnum >= MAXHITRFLVL )
    {
      cerr<<"ERROR: number of FLOAT parameter "<<fnum<<"  GT max number allowed "<<MAXHITRFLVL<<endl;
      exit(-1);
    }
}
 
 void HitReco::CheckHitINum (int inum)
{
  if( inum >= MAXHITRINTVL )
    {
      cerr<<"ERROR: number of INT parameter "<<inum<<"  GT max number allowed "<<MAXHITRINTVL<<endl;
      exit(-1);
    }
}

void HitReco::CheckHitSNum (int snum)
{
  if( snum >= MAXHITRINTVL )
    {
      cerr<<"ERROR: number of STRING  "<<snum<<"  GT max number allowed "<<MAXHITRINTVL<<endl;
      exit(-1);
    }
}
