////////////////////////////////////////////
//
// check event rate
//


#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cmath>

#include <TApplication.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>

#include "EventDecoder.h"
#include "RootUtils.h"
#include "LogMsg.h"

//#define __PRINT_DEBUG
//#define __PRINT_EVQFLAG

using namespace std;

//
// some global defaults (can be reset via command arguments)
//
ssize_t nch         = 1280; // total number of channels
ssize_t ntdc        = 1667; // total number of ADC samples per ch 

// raw data decoder
dlardaq::EventDecoder DaqDecoder( nch, ntdc );


// time difference
double diff_ms(struct timespec t1, struct timespec t2)
{
  return ((t1.tv_sec - t2.tv_sec) * 1.0E+3 + 
	  (t1.tv_nsec - t2.tv_nsec) * 1.0E-6);
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
  
  char cc; 
  const char* opts = "e:i:r:o:N:";
  while((cc = getopt(argc, argv, opts)) != -1)
    {
      switch(cc)
	{
        case 'i': 
          finname = optarg;
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

	default:
	  cerr<<"ERROR: unrecognized option"<<endl;
	  break;
	}
    }
  
  if(finname.empty()) return 0;
  

  // get data files
  std::vector<string> files;
  getFileNames( finname, nfiles, files ); 

  // daq headers
  dlardaq::runheader_t rnh;
  dlardaq::evheader_t  evh;
  dlardaq::footer_t    eft;

  TH1F *hdt = new TH1F("hdt", "hdt", 5000, 0, 5000);
  hdt->GetXaxis()->SetTitle("#Delta t (ms)");
  hdt->GetXaxis()->SetNdivisions(505);
  hdt->GetYaxis()->SetTitle("Events/1 ms");
  
  // event loop
  // read events from the file and process them
  size_t nevtot  = 0;
  size_t nevinc  = 0; // number of events with missing data
  double dttot   = 0; // total running time
  
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
	
	struct timespec first_trig;
	struct timespec last_trig;
	// loop over all events
	for(size_t i=0;i<(size_t)nev;i++)
	  {
	    std::vector<dlardaq::adc16_t> adc;
	    // get event data
	    DaqDecoder.GetEvent( i, evh, adc );
	    
	    
	    if( i % 20 == 0 ) 
	      {
		dlardaq::msg_info<<">>>>> Read event from file "<<i<<" / "<<nev<<endl;
#ifdef __PRINT_DEBUG
		dlardaq::msg_info<<">>> Event number "<<evh.ev_num<<endl;
		dlardaq::msg_info<<">>> Event size   "<<evh.ev_size<<endl;
		dlardaq::msg_info<<">>> Trig number  "<<evh.trig_info.num<<endl;
		dlardaq::msg_info<<">>> Time stamp   "<<evh.trig_info.ts.tv_sec<<" s "
				 <<evh.trig_info.ts.tv_nsec<<" ns"<<endl;
#endif
	      }

	    if( i == 0 ) 
	      {
		first_trig = evh.trig_info.ts;
		last_trig  = first_trig;
	      }
	    else
	      {
		double dt = diff_ms( evh.trig_info.ts, last_trig ); 
#ifdef __PRINT_DEBUG
		if( i % 20 == 0 )
		  {
		    dlardaq::msg_info<<">>> PTime stamp  "<<last_trig.tv_sec<<" s "
				     <<last_trig.tv_nsec<<" ns"<<endl;
		    dlardaq::msg_info<<">>> Dt "<<dt<<" ms"<<endl;
		  }
#endif

		RootUtils::FillConstantBinWidth( *hdt, dt );
		last_trig = evh.trig_info.ts;
	      }
	    
	    // check event quality flags
	    if( !EVDQFLAG(evh.dq_flag) )
	      {
		nevinc++;
		
#ifdef __PRINT_EVQFLAG
		dlardaq::msg_warn<<">>> Flags event "
				 <<evh.ev_num<<" detected, DQBits = "<<bitset<8>(evh.dq_flag)<<endl;
#endif
		//continue;
	      }
	    
	  } // event in a file loop
	
	dttot += diff_ms( last_trig, first_trig )*1.0E-3; // in s
    } // files loop

  dlardaq::msg_info<<"A total of "<<nevtot<<" events taken in "<<dttot<<"s"<<endl;
  dlardaq::msg_info<<"Average Trigger Rate: "<<nevtot/(dttot)<<"Hz"<<endl;
  dlardaq::msg_info<<"Fraction of incomplete events: "<<(float)(nevinc)/nevtot*100<<"%"<<endl;
  
  if( foutname.empty() )
    {
      // draw it
      TApplication *myapp = new TApplication("myapp", 0, 0);	       
      
      TCanvas *cnvs = new TCanvas("cnvs", "cnvs");
      cnvs->cd(1);
      hdt->Draw();
      cnvs->Modified();
      cnvs->Update();
      
      myapp->Run();
    }
  else
    {
      TFile *fout = TFile::Open(foutname.c_str(), "RECREATE");
      hdt->Write();
      fout->Close();
    }
  
  hdt->Delete();

  return 0;
}
