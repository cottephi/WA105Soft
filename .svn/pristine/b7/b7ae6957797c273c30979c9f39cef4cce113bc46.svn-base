#include "DLArCRM.h"

LArQReadMC::DLArCRM::DLArCRM(int id, float x0, float y0, float z0,
			     float wx, float wy, float pitchx, float pitchy,
			     float gain, unsigned int nchansx, unsigned int nchansy,
			     unsigned int nsamples, BasicDigitizer *elecsim)
{
  m_id       = id;

  m_pos[0]   = x0; // in cm
  m_pos[1]   = y0; // in cm
  m_pos[2]   = z0; // in cm
  m_width[0] = wx; // width in cm
  m_width[1] = wy; // width in cm

  m_nchans[0] = nchansx;  // number of channels in x
  m_nchans[1] = nchansy;  // number of channels in y
  m_nsamples  = nsamples; // number of readout samples
  m_pitch[0]  = pitchx;   // pitch for x view
  m_pitch[1]  = pitchy;   // pitch for y view
  m_gain      = gain;     // effective gain


  // do basic checks
  for(int i=0;i<NVIEWS;i++)
    {
      if(m_width[i] < m_pitch[i] * m_nchans[i])
	{
	  std::cout<<"WARNING: dimensions of CRM mismatches number x channel pitch"
		   <<"         resizing the width in view "<<i<<std::endl;
	  m_width[i] = m_pitch[i] * m_nchans[i];
	}
    }

  // properties of electronics
  m_ped      = elecsim->GetPedestal();   // in ADC
  m_pedrms   = elecsim->GetPedRMS();     // in ADC
  m_freq     = elecsim->GetSampleFreq(); // in MHz
  m_bitadc   = elecsim->GetBitsADC();    // number of bits


  fChPed = NULL;
  if( m_ped < 0 )
    {
      m_ped = 0;
      fChPed = &calidaq::ChPedestal::Instance();
    }

  // our charge rojector operator
  m_QProj = new QProjectorCRM( m_pos[0], m_pos[1], m_pitch[0], m_pitch[1], 1.0/m_freq,
			       m_nchans[0], m_nchans[1], nsamples );

  // compute response vector
  elecsim->GetRespVector(m_respvec);
  m_mVtoADC = elecsim->GetmVtoADC();

  // init temporary data storage
  for(int i=0;i<NVIEWS;i++)
    {
      std::vector< LArRawData::DataFragment<float>* > viewdata;
      for(UInt_t ch=0;ch<m_nchans[i];ch++)
	{
	  // where to store intermediate data
	  viewdata.push_back(new LArRawData::DataFragment<float>());
	}
      //std::cout<<" iview = "<<i<<" "<<viewdata.size()<<std::endl;
      m_fltdata.push_back(viewdata);
    }


  m_vxl    = NULL;
  m_vxlcrm = NULL;
  m_datcrm = NULL;
}

///////////////////////////////////////////////////////////////////////////
// dtor
LArQReadMC::DLArCRM::~DLArCRM()
{
  // clean up temporary array
  for(size_t i=0;i<m_fltdata.size();i++)
    {
      while(!m_fltdata[i].empty())
	{
	  delete m_fltdata[i].back();
	  m_fltdata[i].pop_back();
	}
    }

  delete m_QProj;
}

///////////////////////////////////////////////////////////////////////////
// get pitch
float LArQReadMC::DLArCRM::GetPitch(int iview) const
{
  if(iview < 0 || iview >= NVIEWS)
    {
      std::cerr<<"ERROR: wrong view index"<<std::endl;
      return -1.0;
    }

  return m_pitch[iview];
}

///////////////////////////////////////////////////////////////////////////
// is inside CRM?
bool LArQReadMC::DLArCRM::isInsideCRM(float x, float y) const
{
  bool rval = ( (fabs(x - m_pos[0]) < 0.5*m_width[0]) &&
		(fabs(y - m_pos[1]) < 0.5*m_width[1]) );
  return rval;
}


