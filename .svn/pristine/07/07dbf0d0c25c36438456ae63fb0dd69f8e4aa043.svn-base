///////////////////////////////////////////////////////////////////////////////////////
//
//
//  Cast raw data into appropriate format for CRP readout
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "RawDataFormatUtil.h"

using namespace std;

// find search path from env
void GetSearchPathsFromEnv( std::string envname, 
			    std::vector<std::string> &paths)
{

  paths.clear();
  
  if( const char *env_p = std::getenv(envname.c_str()) )
    {
      stringstream ss(env_p);
      string s;
      while(getline(ss, s, ':'))
	paths.push_back(s);
    }
}



// the pointers should have been initalized to valid memory addresses before hand
RawDataFormatUtil::RawDataFormatUtil( RunHeader *rh, GeomConfig *gc,
				      EventHeader *eh, LArRawData::DataCRP *crpdata )
{
  m_runh  = rh;
  
  // we are always dealing with actual raw data here
  m_runh->SetDataType(1);

  m_gconf = gc;
  m_eveh  = eh;
  m_crp   = crpdata;
 
  m_gcfile     = "";
  m_chmap_name = "not_defined";

  GetSearchPathsFromEnv( "WA105CONFIGDIR", m_spath );
  if(m_spath.empty())
    {
      cerr<<"ERROR: $WA105CONFIGDIR env variable is not set "<<endl;
      abort();
    }
}


//
//
//
void RawDataFormatUtil::FillRun(dlardaq::runheader_t &rh)
{
  m_runh->SetRunNumber( (int)(rh.run_num) );

  // for 3x1x1 only for now
  m_runh->SetGeometry( 3000 );
  //m_runh->SetCompression( (rh.data_comp)?1:0 );
  m_runh->SetEfieldValue( GetEfield() );

  std::string geoconf = GeoConfigFileName(m_runh->GetGeometry());
  m_runh->SetConfigFile( geoconf );
  
  InitGeoConfig( geoconf );
}


//
//
// fill event data 
void RawDataFormatUtil::FillEvent(dlardaq::evheader_t &eh, std::vector<dlardaq::adc16_t> &adc)
{
  
  // actual
  size_t dsz = adc.size();
  if( dsz != m_nch * m_nsample )
    {
      cerr<<"ERROR: mismatch between actual and expected data size "<<endl;
      cerr<<"      "<<dsz<<" is actual size and "<<m_nch*m_nsample<<" is expected"<<endl;
      //abort();
    }

  // set event time etc...  
  m_eveh->SetEventNumber( (int)eh.ev_num );
  m_eveh->SetDQFlag( EVDQFLAG(eh.dq_flag) );
  m_eveh->SetEventTime( eh.trig_info.ts );

  
  dlardaq::ChannelMap *chmap =  dlardaq::ChMapInterface::Instance().GetMap(m_chmap_name);
  if(!chmap) return;
  
  // loop over all CRMs
  LArRawData::DataCRM *crm;
  TIter next_crm(m_crp->GetCRMs());
  while ( (crm = (LArRawData::DataCRM*) next_crm()) ) 
    {
      
      int icrm = crm->GetCRMId();
      
      TClonesArray *chdata;
      LArRawData::DataChan *ch = NULL;
      
      // process channels in view 0
      chdata = crm->GetChanView0();
      TIter nextch0(chdata); //
      while ( (ch = (LArRawData::DataChan*)(nextch0())) )
	{
	  int chv = ch->GetChanId(); 
	  
	  // get ch sequence in the file
	  int daqch = chmap->MapToDAQFile(icrm, 0, chv);
	  if(daqch<0) continue; // problem...

	  // now get the segments
	  TIter next(ch->GetDataFragments());
	  
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{
		  LArRawData::DataFragment<UShort_t> *frag = (LArRawData::DataFragment<UShort_t>*)obj;
		  // fill data

		  for(UInt_t i=frag->GetFragStart();i<frag->GetFragEnd();i++)
		    {
		      size_t spos = daqch*m_nsample + i;
		      if( spos >= dsz ) break;
		      frag->AddDatum(i, (UShort_t)adc[spos], true);
		    }
		}
	    }
	}
      
      // process channels in view 1
      chdata = crm->GetChanView1();
      TIter nextch1(chdata); //
      while ( (ch = (LArRawData::DataChan*)(nextch1())) )
	{
	  int chv = ch->GetChanId(); 
	  
	  // get ch sequence in the file
	  int daqch = chmap->MapToDAQFile(icrm, 1, chv);
	  if(daqch<0) continue; // problem...
	  
	  // now get the segments
	  TIter next(ch->GetDataFragments());
	  
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{
		  LArRawData::DataFragment<UShort_t> *frag = (LArRawData::DataFragment<UShort_t>*)obj;
		  // fill data
		  for(UInt_t i=frag->GetFragStart();i<frag->GetFragEnd();i++)
		    {
		      size_t spos = daqch*m_nsample + i;
		      if( spos >= dsz ) break;
		      frag->AddDatum(i, (UShort_t)adc[spos], true);
		    }
		}
	    }
	}

    } // loop over crms

  
}





