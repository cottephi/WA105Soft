/**





 **/

#include <cassert>

#include <TString.h>
#include <TFile.h>
#include <TRandom.h>

#include "HEPSystemOfUnits.h"

#include "RootUtils.h"


#include "DLArLRO.h"



using std::cout;
using std::endl;



LArLReadMC::DLArLRO::DLArLRO() 
{


  dataLRO = new LArRawData::DataLRO();

  NPMTs = 0;

  SetNPMTs(NPMTs);

  sampTime = 400;

  h_is_auto = true;

  timeWinNbins = -1;
  timeWinMin = 0;
  timeWinMax = 0;

  electronLifeTime = -1;
  electroLumFactor = 1;
  quantumEff = 1;

}


LArLReadMC::DLArLRO::~DLArLRO() {

  Clear();

}


void LArLReadMC::DLArLRO::Clear() {

  //to be completed

  if(dataLRO) 
    delete dataLRO;
    

}


void LArLReadMC::DLArLRO:: BeginEvent() {

  for(int i=0; i<dataLRO->hList.GetSize(); ++i) {
    
    TH1F* h = (TH1F*)dataLRO->hList.At(i);
  
    if(!h) continue;

    h->Reset();

  }


}


void LArLReadMC::DLArLRO:: EndEvent() {



}


void LArLReadMC::DLArLRO::SetNPMTs(int N) {

  NPMTs = N;

  buildHistos();

}


void LArLReadMC::DLArLRO::SetPMTQuantumEff(float Q) {

  if(Q > 0 && Q<=1 ){

    quantumEff = Q;
    return;
  }

  if(Q > 1 && Q<= 100){
    std::cout<<" Warning "<<__FILE__<<"::"<<__FUNCTION__<<" value Q= "<<Q<<" converted from  percent to [0,1]"<<std::endl;
    quantumEff = Q/100;
    return;

  }else{
    std::cout<<" Warning "<<__FILE__<<"::"<<__FUNCTION__<<" illegal value value Q="<<Q<<" not set "<<std::endl;
    return;    
  }

}


void LArLReadMC::DLArLRO::SetElectronLifeTime(float tau /* ms */) {

  if( tau <= 0. ) {
    //no attenuation
    electronLifeTime = -1;
    return;
  }

  electronLifeTime = tau;

}


void LArLReadMC::DLArLRO::buildHistos() {


  int nbins = -1;
  double tMin = 0;
  double tMax = 0;

  if(h_is_auto) {
   
    nbins = 1;
    tMin = 0;
    tMax = sampTime;

  }else{

    nbins = timeWinNbins;
    tMin = timeWinMin;
    tMax = timeWinMax;  	
    
  }

  //temp
  //put in the TClonesArray after
  for(int i=0; i<NPMTs; ++i) {
      dataLRO->hList.Add( new TH1F(Form("hPMT%i",i),";time [ns]",nbins,tMin,tMax)  ) ;
  }
  



}



void LArLReadMC::DLArLRO::rebinHistos() {

  int nbins = -1;
  double tMin = 0;
  double tMax = 0;

  if(h_is_auto) {
    nbins = 1;
    tMin = 0;
    tMax = sampTime;

 }else{

    nbins = timeWinNbins;
    tMin = timeWinMin;
    tMax = timeWinMax;  	
    
cout<<nbins<<" "<<tMin<<" " <<tMax<<endl;
  }

  for(int i=0; i<dataLRO->hList.GetSize(); ++i) {
      TH1F *h = (TH1F*)dataLRO->hList.At(i);
   
      h->SetBins(nbins,tMin,tMax);

  }

  

}


void LArLReadMC::DLArLRO::SetSamplingTime(double t) {

  sampTime = t;

  h_is_auto = true;

  rebinHistos();

}



void LArLReadMC::DLArLRO::SetTimeWindow(int n, double tmin, double tmax) {

  timeWinNbins = n;
  timeWinMin = tmin;
  timeWinMax = tmax;

  h_is_auto = false;

  rebinHistos();


}






void LArLReadMC::DLArLRO::RegisterBranches(TTree *tree) {

  if(dataLRO)
    tree->Branch("LRORawData", dataLRO->ClassName(), &dataLRO); 


}


bool LArLReadMC::DLArLRO::ImportSet(std::string type, std::string file, std::string tags, std::string histos, int idx0, int idx1) {


  cout<<"LArLReadMC::DLArLRO::ImportSet "<<endl;

  TString Type(type);
  TString File(file);
  TString Tags(tags);
  TString Histos(histos);


  TFile f( File );

  if( Type=="LAr" ) {
    

    spaceMapFld_LAr.SetName(Type);

    spaceMapFld_LAr.ImportSet(f,Tags,Histos,idx0,idx1);

  }
  else if ( Type=="GAr") {
      
     spaceMapFld_GAr.SetName(Type);

     spaceMapFld_GAr.ImportSet(f,Tags,Histos,idx0,idx1);

  }else{

    cout<<" ##### Not a valid type: "<<type<<endl;
    return false;

  }

  return false;
}