///////////////////////////////////////////////////////////////////////////
// fill with data
void LArQReadMC::DLArCRM::Fill( int pid, double edep, double edepq,
				double xvox, double yvox, double zvox, double tvox,
				double dt, double x, double y, double q,
				double sigmaL, double sigmaT )
{
  if(!isInsideCRM(x, y) && sigmaT == 0) return;

  //! tval should be global time plus drift time !
  float tval = dt + tvox;
  int tbin   = GetTimeBin(tval);
  if( tbin < 0 && sigmaL == 0 ) return;

  // project charge
  float qtot = q * m_gain;
  const LArMCTruth::AnodeTrueQ* anodeQMap = m_QProj->Projection(x, y, tval, qtot, sigmaL, sigmaT);
  if(!anodeQMap)
    return;

  if(!anodeQMap->IsValid())
    return;

  // add data to our buffer


  // view 0
  const std::vector<LArMCTruth::ChanTrueQ*> view0Ch = anodeQMap->View0();
  for(size_t i=0;i<view0Ch.size();i++)
    {
      const std::vector<Float_t> tdata = view0Ch[i]->GetData();
      UInt_t tbin = view0Ch[i]->GetTstart();
      UInt_t ch   = view0Ch[i]->GetChan();
      for(size_t j=0;j<tdata.size();j++)
	{
	  AddData(tdata[j], tbin+j, m_fltdata[0][ch]);
	}
    }

  // view 1
  const std::vector<LArMCTruth::ChanTrueQ*> view1Ch = anodeQMap->View1();
  for(size_t i=0;i<view1Ch.size();i++)
    {
      const std::vector<Float_t> tdata = view1Ch[i]->GetData();
      UInt_t tbin = view1Ch[i]->GetTstart();
      UInt_t ch   = view1Ch[i]->GetChan();
      for(size_t j=0;j<tdata.size();j++)
	{
	  AddData(tdata[j], tbin+j, m_fltdata[1][ch]);
	}
    }


  // add voxel ?
  if(m_vxlcrm)
    m_vxl = m_vxlcrm->AddVoxel( pid, edep, edepq, xvox, yvox, zvox, tvox,
				qtot, x, y, tval, sigmaL, sigmaT );
}


///////////////////////////////////////////////////////////////////////////
// accumulate data
void LArQReadMC::DLArCRM::AddData(float qval, int tbin,
				  LArRawData::DataFragment<float>* frag)
{
  //add charge w/o response smearing (add in the end of event)
  frag->AddDatum(tbin, qval, false);

  // shape data
  /*
  for(size_t i=0;i<m_respvec.size();i++)
  {
    UInt_t bin = (UInt_t) tbin + i;
      if(bin >= frag->GetFragEnd())
	break;
      frag->AddDatum(bin, qval*m_respvec[i], false);
    }
  */
}

///////////////////////////////////////////////////////////////////////////
// Begin event initialization procedures
void LArQReadMC::DLArCRM::BeginEvent()
{
  std::cout<<"Begin event for CRM "<<m_id<<std::endl;
  // init data arrays
  for(size_t i=0;i<m_fltdata.size();i++)
    {
      for(size_t ch=0;ch<m_fltdata[i].size();ch++)
	{
	  m_fltdata[i][ch]->InitFragment(0, m_nsamples);
	}
    }

  // reset voxel info arrays
  if(m_vxlcrm) m_vxlcrm->Clear();

  //std::cout << "voxel CRM = "<<m_vxlcrm << std::endl;

  //std::cout<<m_fltdata.size()<<" "<<m_fltdata[0].size()<<std::endl;
}

