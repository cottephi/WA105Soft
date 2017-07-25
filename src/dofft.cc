////////////////////////////////////////////
//
// performs FFT on the raw data file
// units: time [us], f[MHz]
//


#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cmath>

#include <TRandom3.h>
#include <TApplication.h>
#include <TVirtualFFT.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TH1F.h>

#include "FFTUtils.h"
#include "ChPedestal.h"
#include "ChMapInterface.h"
#include "EventDecoder.h"
#include "RootUtils.h"
#include "LogMsg.h"


using namespace std;

//
// some global defaults (can be reset via command arguments)
//
std::string cmname  = "3x1x1";
ssize_t nch         = 1280; // total number of channels
ssize_t ntdc        = 1667; // total number of ADC samples per ch 

// raw data decoder
dlardaq::EventDecoder DaqDecoder( nch, ntdc );

//
// fft utility
//
FFTUtils SigFFT( ntdc );

// digitizer frequency
float fMHz   = 2.5;
// 
size_t ndata = SigFFT.FFTSize();   // size of data for FFT : 2^n
size_t nfreq = SigFFT.FreqSize();  // number of frequencies
size_t nhalf = ndata/2;
float df     = fMHz/ndata;

//
// power spectrum for plotting
//
void PowerSpectrum( std::vector<TComplex> &fftdata, std::vector<float> &freq,
		    std::vector<float> &spectrum, bool skipdc = true, float Aref = 0 )
{
  freq.clear();
  spectrum.clear();
    
  double p0;
  size_t istart = 0;
  if(skipdc) istart++;

  for( size_t i=istart;i<fftdata.size();i++)
    {
      double p = fftdata[i].Rho()/ndata;
      double f = df*i;
      if( i!=0 && i!=nhalf ) p *= 2;
      if( i==0 ) p0 = p;
      
      if(Aref > 0 ) p = 10*log10(p/Aref);;  // dB
      
      freq.push_back( f );
      spectrum.push_back( p );
    }
}

//
// get list of files assuming format runnum-xx.ext
//
void getFileNames( string fname, size_t nfile, vector<string> &files )
{
  files.push_back( fname );
  size_t pos1 = fname.find_last_of('-');
  if( pos1 == std::string::npos ) 
    {
      dlardaq::msg_err<<"Bad name format for the input file in name "<<fname<<endl;
      return;
    }
  
  // find extension
  size_t pos2 = fname.find_first_of('.', pos1);
  if( pos2 == std::string::npos ) 
    {
      dlardaq::msg_err<<"Unrecognized extension in "<<fname<<endl;
      return;
    }
  
  int cfile = atoi(files[0].substr(pos1+1, pos2-pos1).c_str());
  for(size_t i=1;i<nfile;i++)
    {
      cfile++;
      stringstream ss;
      ss<<files[0].substr(0, pos1+1)<<cfile<<files[0].substr(pos2);
      files.push_back( ss.str() );
    }
  
}


