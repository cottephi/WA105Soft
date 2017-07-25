////////////////////////////////////////////////////////////////////////////////////////////
//
//  Hit reconstructio task
//  Add here you algorithms (see InitHitFinder function)
//  
//  Created: Fri Apr 29 14:30:22 CEST 2016
//
//
////////////////////////////////////////////////////////////////////////////////////////////

#include "HitRecoTask.h"

#include "LiquidArgonMedium.h"

//
// ctor
//
HitRecoTask::HitRecoTask()
{
  // no default value

  // need to configure
  fHitFinder = NULL;
  fRoiFinder = NULL;
  
  fElecMod   = NULL;

  fHasTruth = false;
  
  fVdrift = 0.0;
  
  // calibration data
  fChPed  = NULL;
  
  // namess of data branches for this task
  fDataProds.push_back("RecoHitsView0_Data");
  fDataProds.push_back("RecoHitsView1_Data");
}


//
// dtor
//
HitRecoTask::~HitRecoTask()
{
  if(fHitFinder) delete fHitFinder;
  if(fRoiFinder) delete fRoiFinder;
  if(fElecMod) delete fElecMod;
}


//
//
//
void HitRecoTask::Configure(RunConfig &runpara, RecoConfig &recopara)
{
  
  // get CRP readout config
  std::string elename = runpara.GetCRPConfig()->GetGC_ElecModel();
  InitElecModule(elename);

  // configuration parameters
  fRecConf = recopara;

  // hit algorithm
  InitHitFinder(fRecConf.GetAlgoName());
  
  if(!fRoiFinder) fRoiFinder = new RawChDataROI();
  std::vector<float> params = fRecConf.GetValue("PARA_ROI");
  
  fROI_adcTh    = params[0];
  fROI_nSigTh   = params[1];
  fROI_nSigLow1 = params[2];
  fROI_nSigLow2 = params[3];
  fROI_padLeft  = (UInt_t)params[4];
  fROI_padRight = (UInt_t)params[5];

  fRoiFinder->SetNSigTh(fROI_nSigTh);   
  fRoiFinder->SetNSigLow1(fROI_nSigLow1); 
  fRoiFinder->SetNSigLow2(fROI_nSigLow2); 
  fRoiFinder->SetNPadRight(fROI_padLeft);
  fRoiFinder->SetNPadLeft(fROI_padRight); 

  
  std::vector<float> paracali = fRecConf.GetValue("PARA_CALI");
  if( !paracali.empty() )
    {
      //if( paracali[0] > 0 ) fChMask  = &calidaq::ChMask::Instance();
      if( paracali[1] > 0 ) fChPed = &calidaq::ChPedestal::Instance();
      else fChPed = NULL;
      //if( paracali[2] > 0 ) fChCali  = &calidaq::ChCalib::Instance();      
    }

}