void LArLReadMC::DLArLRO::Fill(int pdg, double edep, double step, double efield, double R, double* pos_step, double* pos_crp, double time,  double driftTime, double gain)
{

  //times are [ns]

  //cout<<__FUNCTION__<<endl;

  if( NPMTs == 0) return;


  //cout<<" LArLReadMC::DLArLRO::"<<__FUNCTION__<<" edep: "<<edep<<" step: "<<step<<" R: "<<R<<" gain: "<<gain<<endl;


  double xp = pos_step[0]*10;
  double yp = pos_step[1]*10;

  // TEMPORARY PATCH WHILE THE LIGHTMAPS HAVE AN OFFSET WRT TO THE DETECTOR
  //  double zp = pos_step[2]*10;
  double zp = pos_step[2]*10 + 480.;


  double xgar = pos_crp[0]*10;
  double ygar = pos_crp[1]*10;

  // TEMPORARY PATCH WHILE THE LIGHTMAPS HAVE AN OFFSET WRT TO THE DETECTOR
  //  double zgar = pos_crp[2]*10;
  double zgar = pos_crp[2]*10 + 480.; // Actually it is fixed in the code

  double drift_time = driftTime;

  float eleAtt = 1;
  if( electronLifeTime > 0 ) eleAtt= TMath::Exp(-drift_time/ms/electronLifeTime);

  //
  // check units
  //
  double totEnergyDep = edep; // MeV
  double delta_x = step*10;   // cm->mm
  double EField = efield;     // V/cm

  //cout<<" Fill: "<<edep<<" "<<delta_x<<" "<<efield<<endl;


  //dummy
  double Ekin = 0;
  double q = 1;
  double A = 1;
  TString part_name = "mu+";

  GLArProcess.ClearYields();
  
  GLArProcess.SetParticle(pdg);//part_name,q,A);
  GLArProcess.SetElectricField(EField*volt/cm);
  GLArProcess.SetEnergy(totEnergyDep*MeV,delta_x*mm,Ekin);
  GLArProcess.UseLArRecombData(true);

  GLArProcess.Compute();

  //GLArProcess.Print();

  //assert(0);

  //GLArProcess.SetElectricField(0);
  //GLArProcess.Compute();

  //GLArProcess.Print();

  

  double ratioX = GLArProcess.GetSingToTripRatioExcit();
  double ratioR = GLArProcess.GetSingToTripRatioRecomb();

  double tauSing = GLArProcess.GetTauSinglet()/ns;
  double tauTripl = GLArProcess.GetTauTriplet()/ns;
  double tauReco = GLArProcess.GetTauRecomb()/ns;
  double tauRise = GLArProcess.GetTauRise()/ns;

  double meanEne = GLArProcess.GetOpPhMeanEne();
  double sigmaEne = GLArProcess.GetOpPhEneWidth();


  int NTotPhot = GLArProcess.GetNumOfPhot();
  
  int NExcitPhot = GLArProcess.GetNumOfExcitons();
    
  int NTotDriftEle = GLArProcess.GetNumOfEle() * eleAtt ;

 
	
  double globalTime = time;

  double vertex_time = globalTime;

  // temporary here
  int nPhotonsPerDriftEle = 300;
  double LArAbsLength = 30*m ; // set to -1 to disable

  for(int i = 1 ; i<= spaceMapFld_LAr.GetNumOfSet() ; ++i) {

        Map::SpaceMapSet *mapSet = (Map::SpaceMapSet*)spaceMapFld_LAr.GetSet(i);

        Map::SpaceMap3 *map3_t0 = (Map::SpaceMap3*)mapSet->GetMap("Time0");
        Map::SpaceMap3 *map3_MPV = (Map::SpaceMap3*)mapSet->GetMap("MPV");
        Map::SpaceMap3 *map3_sigma = (Map::SpaceMap3*)mapSet->GetMap("Sigma");
        Map::SpaceMap3 *map3_w0 = (Map::SpaceMap3*)mapSet->GetMap("Weight");

	// cout<<"  -- "<<i<<" "<<map3w<<" "<<map3_tpk<<" "<<map3_trms<<endl;

	map3_t0->SetPoint( xp, yp, zp );
	map3_MPV->SetPoint( xp, yp , zp);
	map3_sigma->SetPoint( xp, yp, zp );
	map3_w0->SetPoint( xp, yp, zp );

        double t0 = map3_t0->Evaluate();
        double MPV = map3_MPV->Evaluate();
        double sigma = map3_sigma->Evaluate();
        double w0 = map3_w0->Evaluate();
  
	double w = w0*quantumEff;
	int NDET = gRandom->Poisson(NTotPhot*w);

	for(int iop = 0; iop<NDET; iop++ ){

		 TF1 *LandauTMP = new TF1("LandauTMP","landau",t0,10000);
		 LandauTMP->SetParameters(1,MPV,sigma);
		 double valMax = LandauTMP->GetX(0.00001,MPV);
		 TF1 *LandauRec = new TF1("LandauRec","landau",t0,valMax);
		 LandauRec->SetParameters(1,MPV,sigma);
	  	 double travel_time = LandauRec->GetRandom();
	  	 if (travel_time < t0) travel_time = t0 ;
	  	 delete LandauTMP;
	  	 delete LandauRec;

		 double w_absorption = 1;
	  	 if (LArAbsLength>0) w_absorption = exp(- travel_time *ns * (3*pow(10,8)*(m/s) / (1.25 * LArAbsLength)) ); 
	  	 bool is_noabs = gRandom->Uniform(0.,1.) < w_absorption;

	  	 if (is_noabs) {

          	 	double ratio = ratioX/(1+ratioX);

          	 	bool is_singlet = gRandom->Uniform(0.,1.) < ratio;

          	 	double tau;
          	 	if(is_singlet) tau = tauSing;
          	 	else tau = tauTripl;


          	 	double scint_time = GLArProcess.SampleScintTime(tauRise, tau);

          	 	double hit_t = vertex_time + scint_time + travel_time;
	  	 	//
	  	 	//check here
	  	 	TH1F* h = (TH1F*)dataLRO->hList.At(i-1);
	  	 	if(h_is_auto) RootUtils::FillConstantBinWidth(*h,hit_t,true);
	  	 	else h->Fill(hit_t);
		 }	
	 }

       
  }//PMTs


  
  
  for(int i = 1 ; i<=spaceMapFld_GAr.GetNumOfSet() ; ++i) {


        Map::SpaceMapSet *mapSet = (Map::SpaceMapSet*)spaceMapFld_GAr.GetSet(i);

        Map::SpaceMap3 *map3_t0 = (Map::SpaceMap3*)mapSet->GetMap("Time0");
        Map::SpaceMap3 *map3_MPV = (Map::SpaceMap3*)mapSet->GetMap("MPV");
        Map::SpaceMap3 *map3_sigma = (Map::SpaceMap3*)mapSet->GetMap("Sigma");
        Map::SpaceMap3 *map3_w0 = (Map::SpaceMap3*)mapSet->GetMap("Weight");

	double ZGAr = map3_w0->GetZaxis()->GetBinCenter(1);
	map3_t0->SetPoint( xgar, ygar, ZGAr );
	map3_MPV->SetPoint( xgar, ygar , ZGAr);
	map3_sigma->SetPoint( xgar, ygar, ZGAr );
	map3_w0->SetPoint( xgar, ygar, ZGAr );

        double t0 = map3_t0->Evaluate();
        double MPV = map3_MPV->Evaluate();
        double sigma = map3_sigma->Evaluate();
        double w0 = map3_w0->Evaluate();
  
	//zgar = map3w->GetZaxis()->GetBinCenter(1);

	double w = w0*quantumEff;
	int NDET = gRandom->Poisson(NTotDriftEle*nPhotonsPerDriftEle*w);


	//if(NDET>=0) cout<<" NDETGAR "<<NDET<<" "<<i<<" "<<NTotDriftEle<<" "<<w<<" "<<w0<<" "<<ZGAr<<endl;

	for(int iop = 0; iop<NDET; ++iop ){

		 TF1 *LandauTMP = new TF1("LandauTMP","landau",t0,10000);
		 LandauTMP->SetParameters(1,MPV,sigma);
		 double valMax = LandauTMP->GetX(0.00001,MPV);
		 TF1 *LandauRec = new TF1("LandauRec","landau",t0,valMax);
		 LandauRec->SetParameters(1,MPV,sigma);
	  	 double travel_time = LandauRec->GetRandom();
	  	 if (travel_time < t0) travel_time = t0 ;
	  	 delete LandauTMP;
	  	 delete LandauRec;

		 double w_absorption = 1;
	  	 if (LArAbsLength>0) w_absorption = exp(- travel_time *ns * (3*pow(10,8)*(m/s) / (1.25 * LArAbsLength)) ); 
	  	 bool is_noabs = gRandom->Uniform(0.,1.) < w_absorption;

		if (is_noabs) {
           		double ratio = ratioX/(1+ratioX);

           		bool is_singlet = gRandom->Uniform(0.,1.) < ratio;

           		double tau;
           		if(is_singlet) tau = tauSing;
           		else tau = tauTripl;

           		double scint_time = GLArProcess.SampleScintTime(tauRise, tau);
           		double hit_t = vertex_time + drift_time + scint_time + travel_time;

	   		//check here
	   		TH1F* h = (TH1F*)dataLRO->hList.At(i-1);
	   		if(h_is_auto) RootUtils::FillConstantBinWidth(*h,hit_t,true);
	   		else h->Fill(hit_t);
		}
	}


    }//PMTs



}
