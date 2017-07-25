#include "DLArCRP.h"

using namespace std;

// ctor
LArQReadMC::DLArCRP::DLArCRP()
{
  VxlCRP = NULL;
  DatCRP = NULL;

  ncrms = 0;
  ElecSim = NULL;
}


// dtor
LArQReadMC::DLArCRP::~DLArCRP()
{
  Clean();
}

// clean memory
void LArQReadMC::DLArCRP::Clean()
{
  if(ElecSim)
    {
      delete ElecSim;
      ElecSim = NULL;
    }

  ncrms = 0;
  while(!CRMs.empty())
    {
      delete CRMs.back();
      CRMs.pop_back();
    }

  if(VxlCRP)
    {
      delete VxlCRP;
      VxlCRP = NULL;
    }

  if(DatCRP)
    {
      delete DatCRP;
      DatCRP = NULL;
    }

  while(!Noise.empty())
    {
      delete Noise.back();
      Noise.pop_back();
    }
}

// define CRP
void LArQReadMC::DLArCRP::DefineCRP(std::string config)
{
  Clean();
  ReadConfig( config.c_str() );
}


// read config file for CRP
void LArQReadMC::DLArCRP::ReadConfig(const char *crpconfig)
{
  ifstream ifun(crpconfig);
  string word;
  string tmpString;

  if (!ifun.is_open())
    {
      cerr << "ERROR: Cannot find CRP config file " << crpconfig << endl;
      exit(1);
    }

  while (ifun >> word)
    {
      // skip comment
      if (word[0]=='#') {
	getline(ifun,tmpString);
	continue;
      }

      //read datacard
      getline(ifun,tmpString);
      istringstream line(tmpString);
      ReadDatacard(line,word);
    }

  ifun.close();
}

// read data card
void LArQReadMC::DLArCRP::ReadDatacard(istringstream &line, string &keyword)
{
  std::string myLine;
  if(keyword == "DETECTOR")
    {
      line >> myLine;
      cout<<" Detector Model : "<<myLine<<endl;
    }
  else if(keyword == "RAWDATATYPE")
    {
      std::string name;
      int qntz = 0;
      line >> name >> qntz;
      cout<<" Raw data type : "<<name<<" "<< qntz <<endl;

      SetRawDataType(name, qntz);
    }
  else if(keyword == "SAVEVOXELINFO")
    {
      int doit = 0;
      line >> doit;
      if(doit > 0)
	{
	  VxlCRP = new LArVoxel::VoxelInfoCRP();
	  cout<<" Saving voxel information"<<endl;
	}
    }
  else if(keyword == "ELECMODEL")
    {
      std::string elecmod;
      float ped, ped_rms;
      float sampling;
      //float mV2adc;
      int bits;
      line >> elecmod >> ped >> ped_rms >> bits >>sampling;
      cout<<" Elec model      : "<<elecmod<<endl;
      //cout<<"    mV -> ADC    : "<<mV2adc<<endl;
      cout<<"    pedestal     : "<<ped<<", "<<ped_rms<<endl;
      cout<<"    max bits     : "<<bits<<endl;
      cout<<"    sampl. freq.  :  "<<sampling<<endl;

      SetElecSimType(elecmod, bits, ped, ped_rms, sampling);
    }
  else if(keyword == "CRM")
    {
      int id;
      float x, y, z;
      float wx, wy;
      float gain;
      float pitchx, pitchy;
      unsigned int nchx, nchy;
      unsigned int nsamples;

      line >> id >> x >> y >> z >> wx >> wy >> gain >> pitchx >> pitchy >> nchx >> nchy >> nsamples;

      cout<<endl<<" CRM ID   : "<<id<<endl;
      cout<<"     pos (mm)   : ("<<x<<", "<<y<<", "<<z<<")"<<endl;
      cout<<"     width (mm) : "<<wx<<" x "<<wy<<endl;
      cout<<"     gain       : "<<gain<<endl;
      cout<<"     pitch (mm) : "<<pitchx<<", "<<pitchy<<endl;
      cout<<"     nchans     : "<<nchx<<", "<<nchy<<endl;
      cout<<"     nsamples   : "<<nsamples<<endl<<endl;

      if(!ElecSim)
	{
	  cerr<<"ERROR: model for electronics has to be defined first"<<endl;
	  exit(1);
	}

      LArQReadMC::DLArCRM* dlarcrm = new LArQReadMC::DLArCRM( id, x, y, z, wx, wy,
							      pitchx, pitchy,
							      gain, nchx, nchy,
							      nsamples, ElecSim );
      if( Overlaps(dlarcrm) )
	{
	  cerr<<"ERROR: Cannot add CRM at "<<x<<", "<<y<<" with "
	      <<wx<<" x "<<wy<<" due to overlaps with other CRMs"<<endl;
	  delete dlarcrm;
	}
      else // add to the list
	{
	  // save raw data
	  if(DatCRP)
	    {
	      LArRawData::DataCRM *dcrm = DatCRP->AddCRM(id);
	      dlarcrm->SaveRawData(dcrm, dtype);
	      dlarcrm->Quantize(quant);
	    }

	  // save voxel information
	  if(VxlCRP)
	    {
	      dlarcrm->SaveVoxelInfo(VxlCRP->AddCRM(id));
	    }
	  CRMs.push_back(dlarcrm);
	  ncrms++;
	}

    }

  else if(keyword == "NOISE")
    {
      std::string noiseType = "";
      float noiseFreq;
      float noiseRMS;
      int   coher;
      line >> noiseType >> noiseFreq >> noiseRMS >> coher;
      if( noiseType.compare("CAPA") == 0)
	{
	  CapaWhiteNoise *myNModel = new CapaWhiteNoise();
	  myNModel->SetName(noiseType);
	  myNModel->SetElecModule(ElecSim);
	  myNModel->SetNoiseRMS(noiseRMS);
	  myNModel->SetCoherent( coher != 0 );
	  cout << "Adding noise model called " << myNModel->GetName() << endl;
	  Noise.push_back(myNModel);
	}
      /*
      else if ( noiseType.compare("ADC") == 0)
	{
	  ADCWhiteNoise *myNModel = new ADCWhiteNoise();
	  myNModel->SetName(noiseType);
	  myNModel->SetElecModule(ElecSim);
	  myNModel->SetNoiseRMS(noiseRMS);
	  myNModel->SetCoherent( coher != 0 );
	  cout << "Adding noise model called " << myNModel->GetName() << endl;
	  Noise.push_back(myNModel);
	}
      */
      else
	cerr << "ERROR : Uknown noise mode " << noiseType << endl;
    }
}

