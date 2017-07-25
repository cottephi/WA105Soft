////////////////////////////////////////////////////////////////////////////////////////////
//
//  CR track reconstruction task
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "CRTrackRecoTask.h"

//
//
//
CRTrackRecoTask::CRTrackRecoTask()
{
  fTrackRecAlgo = NULL;
  fTrackJoiner  = NULL;

  // namess of data branches for this task
  fDataProds.push_back("RecoCRTracks_View0");
  fDataProds.push_back("RecoCRTracks_View1");
}


//
//
//
CRTrackRecoTask::~CRTrackRecoTask()
{
  if(fTrackRecAlgo) delete fTrackRecAlgo;
  if(fTrackJoiner) delete fTrackJoiner;
}

//
//
//
void CRTrackRecoTask::Configure(RunConfig &runpara, RecoConfig &recopara)
{
  // configuration parameters
  fRecConf = recopara;

  std::string name = fRecConf.GetAlgoName();

  if( name.compare("AlgoMultiCRTrack") != 0 )
  {
    std::cerr<<"ERROR: CRTrackRecoTask::Configure() does not know how to handld "
    << name <<std::endl;
    exit(-1);
  }

  //std::vector<float> params = fRecConf.GetValue("PARA_TSEGREC");
  fTrackRecAlgo = new CRTrackSegRecon( fRecConf );
  fTrackJoiner  = new TrackJoinCRM( fRecConf );
}


//
//
//
void CRTrackRecoTask::ProcessEvent( Event &event )
{
  // could get the number of CRPs from geom config in principle
  //fNcrms = event.GetGeomConfig()

  std::string hits0br = "RecoHitsView0_Data";
  std::string hits1br = "RecoHitsView1_Data";

  const TObjArray *hitsview0 = event.Get(hits0br);
  if(!hitsview0)
  {
    std::cerr<<"ERROR: hit information for view 0 is not found"<<std::endl;
    exit(-1);
  }

  const TObjArray *hitsview1 = event.Get(hits1br);
  if(!hitsview1)
  {
    std::cerr<<"ERROR: hit information for view 1 is not found"<<std::endl;
    exit(-1);
  }

  std::cout<<"CR track rec found hits in this event "<<hitsview0->GetEntries()<<", "
  <<hitsview1->GetEntries()<<std::endl;

  std::vector<LArReco::Track2d> tracks_view0;
  std::vector<LArReco::Track2d> tracks_view1;


  FindTracksView( hitsview0, tracks_view0 );
  FindTracksView( hitsview1, tracks_view1 );

  std::cout<<" Tracks in view 0 "<<tracks_view0.size()<<std::endl;
  std::cout<<" Tracks in view 1 "<<tracks_view1.size()<<std::endl;

  event.GetEventHeader()->SetNumTrkCRP0( (int)tracks_view0.size() );
  event.GetEventHeader()->SetNumTrkCRP1( (int)tracks_view1.size() );

  // add new reco data to event
  event.Add(fDataProds[0], tracks_view0);
  event.Add(fDataProds[1], tracks_view1);

  // clean up
  tracks_view0.clear();
  tracks_view1.clear();
}


//
// find tracks in each view
//
void CRTrackRecoTask::FindTracksView( const TObjArray *hits, std::vector<LArReco::Track2d> &tracks )
{
  std::vector<HitCollection*> hitsView;

  // loop over hits and get hits for given CRM
  int lastcrm = -1;
  for(int i=0;i<hits->GetEntries();i++)
  {
    LArReco::QHit* hit = (LArReco::QHit*)hits->At(i);

    int currcrm = hit->GetCRM();
    if(lastcrm != currcrm)
    {
      hitsView.push_back( new HitCollection() );
      lastcrm = currcrm;
    }

    hitsView.back()->AddHit(hit);
  }


  std::vector< std::vector<LArReco::Track2d> > tracksegs( hitsView.size() );

  for(size_t i=0;i<hitsView.size();i++)
  {
    fTrackRecAlgo->FindTracks( hitsView[i], tracksegs[i] );
    //cout<<"Found "<< tracksegs[i].size() <<" tracks"<<endl;
  }

  MergeSegments( tracksegs, tracks );

  while(!hitsView.empty())
  {
    delete hitsView.back();
    hitsView.pop_back();
  }
}


//
//
//
void CRTrackRecoTask::MergeSegments( std::vector< std::vector<LArReco::Track2d> > &tracksegs,
  std::vector< LArReco::Track2d > &tracks )
  {
    // under construction ...

    // just merge all segments into one array
    // for(size_t i=0;i<tracksegs.size();i++)
    //   {
    //     tracks.insert( tracks.end(), tracksegs[i].begin(), tracksegs[i].end() );
    //   }
    fTrackJoiner->Join( tracksegs, tracks );
  }