//
//
//
void HitRecoTask::ProcessEvent(Event &event)
{
  //
  const LArRawData::DataCRP* dat_crp = event.GetCRPData();
  if(!dat_crp)
    {
      cerr<<"ERROR: no CRP raw data found "<<endl;
      return;
    }

  // get the truth information if any
  ReadVoxelInfo( event );

  // drift velocity for nominal field
  double Efield = event.GetRunHeader()->GetEfieldValue(); // in V/cm
  fVdrift = 0.1 * LiquidArgonMedium::Instance()->DriftVelocity(1.0E-3*Efield); //in cm/us

  const GeomConfig*  geo_config = event.GetGeoConfig();
  if(!geo_config) 
    {
      std::cerr<<"ERROR: no geometry configuration "<<std::endl;
      return;
    }


  std::vector<LArReco::QHit> hits_view0;
  std::vector<LArReco::QHit> hits_view1;
  
  size_t numhits0 = 0;
  size_t numhits1 = 0;
  
  // loop over CRMs
  TIter next_crm(dat_crp->GetCRMs());
  LArRawData::DataCRM *crm;
  while ( (crm = (LArRawData::DataCRM*) next_crm()) ) 
    {
      
      cout<<endl<<" CRM : "<<crm->GetCRMId()<<endl;
      cout<< "  Num channels in view 0 "<< crm->GetNChanView0() <<endl;
      cout<< "  Num channels in view 1 "<< crm->GetNChanView1() <<endl;

      FindHitsView(crm, 0, hits_view0, geo_config);
      numhits0 += hits_view0.size();
      cout  <<"    Hits in view 0 in this CRM "<<hits_view0.size()<<endl;
      cout  <<"    Total hits in view 0       "<<numhits0<<endl;
      FindHitsView(crm, 1, hits_view1, geo_config);
      numhits1 += hits_view1.size();
      cout  <<"    Hits in view 1 in this CRM "<<hits_view1.size()<<endl;
      cout  <<"    Total hits in view 1       "<<numhits1<<endl;
 
      //fill event header	      
      // NOTE: the event header will only update if making a new tree
      event.GetEventHeader()->SetNumRecHit0( crm->GetCRMId(), (int)hits_view0.size() );
      event.GetEventHeader()->SetNumRecHit1( crm->GetCRMId(), (int)hits_view1.size() );

      // set MC true
      AssignHitTruth( hits_view0, hits_view1 );
      
      // add new reco data to event
      event.Add(fDataProds[0], hits_view0);
      event.Add(fDataProds[1], hits_view1);

      hits_view0.clear();
      hits_view1.clear();
    } 
}


//
// find hits in a given view
//
void HitRecoTask::FindHitsView( LArRawData::DataCRM* crm, int iview, 
				std::vector<LArReco::QHit> &hits,
				const GeomConfig* geo_config )
{
  TClonesArray *vData = NULL;

  if(iview == 0)
    vData = crm->GetChanView0();
  else
    vData = crm->GetChanView1();

  LArRawData::DataChan *ch = NULL;  
  TIter next(vData); 
  while ( (ch = (LArRawData::DataChan*)(next())) )
    {
      size_t isz = hits.size();
      float ped    = 0;
      float pedrms = 0;
      // get the pedestal for this channel
      if( fChPed )
	{ 
	  ped = fChPed->Pedestal( crm->GetCRMId(), iview, ch->GetChanId(), pedrms );
	}

      FindHitsCh(ch, hits, ped, pedrms); 
      for(size_t ah=isz;ah<hits.size();ah++)
	{
	  hits[ah].SetCRM( crm->GetCRMId() );
	  hits[ah].SetView( iview );

	  hits[ah].SetCh( ch->GetChanId() );
	  
	  // convert hit time to us
	  hits[ah].SetT( geo_config->GetDriftCoord(hits[ah].GetTbStart()) );

	  // get spatial coordinate for this channel
	  hits[ah].SetX( geo_config->GetCrpCoord(hits[ah].GetCRM(),
						 hits[ah].GetView(),
						 hits[ah].GetCh()) );
	  hits[ah].SetD( fVdrift * hits[ah].GetT() );
	}
    }
}



