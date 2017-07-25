////////////////////////////////////////////////////////////////////////////////////////
//
// Perform gain analysis on 50x50 cm2 area
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>

#include "QHit.h"
#include "Track2d.h"

#include "CRPAnaSummaryFile.h"
#include "EventDataManager.h"
#include "CRTracksAnaDEDX.h"
#include "CRPQAnaStore.h"
#include "RecoUtils.h"

using namespace std;


void get_help()
{
  cout<<"Usage: "<<endl;
  cout<<" -i <input file>    - input file (mandatory)"<<endl;
  cout<<" -s <summary file>  - i/o file should contain purity measurements (mandatory)"<<endl;
  cout<<" -a <y/n>           - write ouput to the summary file provided (default is no)"<<endl;
  cout<<" -t <truncate mean> - fraction for truncated mean ( default 0 = no truncation)"<<endl;
  cout<<" -o <output file>   - name of the ROOT output file (optional)"<<endl;
  cout<<" -h                 - print this message"<<endl;
  exit(-1);
}

//
//
//
int main(int argc, char **argv)
{
  string recfname = "";    // mandatory
  string sumfname = "";    // mandatory
  string outfname = "";    // optional
  bool   doappend = false; //
  double trFrac   = 0.0;   // fraction of event to discard for truncated mean

  if(argc == 1)
  get_help();

  string tmpstr;
  char cc;
  while((cc = getopt(argc, argv, "a:i:s:o:t:")) != -1)
  {
    switch(cc)
    {
      case 'a':
      tmpstr = optarg;
      if(tmpstr.compare("y") >= 0)
      doappend = true;
      else
      doappend = false;
      break;

      case 'i':
      recfname = optarg;
      break;

      case 's':
      sumfname = optarg;
      break;

      case 'o':
      outfname = optarg;
      break;

      case 't':
      trFrac = atof( optarg );
      break;

      default:
      get_help();
    }
  }

  //
  if(recfname.empty())
  {
    cerr<<"Need to provide reconstruction file"<<endl;
    get_help();
  }

  //
  if(sumfname.empty())
  {
    cerr<<"Need to provide analysis summary file"<<endl;
    get_help();
  }

  if(outfname == recfname)
  {
    cerr<<"Cannot have the same name as input / output file"<<endl;
    get_help();
  }

  if( outfname.empty() && !doappend)
  {
    cerr<<"No output to produce. Aborting..."<<endl;
    get_help();
  }

  // open input file
  CRPAnaSummaryFile sFile;
  sFile.Open( sumfname );
  double p1, p2, p3, p4;
  sFile.GetPurity(p1, p2, p3, p4);
  double taue_us = 0.5*(p1 + p3);

  EventDataManager *evmgr = new EventDataManager();
  evmgr->Open( recfname );

  RunConfig rconf;
  evmgr->GetRunConfig( rconf );
  CRTracksAnaDEDX anagain( rconf.GetRunHeader(), rconf.GetCRPConfig() );
  anagain.SetLifetime( taue_us );

  // loop over events and fill histograms
  Event *event;

  //
  int selected = 0;

  // scoring plane (TO DO generalize)
  CRPQAnaStore *anares = new CRPQAnaStore("crp3x1x1", -50., -150., 50, 150.);
  // segment CRP area into LEMs
  anares->BreakIntoSegments(50.0, 50.0);
  anares->SetTrMeanParameter(0, trFrac);

  std::vector< pair<int, int> > mpairs;
  std::vector< pair<int, int> >::iterator itmap;
  for( Long64_t ev = 0;ev<evmgr->GetEntries();ev++)
  {
    //std::cout<<"---> Processing event  "<<ev<<std::endl;
    event = evmgr->GetEvent(ev);

    // get tracks
    std::string sname_trks0 = "RecoCRTracks_View0";
    std::string sname_trks1 = "RecoCRTracks_View1";
    const TObjArray *trksview0 = event->Get(sname_trks0);
    const TObjArray *trksview1 = event->Get(sname_trks1);

    if(!RecoUtils::ViewTrackMatch(trksview0, trksview1, mpairs, 1.0)) continue;

    for(itmap = mpairs.begin(); itmap != mpairs.end(); itmap++)
    {

      LArReco::Track2d* trk0 = (LArReco::Track2d*)trksview0->At(itmap->first);
      LArReco::Track2d* trk1 = (LArReco::Track2d*)trksview1->At(itmap->second);

      if(anagain.Process(trk0, trk1) >= 0)
      {
        selected++;

        std::vector<double> qloss0  = anagain.GetQloss0();
        std::vector<TVector3> path0 = anagain.Get3dPath0();
        std::vector<double> qloss1  = anagain.GetQloss1();
        std::vector<TVector3> path1 = anagain.Get3dPath1();

        anares->Fill(qloss0, path0, qloss1, path1 );

      }
      //else
      //cout<<" this was "<<ev<<endl;

    }

  }
  cout<<"Used : "<<selected<<" out of "<<evmgr->GetEntries()<<endl;

  // finalize calculation
  anares->ComputeAreaMap();

  if(doappend)
  {
    int lemid;
    double d1, d2, d3, d4;
    for(size_t i=0;i<anares->GetNbins();i++)
    {
      lemid = (int)(i+1);
      anares->GetBinContent( i, 0, d1, d2 );
      anares->GetBinContent( i, 1, d3, d4 );
      sFile.WriteCharge( lemid, d1, d2, d3, d4 );
    }
  }

  // close event manager
  evmgr->Close();

  // write histograms
  if( !outfname.empty() )
  {
    TFile *fout = TFile::Open( outfname.c_str(), "RECREATE" );
    fout->cd();
    anares->Write( fout );
    fout->Close();
  }

  delete anares;
  delete evmgr;

}
