#include "WA105CRFlux.h"

using namespace std;

// ctor for base class
WA105CRFlux::WA105CRFlux()
{
  m_detx = m_dety = m_detz = 0;
  m_szx  = m_szy  = m_szz  = 0;
  m_rate = 0;

  m_normrate = 166;     // 1/m2/s

  //m_seed    = 20150305; // default: change with SetSeed
  //gRandom->SetSeed( m_seed );
}

// dtor for base class
WA105CRFlux::~WA105CRFlux()
{;}

// timing info
int WA105CRFlux::GetTiming(float t0, float twin, std::vector<float> &evtimes)
{
  evtimes.clear();

  // generate time distribution over a slightly larger time window
  float tstart    =  1.1*t0;          //in s
  float tend      =  1.1*(t0 + twin); //in s
  int count       = 0;
  
  if(tstart >= tend ) // make just a single event at a given time t0
    {
      evtimes.push_back(t0);
      count = 1;
    }
  else
    {
      float mytime = tstart;
      cout<<"Generating event timing with "<<m_rate<<" muon/s"<<endl;
      
      while(mytime <= tend)
	{
	  float tevent = gRandom->Exp(1.0/m_rate); //per s
	  
	  mytime += tevent;
	  if(mytime > tend)
	    break;
	  
	  if(mytime >= t0 && mytime <= (t0+twin))
	    {
	      count++;
	      evtimes.push_back(mytime);
	    }
	}
    }
  
  cout<<"Generated "<<count<<" CR events in time window ["
      <<t0<<", "<<t0+twin<<"] s"<<endl;

  return count;
}


//////////////////////////////////////////////////////////////////////////
//
// CRY flux file input
//
//////////////////////////////////////////////////////////////////////////

// ctor
WA105CRYFlux::WA105CRYFlux()
{
  m_rate = 0;
  m_fin  = NULL;
  m_tree = NULL;

  m_runtime = 0;
  m_murate  = 0;
  
  m_detx = 0;
  m_dety = 0;
  m_detz = 0;

  m_szx = 0;
  m_szy = 0;
  m_szz = 0;
}

// dtor
WA105CRYFlux::~WA105CRYFlux()
{
  if(m_fin)
  if(m_fin->IsOpen())
    m_fin->Close();
}

void WA105CRYFlux::OpenFile(const char *fname)
{
  if(m_fin)
    if(m_fin->IsOpen())
      m_fin->Close();
      
  m_fin = TFile::Open(fname);
  
  m_tree = (TTree*)m_fin->Get("cosmic");
  m_tree->SetBranchAddress("cosmic", &aParticle);
  m_ntot = m_tree->GetEntries();
  
  //Get info
  TH1F *hInfo = (TH1F*)m_tree->GetUserInfo()->FindObject("hInfo");
						
  m_runtime = hInfo->GetBinContent(1);
  m_murate  = hInfo->GetBinContent(2);
  
  m_detx = hInfo->GetBinContent(6);
  m_dety = hInfo->GetBinContent(7);
  m_detz = hInfo->GetBinContent(8);

  m_szx = hInfo->GetBinContent(9);
  m_szy = hInfo->GetBinContent(10);
  m_szz = hInfo->GetBinContent(11);

  //if(m_normrate > 0)
  m_rate = m_ntot / m_runtime; //*m_pdgrate/m_murate;

  cout<<"In the file: "<<fname<<endl;
  cout<<"   Total entries = "<<m_ntot<<endl;
  cout<<"   Total runtime = "<<m_runtime<<" s"<<endl;
  cout<<"   Rate norm     = "<<m_rate<<" rays/s"<<endl;
  cout<<"   Mu rate       = "<<m_murate<<" muons/m2/s"<<endl;
}

//
void WA105CRYFlux::SetNormRate(float val)
{
  if(val <= 0) return;

  m_normrate = val;
  m_rate     = m_ntot / m_runtime * m_normrate / m_murate;
}