//
// find hits
//
void HitRecoTask::FindHitsCh( LArRawData::DataChan *chdata, 
			      std::vector<LArReco::QHit> &hits,
			      float ped, float pedrms)
{
  // get data fragments
  size_t nfrags = 0;
  std::vector< LArRawData::DataFragment<Float_t> > myROIs;
  // get pedestal and ROIs
  if( chdata->GetChanStatus() == LArRawData::FULLDATA )
    {
      if( ped == 0 && pedrms == 0 )  // try to find pedestal ourselves
	nfrags = fRoiFinder->FindROI(chdata, myROIs, fROI_adcTh, true);
      else
	nfrags = fRoiFinder->FindROI(chdata, myROIs, ped, pedrms, true);
    }
  else if( chdata->GetChanStatus() == LArRawData::ZSDATA ) 
    {
      TClonesArray *frgs = chdata->GetDataFragments();
      nfrags = chdata->GetNFrags();
      // push it to the vector (should change at some point)
      for(size_t i=0;i<nfrags;i++)
	{
	  myROIs.push_back( *((LArRawData::DataFragment<Float_t>*)frgs->At(i)) );
	}
    }
  else
    {
      cerr << "ERROR : cannot treat this datatype to find hits" << endl;
    }
  
  if( nfrags == 0)
    {
      return;
    }
  
  float ped_ch  = chdata->GetPed();
  float ped_rms = chdata->GetPedRMS();
  
  // loop over fragments
  for(size_t i=0;i<myROIs.size();i++)
    {
      LArRawData::DataFragment<Float_t> *Frag = &myROIs[i];

      std::vector<float> roidata;
      UInt_t fragstrt = Frag->GetData(roidata);
      
      // find hits
      fHitFinder->FindHits(fDt, fragstrt, roidata, ped_ch, ped_rms, false);
   
      
      std::vector<LArReco::QHit*> htemp = fHitFinder->GetHits();
      // add hits to the list
      for(size_t i=0;i<htemp.size();i++)
	{
	  hits.push_back(*htemp[i]);
	  //hits.back().SetCh(chdata->GetChanId());
	}
    }
  
  //myROIs.clear();
  return;
}
	    
 
//
// get electronics properties
//
void HitRecoTask::InitElecModule(std::string& name)
{
  if(fElecMod) delete fElecMod;
  fElecMod = NULL;
  
  fPreampCalib = 0;
  fAdcCalib    = 0;
  fDt          = 0;
  fResp.clear();
  fRespADC.clear();

  if(name.compare("ETHZ") == 0)
    {
      fElecMod = new DigitizerETHZ(); 
    }
  else
    {
      std::cerr<<"ERROR: Uknown elecsim module "<<name<<std::endl;
      return;
    }
  
  fPreampCalib = fElecMod->GetCalibConst();
  fAdcCalib    = fElecMod->GetmVtoADC();
  fDt          = 1.0/fElecMod->GetSampleFreq(); //in us
  
  fElecMod->ComputeRespVector();
  fElecMod->GetRespVector(fResp);

  for(size_t i=0;i<fResp.size();i++)
    fRespADC.push_back(fResp[i]/fAdcCalib);
}
 

//
// initialize hit finder algorithm
//
void HitRecoTask::InitHitFinder(std::string& name)
{
  if(fHitFinder) delete fHitFinder;
  fHitFinder = NULL;

  if( name.compare("AlgoMultiHit") == 0 )
    {
      std::vector<float> params = fRecConf.GetValue("PARA_HIT");
      fMultiHit_relTh1   = params[0];
      fMultiHit_relTh2   = params[1];
      fMultiHit_absTh1   = params[2];
      fMultiHit_absTh2   = params[3];
      fMultiHit_aboveT   = params[4];
      fMultiHit_padLeft  = (UInt_t)params[5];
      fMultiHit_padRight = (UInt_t)params[6];

      fHitFinder = new AlgoMultiHit(fPreampCalib/fAdcCalib, fMultiHit_relTh1, fMultiHit_relTh2, 
				    fMultiHit_absTh1, fMultiHit_absTh2, fMultiHit_aboveT/fDt,
				    fMultiHit_padLeft, fMultiHit_padRight);
    }
  else if ( name.compare("AlgoTDeconHit") == 0 )
    {
      std::vector<float> params = fRecConf.GetValue("PARA_HIT");
      fTDecon_hitTh = params[0];
      fHitFinder = new AlgoTDeconHit(fRespADC, fTDecon_hitTh);
    }
  else
    {
      std::cerr<<"ERROR: Uknown hit algorithm "<<name<<std::endl;
      exit(-1);
    }
}


