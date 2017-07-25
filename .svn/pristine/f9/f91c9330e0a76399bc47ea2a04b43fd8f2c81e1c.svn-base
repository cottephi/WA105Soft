#include "RawChDataProcess.h"

using namespace std;


// RawChDataROI
RawChDataROI::RawChDataROI()
{
  m_ped      = 0.0;
  m_pedrms   = 0.0;
  m_nsigth   = 4.0;
  m_nsiglow1 = 0.5;   // ped - m_nsiglow1 x ped_rms on 1st pass
  m_nsiglow2 = -0.01; // ped - m_nsiglow2 x ped_rms on 2nd pass

  m_padleft  = 5;
  m_padright = 10;
}

//
RawChDataROI::~RawChDataROI()
{;}
  

//
//
// ProcessRawChData find regions of interest in raw channel data
size_t RawChDataROI::FindROI( LArRawData::DataChan *chdata, 
			      std::vector< LArRawData::DataFragment<Float_t> > &aFrags,
			      float thresh_guess, bool padroi )
{
  if (chdata->GetChanStatus() != LArRawData::FULLDATA)
    {
      cerr<<"ERROR: only full channel data can be used "<<endl;
      return 0;
    }
  aFrags.clear();

  // get a vector of floats
  std::vector<float> data;
  if(chdata->GetDataType() == LArRawData::DTID_USHORT)
    {
      LArRawData::DataFragment<UShort_t>* frag = (LArRawData::DataFragment<UShort_t>*)chdata->GetDataFragment(0);
      if(frag->GetDataFloat(data) != 0)
	{
	  cerr << "ERROR: the fragment does not start at 0 "<<endl;
	  return 0;
	}	    
    }
  else if(chdata->GetDataType() == LArRawData::DTID_FLOAT)
    {
      LArRawData::DataFragment<Float_t>* frag = (LArRawData::DataFragment<Float_t>*)chdata->GetDataFragment(0);
      if(frag->GetData(data) != 0)
	{
	  cerr << "ERROR: the fragment does not start at 0 "<<endl;
	  return 0;
	}
    }
  
  std::vector< std::pair<size_t, size_t> > subsamples;
  FindROI( data, thresh_guess, subsamples, padroi );

  // save computed pedestal and rms
  chdata->SetPed(m_ped);
  chdata->SetPedRMS(m_pedrms);
  
  // add found fragments to the array
  for(size_t i=0;i<subsamples.size();i++)
    {
      LArRawData::DataFragment<Float_t> frag;
      
      std::vector<float> subdata;
      //cout << i << " " << data.size()<< " " <<subsamples[i].first << " " <<subsamples[i].second << endl;
      for(size_t j=subsamples[i].first;j<subsamples[i].second;j++)
	{
	  // pedestal subtracted data
	  subdata.push_back( data[j] - m_ped );
	}
      frag.SetData(subsamples[i].first, subdata);
      aFrags.push_back( frag );
    }

  //
  return subsamples.size();
}

//
//
// ProcessRawChData find regions of interest in raw channel data
size_t RawChDataROI::FindROI( LArRawData::DataChan *chdata, 
			      std::vector< LArRawData::DataFragment<Float_t> > &aFrags,
			      float ped, float pedrms, bool padroi )
{
  if (chdata->GetChanStatus() != LArRawData::FULLDATA)
    {
      cerr<<"ERROR: only full channel data can be used "<<endl;
      return 0;
    }
  aFrags.clear();

  // get a vector of floats
  std::vector<float> data;
  if(chdata->GetDataType() == LArRawData::DTID_USHORT)
    {
      LArRawData::DataFragment<UShort_t>* frag = (LArRawData::DataFragment<UShort_t>*)chdata->GetDataFragment(0);
      if(frag->GetDataFloat(data) != 0)
	{
	  cerr << "ERROR: the fragment does not start at 0 "<<endl;
	  return 0;
	}	    
    }
  else if(chdata->GetDataType() == LArRawData::DTID_FLOAT)
    {
      LArRawData::DataFragment<Float_t>* frag = (LArRawData::DataFragment<Float_t>*)chdata->GetDataFragment(0);
      if(frag->GetData(data) != 0)
	{
	  cerr << "ERROR: the fragment does not start at 0 "<<endl;
	  return 0;
	}
    }

  // set to the external pedestal
  m_ped    = ped;
  m_pedrms = pedrms;
  float thresh    = m_ped + m_nsigth * m_pedrms;
  float lowbound  = m_ped - m_nsiglow2 * m_pedrms;

  if(m_pedrms < 0.1)
    {
      thresh   = m_ped + 2;
      lowbound = m_ped + 0.5;
    }

  std::vector< std::pair<size_t, size_t> > subsamples;
  std::vector< size_t > pedsamples;
  float minval;
  FindROI(data, thresh, lowbound, subsamples, pedsamples, minval, padroi);
  
  // save pedestal and rms for this channel
  chdata->SetPed(m_ped);
  chdata->SetPedRMS(m_pedrms);
  
  // add found fragments to the array
  for(size_t i=0;i<subsamples.size();i++)
    {
      LArRawData::DataFragment<Float_t> frag;
      
      std::vector<float> subdata;
      for(size_t j=subsamples[i].first;j<subsamples[i].second;j++)
	{
	  // pedestal subtracted data
	  subdata.push_back( data[j] - m_ped );
	}
      frag.SetData(subsamples[i].first, subdata);
      aFrags.push_back( frag );
    }

  //
  return subsamples.size();
}

//
//
// pad regions of interest with extra data samples
void RawChDataROI::PadROI( std::vector< std::pair<size_t, size_t> > &rois, 
			   size_t padleft, size_t padright, size_t maxsize )
{
  for(size_t i=0;i<rois.size();i++)
    {
      size_t startidx = rois[i].first;
      size_t endidx   = rois[i].second;
      
      if(i == 0 && startidx < padleft)
	startidx = 0;
      else 
	startidx -= padleft;
      
      endidx += padright;
      if(endidx > maxsize) endidx = maxsize;
      
      //check for overlaps
      if(i > 0)
	{
	  if(rois[i-1].second >= startidx)
	    {
	      startidx = rois[i-1].second + 1;
	    }	  
	}
      
      rois[i].first  = startidx;
      rois[i].second = endidx;
    }
}