//
//
//
int main(int argc, char** argv)
{
  ssize_t nev     = -1;
  ssize_t nfiles  = 1;
  string finname  = "";
  string foutname = "";
  float Aref = 0;

  // 
  calidaq::ChPedestal *caliPed = NULL;
  
  char cc; 
  const char* opts = "e:i:r:o:N:P:";
  while((cc = getopt(argc, argv, opts)) != -1)
    {
      switch(cc)
	{
        case 'i': 
          finname = optarg;
          break;
	  
	case 'r': //reference amplitude to convert to dB
	  Aref = atof( optarg );
	  break; 

	case 'e':
	  nev = atoi( optarg );
	  break;

	case 'o':
	  foutname = optarg;
	  break;

	case 'N':
	  nfiles = atoi( optarg );
	  break;

	case 'P':
	  if( atoi( optarg ) > 0 )
	    caliPed = &calidaq::ChPedestal::Instance();
	  break;
	  
	default:
	  cerr<<"ERROR: unrecognized option"<<endl;
	  break;
	}
    }
  
  if(finname.empty()) return 0;
  
  // calculate channel map
  dlardaq::ChannelMap *chmap =  dlardaq::ChMapInterface::Instance().GetMap(cmname);
  if(!chmap) 
    {
      dlardaq::msg_err<<"Cannot find channel map for "<<cmname
		      <<". Aborting..."<<endl;
      exit(-1);
    }
  

  // get data files
  std::vector<string> files;
  getFileNames( finname, nfiles, files ); 

  // histogram of the fft contains all channels
  TH2F *h2fft = new TH2F("event_fft", "event_fft", nch, 0, nch, nfreq, 0, nfreq*df);
  h2fft->SetStats(0);
  h2fft->GetXaxis()->SetTitle("ch");
  h2fft->GetYaxis()->SetTitle("f (MHz)");
  h2fft->SetDirectory(0);
  
  // daq headers
  dlardaq::runheader_t rnh;
  dlardaq::evheader_t  evh;
  dlardaq::footer_t    eft;
  

  // event loop
  // read events from the file and process them
  size_t counter = 0;
  size_t nevtot  = 0;
  for(size_t f=0;f<files.size();f++)
    {
      dlardaq::msg_info<<"Reading "<<files[f]<<endl;
        DaqDecoder.Open(files[f]);
	
	rnh = DaqDecoder.GetRunHeader();
	eft = DaqDecoder.GetFileFooter();
	
	dlardaq::msg_info<<"Run number      "<<rnh.run_num<<endl;
	dlardaq::msg_info<<"Number of event in the file "<<eft.num_events<<endl;
	
	size_t nevfile = DaqDecoder.GetTotEvents();
	if( nev <= 0 || nev >= (ssize_t)nevfile )
	  nev = nevfile;
	
	// skip empty files
	if( nev == 0 ) continue;
	
	nevtot += nev;
	
	// loop over all events
	for(size_t i=0;i<(size_t)nev;i++)
	  {
	    std::vector<dlardaq::adc16_t> adc;
	    // get event data
	    DaqDecoder.GetEvent( i, evh, adc );
	    
	    if( i % 20 == 0 ) 
	      {
		dlardaq::msg_info<<">>>>> Read event from file "<<i<<" / "<<nev<<endl;
#ifdef DOFFT_DEBUG
		dlardaq::msg_info<<">>> Event number "<<evh.ev_num<<endl;
		dlardaq::msg_info<<">>> Event size   "<<evh.ev_size<<endl;
		dlardaq::msg_info<<">>> Trig number  "<<evh.trig_info.num<<endl;
		dlardaq::msg_info<<">>> Time stamp   "<<evh.trig_info.ts.tv_sec<<" s "
				 <<evh.trig_info.ts.tv_nsec<<" ns"<<endl;
#endif
	      }

	    // process only comlete events
	    if( !EVDQFLAG(evh.dq_flag) )
	      {
#ifdef DOFFT_DEBUG
		dlardaq::msg_warn<<">>> Flags event "
				 <<evh.ev_num<<" detected, DQBits = "<<bitset<8>(evh.dq_flag)<<endl;
#endif
		continue;
	      }
      
	    // loop over channels
	    std::vector<dlardaq::adc16_t>::iterator first, last;
	    first = adc.begin();
	    for(size_t c=0;c<(size_t)nch;c++)
	      {
	  
		std::vector<dlardaq::adc16_t>::iterator last = first + ntdc;
		
		// 
		std::vector<float> data;
		//std::vector<float> data( first, last );
		
		float chped = 0;
		if(caliPed) // get pedestal if requested
		  {
		    int icrm, iview, ichv;
		    if( chmap->MapToCRP( c, icrm, iview, ichv ) > 0) 
		      chped = caliPed->Pedestal( icrm, iview, ichv );
		  }
		
		// subtract pedestal
		if( chped != 0 ) 
		  {
		    std::vector<dlardaq::adc16_t>::iterator it = first;
		    for(;it!=last;it++)
		      {
			data.push_back( *it - chped );
		      }
		  }
		else
		  data.insert( data.begin(), first, last );
		
		//
		first = last;
	  
		std::vector<TComplex> fftdata;
		// do fft
		SigFFT.DoFFT( data, fftdata );
		
		// get power spectrum
		std::vector<float> freq, spectrum;
		PowerSpectrum( fftdata, freq, spectrum, false, Aref );
		
		// fill histogram 
		for(size_t j=0;j<freq.size();j++)
		  {
		    h2fft->Fill( c+0.5, freq[j], spectrum[j] );
		  }
		
	      }
	    counter++;
	  } // event in a file loop
    } // files loop

  // averaged over all the events
  h2fft->Scale(1./counter);

  //
  dlardaq::msg_info<<"Used events "<<counter<<" out of "<<nevtot<<endl;

  if( foutname.empty() )
    {
      // draw it
      TApplication *myapp = new TApplication("myapp", 0, 0);	       
      
      TCanvas *cnvs = new TCanvas("cnvs", "cnvs");
      cnvs->cd(1);
      h2fft->Draw("colz");
      cnvs->SetLogz();
      cnvs->Modified();
      cnvs->Update();
      
      myapp->Run();
    }
  else
    {
      TFile *fout = TFile::Open(foutname.c_str(), "RECREATE");
      h2fft->Write();
      fout->Close();
    }
  
  h2fft->Delete();

  return 0;
}