//
// assign hit truth
//
void HitRecoTask::AssignHitTruth(std::vector<LArReco::QHit> &hits0,
				 std::vector<LArReco::QHit> &hits1)
{
  if(!fHasTruth) return;
  
  float difft = 0;
	   
  // assign HitIndex and associate and set   MC true
  int nas;
  float qtot; 
  float qdiff, currqdiff;
  float qmchit;
  int idx;

  for (int ah=0; ah<(int)hits0.size(); ah++) 
    {
      //LArReco::QHit* ahit = &hits0[ah]; 

      int crmn = hits0[ah].GetCRM();
      int chnn = hits0[ah].GetCh();
      
      nas    = 0;
      qdiff  = 99999;
      qmchit = 0;
      //hits0[ah].SetHitIndex(ah);  
      
      
      //loop on MC_hits
      for (int mch=0; mch<MC_HIT0[crmn] [chnn][0] [0]; mch++) 
	{  
	  difft= hits0[ah].GetTbStart()- MC_HIT0 [crmn] [chnn][mch] [1];  
	  qtot=0;
	  
	  if (abs(difft) <10) 
	    {  
	      nas++; 
	      for (int e=2; e<8; e++) qtot+=MC_HIT0[crmn][chnn][mch][e];
	      
	      currqdiff=abs(qtot-hits0[ah].GetQ())/qtot; 
	      if (currqdiff <qdiff) 
		{ 
		  qdiff = currqdiff;
		  idx = mch; 
		  qmchit = qtot; 
		}
	    } 	// if (abs(difft) <10) {    
	  //
	}//int nn=1; nn<=MC_HIT0 [hits0[ah].GetCRM()] [hits0[ah].GetCh()][0] [0]; nn++)   
	     
      //		 if (nas>0) {
  
      MC_HIT0[crmn][chnn][idx][1] = -99999;   ////MC_hit already associated
      for (int nq=0; nq<6; nq++) 
	{
	  hits0[ah].SetHitMCCharge(nq,MC_HIT0[crmn][chnn][idx][2+nq]);
	}
    }//  for (int ah=crmcounter0; ah<hitsView0.GetEntries(); ah++) {  


  for (int ah=0; ah<(int)hits1.size(); ah++) 
    {
      int crmn = hits1[ah].GetCRM();
      int chnn = hits1[ah].GetCh();
      
      nas    = 0;
      qdiff  = 99999;
      qmchit = 0;
	     
      // and  MC true	  
      //loop on MC_hits
      for (int mch=0; mch<MC_HIT1[crmn][chnn][0][0]; mch++) {  
	difft= hits1[ah].GetTbStart()- MC_HIT1 [crmn] [chnn][mch] [1];  
	qtot=0;
	
	if (abs(difft) <10) {  
	  nas++; 
	  for (int e=2; e<8; e++) qtot+=MC_HIT1 [crmn] [chnn][mch] [e];
	  
	  currqdiff=abs(qtot-hits1[ah].GetQ())/qtot; 
	
	  if (currqdiff <qdiff) { 
	    qdiff=currqdiff;
	    idx= mch; 
	    qmchit=qtot; 
	  }
	  
	} 	// if (abs(difft) <10) {    
	//		   
      }//int nn=1; nn<=MC_HIT1 [hits1[ah].GetCRM()] [hits1[ah].GetCh()][0] [0]; nn++)   
	      
      MC_HIT1[crmn][chnn][idx] [1]=-99999;   ////MC_hit already associated
      for (int nq=0; nq<6; nq++) {
	hits1[ah].SetHitMCCharge(nq,MC_HIT1 [crmn] [chnn][idx] [2+nq]);
      } 
    }//  for (int ah=crmcounter1; ah<hitsView0.GetEntries(); ah++) {  
}


