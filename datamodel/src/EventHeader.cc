#include "EventHeader.h"

using namespace std;

ClassImp(EventHeader)

EventHeader::EventHeader()
{
  EH_EventNumber=0;
  EH_TrueP=0.;
 
  for (int i=0; i<3; i++) {
    EH_TrueMomentum[i]=0; 
  }  
  EH_TrueTheta=-999.;
  EH_TruePhi=-999.;
  EH_TrueProdX=-999.;
  EH_TrueProdY=-999.;
  EH_TrueProdZ=-999.;

  EH_TrueTotDepEne=0.;
  EH_TrueTotDepEneQ=0.;
  EH_TrueTotDepEEne=0.;
  EH_TrueTotDepEEneQ=0.;
  EH_TrueTotDepHEne=0.;
  EH_TrueTotDepHEneQ=0.;
  EH_TrueEneFromBeam=0.;
  EH_TrueEneFromCRBack=0.;
 
  EH_TrueEneLost = 0.;
  EH_TrueTotDepEneCRT1=0.;
  EH_TrueTotDepEneCRT2=0.;

  EH_RecTheta=-999.;
  EH_RecPhi=-999.;
  EH_RecProdX=-999.;
  EH_RecProdY=-999.;
  EH_RecProdZ=-999.;

  EH_RecTotDepEne=0.;
  EH_RecTotDepEneQ=0.;
  EH_RecTotDepEEne=0.;
  EH_RecTotDepEEneQ=0.;
  EH_RecTotDepHEne=0.;
  EH_RecTotDepHEneQ=0.;  

  EH_DQFlag = false;

  EH_BCh1[0] = EH_BCh1[1] = -999;
  EH_BCh2[0] = EH_BCh2[1] = -999;
  EH_EvTof = -999;
  EH_EvCherenkov[0] =  EH_EvCherenkov[1] =  EH_EvCherenkov[2] = -999;
  
  for (int ncrm=0; ncrm<MAXCRM; ncrm++) {
    EH_nhitv0[ncrm]=0;
    EH_nhitv1[ncrm]=0;
  }

  EH_crptrks0 = 0;
  EH_crptrks1 = 0;
  
  // list of primaries
  EH_Primaries = NULL;
}

EventHeader::~EventHeader()
{
  if(EH_Primaries) 
    delete EH_Primaries;
}


void EventHeader::DumpEventHeader_True() 
{

  cout <<"========================================="<<endl;
  cout <<"            EVENT HEADER DUMP           "<<endl;
  cout <<"========================================="<<endl;

  cout <<"EventNumber "<<EH_EventNumber<<endl;
  cout <<"EventDQFlag "<<EH_DQFlag<<endl;
  cout <<"Event time  "<<EH_EventTime.AsString()<<endl;
  cout <<"Momentum    "<<EH_TrueP <<endl;
  cout <<"px, py, pz  "<<EH_TrueMomentum[0]<<"  "<<EH_TrueMomentum[1]<<"  "<<EH_TrueMomentum[2]<<endl;
  cout <<"Theta       "<<EH_TrueTheta <<endl;
  cout <<"Phi         "<<EH_TruePhi<<endl;
  cout <<"production vertex  (x,y,z)             "<<EH_TrueProdX<<"  "<<EH_TrueProdY<<"  "<<EH_TrueProdZ<<endl;  
  cout <<"Total deposited energy                 "<<EH_TrueTotDepEne<<"  quenched  "<<EH_TrueTotDepEneQ<<endl;   
  cout <<"Total deposited em energy              "<<EH_TrueTotDepEEne<<"  quenched  "<<EH_TrueTotDepEEneQ<<endl; 
  cout <<"Total deposited had energy             "<<EH_TrueTotDepHEne<<"  quenched  "<<EH_TrueTotDepHEneQ<<endl; 
  cout <<"Energy Deposited by Beam Particle      "<<EH_TrueEneFromBeam<<endl;                                    
  cout <<"Energy Deposited by CR background       " <<EH_TrueEneFromCRBack<<endl;                                
  cout <<"Energy lost when exiting active volume "<<EH_TrueEneLost<<endl;                                        
  cout <<"Energy lost in CRT1                    "<<EH_TrueTotDepEneCRT1<<endl;                                  
  cout <<"Energy lost in CRT2                    "<<EH_TrueTotDepEneCRT2<<endl;                                  
  cout <<"Edep + Elost                           "<<(EH_TrueEneLost + EH_TrueTotDepEne)<<endl;                   

  // print list of primaries in MC
  if(EH_Primaries) 
    {
      cout<<"Primary particles saved (MC only) "<<endl;
      TIter iter(EH_Primaries);
      TParticle *p;
      while( (p = (TParticle*)iter.Next() ) )
	{
	  p->Print();
	  cout<<"           start time : "<<p->T()<<endl;
	}
    }
  cout <<"========================================="<<endl;
  cout <<"        END EVENT HEADER DUMP           "<<endl;
  cout <<"========================================="<<endl<<endl;
}

// add primary
void EventHeader::SetPrimary( TParticle *p )
{
  if(!EH_Primaries) 
    {
      EH_Primaries = new TObjArray();
      EH_Primaries->SetOwner(true);
    }
  
  EH_Primaries->Add( new TParticle( *p ) );
}

// clone the array
void EventHeader::SetPrimaryArray( TObjArray *arr )
{
  if(EH_Primaries)
    delete EH_Primaries;
  
  EH_Primaries = (TObjArray*)arr->Clone();
  EH_Primaries->SetOwner(true);
}