//
int WA105CRYFlux::GetEntryVector(float t0, float twin,  std::vector<float> &evtimes,
				 std::vector<Long64_t> &events)
{
  evtimes.clear();
  events.clear();
  // get event timing
  int nev = GetTiming(t0, twin, evtimes);

  if(!nev) return nev; //why 0?

  if( (m_ntot - m_used.size()) <= (size_t)nev )
    {
      cout<<"WARNING: not enough unused entries left in the cosmic file"<<endl;
      cout<<"         will reset now for re-use."<<endl;
      m_used.clear();
    }

  // get current size
  size_t startpos = m_used.size();

  //now draw lottery tickets
  vector<Long64_t>::iterator it;
  Long64_t skip  = 0;
  int count = 0;
  while(count < nev && skip < m_ntot)
    {
      Long64_t val = (Long64_t)( gRandom->Uniform(0, m_ntot) );
      it = find(m_used.begin(), m_used.end(), val);
      if( it == m_used.end() ) //this entry has not been used already
	{
	  m_used.push_back(val);
	  count++;
	}
      else
	{
	  skip++;
	  continue;
	}
    }
  
  if(skip >= m_ntot)
    {
      cerr<<"ERROR: too many events were skipped in the CRY file"<<endl;
      cout<<skip<<endl;
      return 0;
    }
  
  for(size_t i=startpos;i<m_used.size();i++)
    events.push_back( m_used[i] );
  
  return nev;
}

//
int WA105CRYFlux::GetEntry(Long64_t eventnum, particle_fld &crevent)
{
  if(eventnum >= m_ntot || eventnum < 0)
    return 0;
  
  int rval = m_tree->GetEntry(eventnum);
  crevent = aParticle;
  return rval;
}


// 
int WA105CRYFlux::GetCosmics(float t0, float twin, 
			     std::vector<particle_fld> &crflux)
{

  crflux.clear();

  // get event timing
  vector<float> evtimes;
  vector<Long64_t> events;

  int nev = GetEntryVector(t0, twin, evtimes, events);

  if(!nev) return nev; //why 0?


  particle_fld crparticle;
  for(size_t i=0;i<events.size();i++)
    {
      if(!GetEntry(events[i], crparticle))
	continue;
      crparticle.time  = evtimes[i];
      crflux.push_back(crparticle);
      
      //cout<<i<<" "<<events[i]<<" "<<crparticle.x<<" "<<crparticle.y<<" "
      //<<crparticle.z<<" "<<crparticle.pdg<<endl;
    }
  
  return nev;
}


// set random number generator seed
void WA105CRFlux::SetSeed(int seed)
{
  m_seed = seed; 
  gRandom->SetSeed(m_seed);
}



// to test
// root[0] .L WA105CRFlux.cc+
// root[1] run("cryrootfile.root", someseed)
/*
void run(const char *fname, int seed = 121)
{
  float t0   = -4.0;
  float twin = 8.0;           // in ms
  float vdrift = 1.5;         // in m/ms 
  float tmin = t0;
  float tmax = t0 + twin;
  vector<particle_fld> crflux;
  
  WA105CRYFlux *crflx = new WA105CRYFlux();//fname);
  crflx->SetSeed(seed);
  crflx->OpenFile(fname);
  //crflx->SetNormRate(166.0);
  
  crflx->GetCosmics(t0/1000.0, twin/1000.0, crflux);

  TH1F *htime = new TH1F("htime", "htime", 20000, tmin, tmax);
  htime->SetDirectory(0);
  htime->GetXaxis()->SetTitle("T (ms)");
  
  for(size_t i=0;i<crflux.size();i++)
    {
      if(crflux[i].time<= t0) 
	cout<<crflux[i].time<<endl;;
      htime->Fill( crflux[i].time * 1000);
    }

    
  //for(int i=0;i<crflx->nCRYgen;i++)
    //{
      //cout<<i<<" "<<crflx->CRYEventId[i]<<" "<<crflx->CRYEventTime[i]<<endl;
   // }
  
  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->cd();
  htime->Draw();
  
}
*/