///////////////////////////////////////////////////////////////////////////
// End event initialization procedures
void LArQReadMC::DLArCRM::EndEvent()
{
  std::cout<<"EndEvent processes for CRM "<<m_id<<std::endl;

  if(!m_datcrm)  // no raw data output
    return;

  bool isFloat   = (m_dtype == LArRawData::DTID_FLOAT);
  bool isUShort  = (m_dtype == LArRawData::DTID_USHORT);

  // always quantize if our data type is integer
  bool quant     = (m_quantize || isUShort);

  float maxadc   = pow(2, m_bitadc) - 1;

  // peform final action
  // add noise

  // add pedestal, quantize and copy data
  for(size_t iview=0;iview<m_fltdata.size();iview++)
    {
      for(size_t ch=0;ch<m_fltdata[iview].size();ch++)
	{
	  LArRawData::DataChan *chan = m_datcrm->GetChan(iview, (Int_t)ch);
	  if(!chan) //first time --> create a channel
	    chan = m_datcrm->AddDataChannel( iview, (Int_t)ch, m_dtype,
					     LArRawData::FULLDATA );

	  if(((Int_t)ch != chan->GetChanId()))
	    {
	      std::cerr<<"ERROR : channel ID codes appear to be mixed up"<<std::endl;
	      continue;
	    }

	  // shape signal with pre-amp response
	  ApplyShaping( m_fltdata[iview][ch] );

	  //std::cout<<" Channel id "<<chan->GetChanId()<<std::endl;
	  //std::cout<<" GetNFrags  "<<chan->GetNFrags()<<std::endl;

	  TObject* dfobj = chan->GetDataFragment(0);

	  //std::cout<<" Fragment "<<dfobj<<std::endl;

	  if(!dfobj) //first time --> create a fragment
	    dfobj = chan->AddDataFragment();

	  //std::cout<<" Added fragment "<<dfobj<<std::endl;

	  // initialize fragments (should be only one)
	  chan->InitFragments( m_fltdata[iview][ch]->GetFragStart(),
			       m_fltdata[iview][ch]->GetFragSize() );

	  // loop over the data and add them to the fragment
	  for(size_t i=m_fltdata[iview][ch]->GetFragStart();
	      i<m_fltdata[iview][ch]->GetFragEnd();i++)
	    {

	      //m_fltdata[iview][ch]->AddDatum(i, m_ped, false);

	      float val = m_fltdata[iview][ch]->GetDatum(i);

	      // quantize & add pedestal
	      if( quant )
		{
		  float ped    = m_ped;
		  float pedrms = m_pedrms;
		  if( fChPed )
		    {
		      ped = fChPed->Pedestal( m_id, iview, ch, pedrms );
		    }
		  //float ped = m_ped;
		  if(pedrms > 0) ped += gRandom->Gaus(0, pedrms);
		  //val = floor(val / m_mVtoADC + 0.5 + m_ped);
		  val = floor(val / m_mVtoADC + 0.5 + ped);
		}

	      // clip at max val
	      if(val > maxadc) val = maxadc;

	      if( isUShort )
		{
		  if(val < 0 ) val = 0; //zero
		  static_cast< LArRawData::DataFragment<UShort_t>* >(dfobj)->AddDatum(i, (UShort_t)(val), true);
		}
	      else if(isFloat)
		static_cast< LArRawData::DataFragment<Float_t>* >(dfobj)->AddDatum(i, val, true);
	    }
	}
    }
  std::cout<<"Finish EndEvent "<<std::endl;
}

// Add noise
void LArQReadMC::DLArCRM::AddNoise(std::vector<BasicNoise*> &Noise)
{
  std::cout << " Adding noise ... " << std::endl;
  for(size_t iview=0;iview<m_fltdata.size();iview++)
    {
      for(size_t ch=0;ch<m_fltdata[iview].size();ch++)
	{
	  std::vector<float> myData;
	  m_fltdata[iview][ch]->GetData(myData);
	  for(size_t j=0;j<Noise.size();j++)
	    {
	      bool newevent = (ch == 0);
	      //std::cout << newevent << " " << Noise[j]->GetCoherent() << std::endl;
	      Noise[j]->SetNewEvent( newevent );
	      Noise[j]->AddNoise( myData );
	    }
	  m_fltdata[iview][ch]->SetData(m_fltdata[iview][ch]->GetFragStart(), myData);
	}
    }

}

// apply shaping
void LArQReadMC::DLArCRM::ApplyShaping(LArRawData::DataFragment<float>* qdata)
{
  std::vector<float> tmpvec;
  tmpvec.resize(qdata->GetFragSize(), 0);
  UInt_t fstart = qdata->GetFragStart();
  size_t N  = tmpvec.size();
  size_t Nr = m_respvec.size();
  for(size_t i=0;i<N;i++)
    {
      float qval = qdata->GetDatum( fstart + i );
      if(qval == 0) continue;
      for(size_t j=0;j<Nr;j++)
	{
	  if((i + j) >= N) break;
	  tmpvec[i+j] += qval * m_respvec[j];
	}
    }

  qdata->SetData(fstart, tmpvec);
}