//
// function to get the true charge
//
void HitRecoTask::ReadVoxelInfo(Event &event)
{
  fHasTruth = false;
  const LArVoxel::VoxelInfoCRP* vxl_crp = event.GetVoxelInfo();
  if(!vxl_crp) return; // no truth

  fHasTruth = true;
  
  //std::cout<<"Found voxel info "<<std::endl;

  //float x_CRM[MAXCRM],y_CRM[MAXCRM],z_CRM[MAXCRM];
  //float pitchx_CRM[MAXCRM],pitchy_CRM[MAXCRM];
  //int nchax_CRM[MAXCRM],nchay_CRM[MAXCRM], nsample_CRM[MAXCRM]; 
  //float pitchT;
  
  const GeomConfig* geo_config = event.GetGeoConfig();
  if(!geo_config) return;

  // CRP properties
  int ncrm     = geo_config->GetCRMnum();
  float pitchT = geo_config->GetCRMpitchT();

  std::vector<float> x_CRM, y_CRM;
  std::vector<float> pitchx_CRM, pitchy_CRM;
  std::vector<int> nchax_CRM, nchay_CRM, nsample_CRM;

  int chXMax, chYMax;   
 
  // resize arrays
  x_CRM.resize( ncrm );
  y_CRM.resize( ncrm );
  pitchx_CRM.resize( ncrm );
  pitchy_CRM.resize( ncrm );
  nchax_CRM.resize( ncrm );
  nchay_CRM.resize( ncrm );
  nsample_CRM.resize( ncrm );
  
  // get CRP properties
  chXMax = chYMax = 0;
  for (int nn=0; nn<ncrm; nn++) 
    {
      // center
      x_CRM[nn] = geo_config->GetCRMcX(nn);
      y_CRM[nn] = geo_config->GetCRMcY(nn);
      
      // pitch
      pitchx_CRM[nn] = geo_config->GetCRMpitchX(nn);
      pitchy_CRM[nn] = geo_config->GetCRMpitchY(nn);

      // number of channels in one view
      nchax_CRM[nn] = geo_config->GetCRMnchaX(nn);
      if (nchax_CRM[nn]>chXMax) 
	chXMax = nchax_CRM[nn]; 
      
      // number of channels in the other view
      nchay_CRM[nn]=geo_config->GetCRMnchaY(nn);
      if (nchay_CRM[nn]>chYMax) 
	chYMax = nchay_CRM[nn];
      
      // number of time samples
      nsample_CRM[nn]=geo_config->GetCRMnsample(nn);
    }  

  //int nmaxhit = 500;
  
  // view 0
  /*
  float truech0 [chXMax]  [nmaxhit];
  int   timebin0  [chXMax]  [nmaxhit]; 
  int   pid0 [chXMax][nmaxhit];  
  int   touchedch0 [chXMax];
  */
  std::vector< std::vector<float> > truech0(chXMax, std::vector<float>());
  std::vector< std::vector<int> > timebin0(chXMax, std::vector<int>());
  std::vector< std::vector<int> > pid0(chXMax, std::vector<int>());
  std::vector< int > touchedch0(chXMax);

  // view 1
  /*
  float truech1 [chYMax]  [nmaxhit];
  int   timebin1  [chYMax]  [nmaxhit]; 
  int   pid1 [chYMax] [nmaxhit];  
  int   touchedch1 [chYMax];
  */
  std::vector< std::vector<float> > truech1(chYMax, std::vector<float>());
  std::vector< std::vector<int> > timebin1(chYMax, std::vector<int>());
  std::vector< std::vector<int> > pid1(chYMax, std::vector<int>());
  std::vector< int > touchedch1(chYMax);
  
  
  // int gap=4;
  int gap=6;
  // int gap=2;
  // int gap=8;
  // int gap=10;
  
  int icrm=-1; 
  
  
  TObjArray *vCRM;
  TObjArray *Voxels;  
  
  LArVoxel::VoxelInfoCRM* vxlcrm=0;
  vCRM = vxl_crp->GetVoxelCRMs(); 
  TIter next(vCRM);
  
  while( (vxlcrm = (LArVoxel::VoxelInfoCRM*)next()) )
    {  
      int crmid = vxlcrm->GetCRMId();
      if (crmid>icrm) 
	{
	  icrm=crmid;
	  for (int nc=0; nc<chXMax; nc++) 
	    {
	      touchedch0[nc]=0; 
	      truech0[nc].clear();
	      timebin0[nc].clear();
	      pid0[nc].clear();
	    }	  
	  for (int nc=0; nc<chYMax; nc++) 
	    { 
	      touchedch1[nc]=0;
	      truech1[nc].clear();
	      timebin1[nc].clear(); 
	      pid1[nc].clear();   
	    }	  

	} 

      // projection object
      QProjectorCRM qproj( x_CRM[crmid], y_CRM[crmid], 
			   pitchx_CRM[crmid], pitchy_CRM[crmid],pitchT, 
			   (UInt_t) nchax_CRM[crmid], (UInt_t) nchay_CRM[crmid],
			   (UInt_t) nsample_CRM[crmid] );
      
      LArVoxel::VoxelInfo* vxl=0;
      Voxels=vxlcrm->GetVoxels(); 
      TIter next(Voxels);
      //voxel information
      
      int currCHX=-1;
      int currCHY=-1;
      int currpidx=-9999;
      int currpidy=-9999;
      int currtimex=-9999;
      int currtimey=-9999;
      float chargex=0;
      float chargey=0;
      int diffx=-999;
      int diffy=-999;
      
      while( (vxl = (LArVoxel::VoxelInfo*)next()) )
	{
            
	  
	  // calculate projection of charge on the anode
	  qproj.Projection( vxl->GetXCrp(), vxl->GetYCrp(), vxl->GetTCrp(), vxl->GetQCrp(),
			    vxl->GetSigmaL(), vxl->GetSigmaT() );
	     	
	  
	  if (currCHX==-1) 
	    {
	      chargex   = 0;
	      currCHX   = qproj.GetChNum(vxl->GetXCrp(),0); 
	      currtimex = qproj.GetTimeBin(vxl->GetTCrp());
	      currpidx  = vxl->GetPid(); 
	  }   
	
	  if (currCHY==-1) 
	    {
	      chargey   = 0;
	      currCHY   = qproj.GetChNum(vxl->GetYCrp(),1); 
	      currtimey = qproj.GetTimeBin(vxl->GetTCrp());
	      currpidy  = vxl->GetPid(); 
	    }  
	
	
		
	  diffx = abs (qproj.GetTimeBin(vxl->GetTCrp()) - currtimex); 
	  diffy = abs (qproj.GetTimeBin(vxl->GetTCrp()) - currtimey); 	
	  
	      
	
	  //view 0 
	  
	  if (qproj.GetChNum(vxl->GetXCrp(),0) == currCHX && 
	      (diffx==0) &&  vxl->GetPid()==currpidx) 
	    {
	      chargex += vxl->GetQCrp();  
	    }  //if
	  else  
	    {   
	      touchedch0[currCHX] += 1; //number of "hits" for this channel 
	      truech0[currCHX].push_back(chargex);
	      timebin0[currCHX].push_back(currtimex);
	      pid0[currCHX].push_back(currpidx);
	      
	      chargex   = vxl->GetQCrp();
	      currCHX   = qproj.GetChNum(vxl->GetXCrp(),0); 
	      currtimex = qproj.GetTimeBin(vxl->GetTCrp());
	      currpidx  = vxl->GetPid();    
	    }   //else    
	  
	
	  //view 1
	  
	  if (qproj.GetChNum(vxl->GetYCrp(),1) == currCHY && 
	      (diffy==0) &&  vxl->GetPid()==currpidy) 
	    {
	      chargey+=vxl->GetQCrp();   
	    }   //if
	  else  
	    {    
	      touchedch1 [currCHY] += 1; //number of "hits" for this channel   
	      truech1[currCHY].push_back(chargey); 
	      timebin1[currCHY].push_back(currtimey); 
	      pid1[currCHY].push_back(currpidy); 


	      chargey   = vxl->GetQCrp();
	      currCHY   = qproj.GetChNum(vxl->GetYCrp(),1); 
	      currtimey = qproj.GetTimeBin(vxl->GetTCrp());
	      currpidy  = vxl->GetPid();     
	    }   //else   		    
	} //while( (vxl = (LArVoxel::VoxelInfo*)next()) ) 

      
  
      for(int nc=0; nc<chXMax; nc++) {
	for (int nf=0; nf<20; nf++) {
	  for (int nv=0; nv<8; nv++) {
	    MC_HIT0 [crmid][nc] [nf] [nv]=0;  
	  }}}     
      for(int nc=0; nc<chYMax; nc++) {
	for (int nf=0; nf<20; nf++) {
	  for (int nv=0; nv<8; nv++) { 
	    MC_HIT1 [crmid][nc] [nf] [nv]=0; 
	  }}}        
      
      
      // now build MC_hits for view0   
      
      int ntot0=0;
      int ntot1=0;
      
      
      for (int ch=0; ch<chXMax; ch++) {  //loop on channels
	std::vector<int> r0time;//(touchedch1[ch]);
	       
	for (int nh=0; nh<touchedch0[ch]; nh++)  
	  r0time.push_back(timebin0[ch][nh]); 
	
	//time sorting	     
	std::sort(r0time.begin(), r0time.end()); 
	
	//for (int nh=0; nh<touchedch0[ch]; nh++)  r0time[nh]=timebin0  [ch]  [nh];  
	//time sorting	     
	//std::sort(r0time, r0time+touchedch0[ch]);  
	
	int nf=0; 
	int TBin_ref=-1;  
	
	for (int rt=0; rt<touchedch0[ch]; rt++) { 
	  int tocompare=r0time[rt];  
	  for (int nh=0; nh<touchedch0[ch]; nh++) { 
	    if (timebin0 [ch][nh]==tocompare) { 
	      
	      //found element in the list, check TBin
	      if ((timebin0 [ch] [nh]- TBin_ref)<=gap ){	  
		if  ( abs (pid0 [ch] [nh]) == 13 ) { MC_HIT0[crmid][ch]  [nf] [2]+=truech0 [ch] [nh] ;} 
		else if  ( abs (pid0 [ch] [nh]) ==  11) { MC_HIT0 [crmid][ch]  [nf] [3]+=truech0 [ch] [nh] ;}
		else if  (  pid0 [ch]  [nh]  ==  22) { MC_HIT0 [crmid][ch] [nf] [4]+=truech0 [ch]  [nh] ;}
		else if  (  pid0 [ch]  [nh]  == 111)  {MC_HIT0 [crmid][ch] [nf] [5]+=truech0 [ch]  [nh] ;}
		else if  (  abs (pid0 [ch] [nh])   == 211) { MC_HIT0 [crmid][ch] [nf] [6]+=truech0 [ch]  [nh] ;}
		else { MC_HIT0 [crmid][ch] [nf] [7]=truech0 [ch]  [nh] ;}
		
		
		TBin_ref=timebin0 [ch]  [nh] ; 
		timebin0  [ch] [nh]=-1; 
	      }  
	      else
		{  
		  nf++;	
		  MC_HIT0 [crmid][ch] [0] [0]=nf;
		  
		  MC_HIT0 [crmid][ch] [nf] [1]=timebin0 [ch]  [nh] ; 
		  if  ( abs (pid0 [ch] [nh]) == 13 ) { MC_HIT0[crmid][ch][nf] [2]=truech0 [ch] [nh] ;} 
		  else if  ( abs (pid0 [ch] [nh]) ==  11) { MC_HIT0 [crmid][ch] [nf] [3]=truech0 [ch] [nh] ;}
		  else if  (  pid0 [ch] [nh]  ==  22) { MC_HIT0 [crmid][ch] [nf] [4]=truech0 [ch] [nh] ;}
		  else if  (  pid0 [ch] [nh]  == 111)  {MC_HIT0 [crmid][ch] [nf] [5]=truech0 [ch]  [nh] ;}
		  else if  (  abs (pid0 [ch] [nh])   == 211) { MC_HIT0 [crmid][ch] [nf] [6]=truech0 [ch] [nh] ;}
		  else { MC_HIT0 [crmid][ch] [nf] [7]=truech0 [ch] [nh] ;}
		  TBin_ref=timebin0 [ch] [nh] ;
		  timebin0  [ch] [nh]=-1;   
		  
		}  //else 
	    }//tocompare
	    
	  } //nh
	  
	} //rt 
		   
	ntot0+=MC_HIT0 [crmid][ch] [0] [0];  
				 
	
  
 
      }  //  for(int nc=0; nc<chXMax; nc++)
      
      // now build MC_hits for view1   

      
      
      for (int ch=0; ch<chYMax; ch++) {  //loop on channels 
	//int r1time[touchedch1[ch]];    
	std::vector<int> r1time;//(touchedch1[ch]);
	       
	for (int nh=0; nh<touchedch1[ch]; nh++)  
	  r1time.push_back(timebin1 [ch][nh]); 
	
	//time sorting	     
	std::sort(r1time.begin(), r1time.end()); 
	
	int nf1=0; 
	int TBin1_ref=-1;  
	
	for (int rt=0; rt<touchedch1[ch]; rt++) {
	  
	  int tocompare=r1time[rt];  
	  
	  for (int nh=0; nh<touchedch1[ch]; nh++) {
	    
	    if (timebin1 [ch] [nh]==tocompare) { 
	      
	      //found element in the list, check TBin
	      if ((timebin1 [ch] [nh]- TBin1_ref)<=gap ){	  
		
		if  ( abs (pid1 [ch]  [nh]) == 13 ) { MC_HIT1 [crmid][ch] [nf1] [2]+=truech1 [ch] [nh] ;} 
		else if  ( abs (pid1 [ch] [nh]) ==  11) { MC_HIT1 [crmid][ch] [nf1] [3]+=truech1 [ch] [nh] ;}
		else if  (  pid1 [ch]  [nh]  ==  22) { MC_HIT1 [crmid][ch] [nf1] [4]+=truech1 [ch] [nh] ;}
		else if  (  pid1 [ch]  [nh]  == 111)  {MC_HIT1 [crmid][ch] [nf1] [5]+=truech1 [ch] [nh] ;}
		else if  (  abs (pid1 [ch]  [nh])   == 211) { MC_HIT1 [crmid][ch] [nf1] [6]+=truech1 [ch] [nh] ;}
		else { MC_HIT1 [crmid][ch] [nf1] [7]=truech1 [ch] [nh] ;}
			     
			     
		TBin1_ref=timebin1 [ch] [nh] ; 
		timebin1  [ch]  [nh]=-1; 
	      }  
	      else
		{  
		  nf1++;	
		  MC_HIT1 [crmid][ch]  [0] [0]=nf1;
			     
		  MC_HIT1 [crmid][ch]  [nf1] [1]=timebin1 [ch] [nh] ; 
		  if  ( abs (pid1[ch]  [nh]) == 13 ) { MC_HIT1 [crmid][ch] [nf1] [2]=truech1 [ch]  [nh] ;} 
		  else if  ( abs (pid1 [ch] [nh]) ==  11) { MC_HIT1 [crmid][ch] [nf1] [3]=truech1 [ch] [nh] ;}
		  else if  (  pid1 [ch] [nh]  ==  22) { MC_HIT1 [crmid][ch] [nf1] [4]=truech1 [ch] [nh] ;}
		  else if  (  pid1 [ch] [nh]  == 111)  {MC_HIT1 [crmid][ch] [nf1] [5]=truech1 [ch]  [nh] ;}
		  else if  (  abs (pid1 [ch] [nh])   == 211) { MC_HIT1 [crmid][ch] [nf1] [6]=truech1 [ch] [nh] ;}
		  else { MC_HIT1 [crmid][ch] [nf1] [7]=truech1 [ch]  [nh] ;}
		  TBin1_ref=timebin1 [ch] [nh] ;
		  timebin1  [ch]  [nh]=-1;   
		  //			     
		}  //else 
	    }//tocompare
		        
	  } //nh
	     
	} //rt	  
	
	ntot1+=MC_HIT1 [crmid][ch] [0] [0]; 
	
      }  //for (int ch=0; ch<chYMax; ch++)
      
    } //while( (vxlcrm = (LArVoxel::VoxelInfoCRM*)next()) ) 	  


  // the end
}
      

     