// add different output data types here
void LArQReadMC::DLArCRP::SetRawDataType(std::string tname, int qntz)
{
  quant   = (qntz != 0); //quantize data
  DatCRP  = new LArRawData::DataCRP();
  dtype   = LArRawData::DTID_FLOAT;

  if(tname.find("FLOAT") != std::string::npos)
    {
      dtype = LArRawData::DTID_FLOAT;
    }
  else if(tname.find("USHORT") != std::string::npos)
    {
      dtype = LArRawData::DTID_USHORT;
    }
  else
    {
      cout<<"WARNING: uknown datatype "<<tname<<endl;
      cout<<"         using FLOAT     "<<endl;
    }
}

// add different electronics models here
void LArQReadMC::DLArCRP::SetElecSimType(std::string tname, int bits, float ped,
					 float ped_rms, float sampling )
{
  if(tname.find("ETHZ") != std::string::npos)
    {
      ElecSim = new DigitizerETHZ();
    }
  else
    {
      cout<<"WARNING: uknown elecsim model "<<tname<<endl;
      ElecSim = new DigitizerETHZ();
    }

  ElecSim->SetPedestal(ped);
  ElecSim->SetPedRMS(ped_rms);
  ElecSim->SetBitsADC(bits);
  ElecSim->SetSampleFreq(sampling);
  //ElecSim->SetmVtoADC(mVadc);
  ElecSim->ComputeRespVector();
}

// begin event
void LArQReadMC::DLArCRP::BeginEvent()
{
  //
  for(size_t i=0;i<CRMs.size();i++)
    {
      CRMs[i]->BeginEvent();
    }
}

// end event
void LArQReadMC::DLArCRP::EndEvent()
{
  //
  for(size_t i=0;i<CRMs.size();i++)
    {
      //CRMs[i]->EndEvent();
      if(!Noise.empty())
	{
	  //for(size_t j=0;j<Noise.size();j++)
	  //{
	  //  Noise[j]->SetNewEvent( true );
	  //}
	  CRMs[i]->AddNoise( Noise );
	}

      CRMs[i]->EndEvent();
    }
}