//
//
//
std::string RawDataFormatUtil::GeoConfigFileName( int geo_type )
{
  //
  std::string geoconf = "";
  
  int id = geo_type / 1000;
  switch (id)
    {
    case 3:
      geoconf = "WA105_1x1x3.config";
      break;
      
    case 6:
      geoconf = "WA105_6x6x6.config";
      break;
    
    default:
      cerr<<"ERROR "<<geo_type<<" is not defined geo type"<<endl;
      break;      
    }

  return geoconf;
}


//
//
//
void RawDataFormatUtil::InitGeoConfig( std::string gcfile )
{
  
  // already initialized ?
  if( gcfile == m_gcfile )
    {
      return;
    }
  
  m_crp->Clear();
  m_gcfile = gcfile;
  
  ifstream inFile;

  bool ok = false;
  for(size_t i=0;i<m_spath.size();i++)
    {
      string fname = m_spath[i] + "/" + m_gcfile;
      inFile.open(fname.c_str(), ios::in);

      if(inFile.is_open())
	{
	  ok = true;
	  break;
	}
    }
  
  if(!ok)
    {
      cerr <<"  detector configuration file "<<gcfile<< "  could not be found in "<<endl;
      for(size_t i=0;i<m_spath.size();i++)
	cout<<" "<<m_spath[i]<<endl;
      abort();
    }

  string aLine  = "";
  string check1 = "DETECTOR";
  string check2 = "RAWDATATYPE";
  string check3 = "ELECMODEL";
  string check4 = "CRM";
  //string check5 = "NOISE";

  int ncrm = 0;
    
  int ped,maxbits,ifquanta;
  float pedrms;
  //int coherent;
  //float freq,rms;
  float sampling;
  int id,nchax,nchay,nsample;
  float x,y,z,dx,dy,gain,pitchx,pitchy;
  
  m_nch = 0;
  m_nsample = 0;
  while (!inFile.eof())
    {
      getline(inFile,aLine);
      istringstream ss;
      if (aLine.compare(0, check1.length(), check1)==0)
	{
	  string astring="";
	  string aname = "";
	  ss.str(aLine);
	  ss>>astring>>aname;
	  m_gconf->SetGC_GeomType( aname );
	} // select RAWDATATYPE
       if (aLine.compare(0, check2.length(), check2)==0)
       { 
	  string astring="";
	  string aname="";
	  ss.str(aLine);
	  ss>>astring>>aname>>ifquanta;
	  m_gconf->SetGC_RDType( aname );
	  m_gconf->SetGC_quanta( ifquanta );
	   
       	}  // select ELECMODEL
       if  (aLine.compare(0,check3.length(),check3)==0) 
	 {
	   
	   string astring="";
	   string aname="";
	   ss.str(aLine); 
	   ss>>astring>>aname>>ped>>pedrms>>maxbits>>sampling;
	   m_nadc = maxbits;
	   m_gconf->SetGC_ElecModel(aname);
	   m_gconf->SetGC_Ped(ped);
	   m_gconf->SetGC_PedRMS(pedrms);
	   m_gconf->SetGC_maxbit(maxbits);
	   m_gconf->SetGC_samplingfreq(sampling);
	   m_gconf->SetCRMpitchT(1./sampling); //in us
	   
	} 
       if  (aLine.compare(0,check4.length(),check4)==0) 
	 {
	   string aname="";
	   string bname="";
	   ss.str(aLine); 
	   ss>>aname>>id>>x>>y>>z>>dx>>dy>>gain>>pitchx>>pitchy>>nchax>>nchay>>nsample; 
	   
	   m_gconf->SetCRMcX(ncrm,x);
	   m_gconf->SetCRMcY(ncrm,y);
	   m_gconf->SetCRMcZ(ncrm,z);
	   m_gconf->SetCRMxdim(ncrm,dx);
	   m_gconf->SetCRMydim(ncrm,dy); 
	   m_gconf->SetCRMpitchX(ncrm,pitchx);
	   m_gconf->SetCRMpitchY(ncrm,pitchy);
	   m_gconf->SetCRMnchaX(ncrm,nchax);
	   m_gconf->SetCRMnchaY(ncrm,nchay);
	   m_gconf->SetCRMgain(ncrm,gain);
	   m_gconf->SetCRMnsample(ncrm,nsample);      
	   
	   // should be the same for all crms though
	   m_nsample = nsample;

	   // add all the channels
	   m_nch += (size_t)( nchax + nchay );

	   LArRawData::DataCRM *dcrm = m_crp->AddCRM(id);

	   // channels in view 0
	   for(int ch=0;ch<nchax;ch++)
	     {
	       LArRawData::DataChan *chan = dcrm->AddDataChannel( 0, (Int_t)ch, 
								  LArRawData::DTID_USHORT, 
								  LArRawData::FULLDATA );
	       // initialize fragments (should be only one)
	       chan->AddDataFragment();
	       chan->InitFragments( 0, (size_t)(nsample) );
	     }
	   
	   // channels in view 1
	   for(int ch=0;ch<nchay;ch++)
	     {
	       LArRawData::DataChan *chan = dcrm->AddDataChannel( 1, (Int_t)ch, 
								  LArRawData::DTID_USHORT, 
								  LArRawData::FULLDATA );
	       // initialize fragments (should be only one)
	       chan->AddDataFragment();
	       chan->InitFragments( 0, (size_t)(nsample) );
	     }
	   
	   ncrm++;  //# CRM 
	 } // if  (aLine.compare(0,check4.length(),check4)==0)
       
       //if(aLine.compare(0,check5.length(),check5)==0) 
       //{ 
       //string astring="";
       //string aname="";
       //ss.str(aLine);
       //ss>>astring>>aname>>freq>>rms>>coherent;
       //m_gconf->SetGC_NoiseName(aname);
       //m_gconf->SetGC_NoiseFreq(freq);
       //m_gconf->SetGC_NoiseRMS(rms);
       //m_gconf->SetGC_cohe(coherent);
       
    } //eof 
  
  m_gconf->SetCRMnum(ncrm); 

  m_gconf->CheckGeomConfig();
  
  int idgeo=0; 
  if(m_gcfile.find("6x6x6") != std::string::npos) 
    {
      idgeo = 6000+ncrm;
    }
  else if(m_gcfile.find("3x1x1") != std::string::npos) 
    {
      m_chmap_name = "3x1x1";
      idgeo = 3000+ncrm; 
    }
  else if(m_gcfile.find("1x1x3") != std::string::npos) 
    {
      m_chmap_name = "3x1x1";
      idgeo = 3000+ncrm; 
    }
  m_runh->SetGeometry(idgeo);
}
 

//
//
//