// fill
void LArQReadMC::DLArCRP::Fill( int pid, double edep, double edepq,
				double xvox, double yvox, double zvox, double tvox,
				double dt, double x, double y, double q,
				double sigmaL, double sigmaT )
{
  // pid   -- particle pdg id code
  // edep  -- deposited energy in MeV
  // edepq -- deposited energy quenched
  // xvox, yvox, zvox, tvox -- voxel position and absolute time of particle
  // dt     -- drift time
  // x, y   -- position on CRP
  // sigmaL -- longitudinal charge diffusion at anode ( us )
  // sigmaT -- transverse charge diffusion at anode ( cm )

  for(size_t i=0;i<CRMs.size();i++) //should only fill one CRM though
    CRMs[i]->Fill(pid, edep, edepq, xvox, yvox, zvox, tvox, dt, x, y, q, sigmaL, sigmaT);
}

// check if the time is inside drift window
bool LArQReadMC::DLArCRP::IsInDriftWin(double dt)
{
  if(CRMs.empty()) return false;

  bool rval = false;
  for(size_t i=0;i<CRMs.size();i++)
    {
      if(CRMs[i]->GetTimeBin( dt ) >= 0 )
	{
	  rval = true;
	  break;
	}
    }

  return rval;
}

// register branches
void LArQReadMC::DLArCRP::RegisterBranches(TTree *tree)
{
  //cout<<"Tree address "<<tree<<endl;
  if(DatCRP)
    tree->Branch("CRPRawData", DatCRP->ClassName(), &DatCRP);
  if(VxlCRP)
    tree->Branch("CRPVoxelData", VxlCRP->ClassName(), &VxlCRP);
}


// check for overlaps with other CRMs
bool LArQReadMC::DLArCRP::Overlaps(LArQReadMC::DLArCRM *crm)
{
  int ncrn = 4; //number of corners
  float xcrn[4];
  float ycrn[4];

  // set the corners to slightly smaller values
  xcrn[0] = (crm->GetX0() - 0.99999*0.5*crm->GetWidthX());
  xcrn[1] = (crm->GetX0() - 0.99999*0.5*crm->GetWidthX());
  xcrn[2] = (crm->GetX0() + 0.99999*0.5*crm->GetWidthX());
  xcrn[3] = (crm->GetX0() + 0.99999*0.5*crm->GetWidthX());

  ycrn[0] = (crm->GetY0() - 0.99999*0.5*crm->GetWidthY());
  ycrn[1] = (crm->GetY0() + 0.99999*0.5*crm->GetWidthY());
  ycrn[2] = (crm->GetY0() + 0.99999*0.5*crm->GetWidthY());
  ycrn[3] = (crm->GetY0() + 0.99999*0.5*crm->GetWidthY());

  for(int i=0;i<ncrn;i++)
    {
      for(size_t j=0;j<CRMs.size();j++)
	{
	  if(CRMs[j]->isInsideCRM(xcrn[i], ycrn[i]))
	    {
	      std::cerr<<"ERROR: Overlap with CRM "
		       <<CRMs[j]->GetCRMId()<<std::endl;
	      //std::cerr<<xcrn[i] << " "<<ycrn[i] <<std::endl;
	      return true;
	    }
	}
    }

  return false;
}

// distance to CRP
double LArQReadMC::DLArCRP::DistanceToCRP(double x, double y, double z)
{
  double rval = -1.0;
  if(!CRMs.empty())
    {
      // in principle they should all hang at the same height
      rval = z - CRMs[0]->GetZ0();
    }

  return rval;
}

double LArQReadMC::DLArCRP::GetZ()
{
  double rval = -999.0;
  if(CRMs.empty())
    std::cerr<<"ERROR: no CRM defined"<<std::endl;
  else
    rval = CRMs[0]->GetZ0();

  return rval;
}


// get gain for a specific CRM
float LArQReadMC::DLArCRP::GetCRMGain(double x, double y)
{
  float rval = -1.0;

  for(size_t j=0;j<CRMs.size();j++)
    {
      if(CRMs[j]->isInsideCRM(x, y))
	{
	  rval = CRMs[j]->GetGain();
	  break;
	}
    }

  return rval;
}


//
//
//
void LArQReadMC::DLArCRP::SetCollEffMap( std::string fname )
{
  for(size_t i=0;i<CRMs.size();i++)
    CRMs[i]->SetCollEffMap( fname );     

}
