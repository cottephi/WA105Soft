////////////////////////////////////////////////////////////////////////////////
//
//   Class to define CRP and fill it with data
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DLARCRM_H__
#define __DLARCRM_H__

#include <vector>
#include <string>
#include <cmath>

#include "VoxelInfoCRM.h"
#include "VoxelInfo.h"
#include "DataFragment.h"
#include "DataCRM.h"
#include "ElecSim.h"
#include "NoiseGen.h"
#include "QProjector.h"

// channel pedestal
#include "ChPedestal.h"

#define NVIEWS 2

namespace LArQReadMC
{
  class DLArCRM
  {
    
  public:
    DLArCRM( int id, float x0, float y0, float z0, float wx, float wy, 
	     float pitchx, float pitchy, float gain, 
	     unsigned int nchansx, unsigned int nchansy,
	     unsigned int nsamples, BasicDigitizer *elecsim );
    ~DLArCRM();
    
    void SetCollEffMap( std::string fname )
    { m_QProj->LoadEffFile( fname ); }

    // call to save voxel info to specified object
    void SaveVoxelInfo(LArVoxel::VoxelInfoCRM* vxlcrm) { m_vxlcrm = vxlcrm; }
    void SaveRawData(LArRawData::DataCRM* datcrm, LArRawData::Dtype dtype)
    {       
      m_dtype  = dtype; 
      m_datcrm = datcrm; 
    }
    
    int   GetCRMId() const { return m_id; }
    float GetX0() const { return m_pos[0]; }
    float GetY0() const { return m_pos[1]; }
    float GetZ0() const { return m_pos[2]; }
    float GetWidthX() const { return m_width[0]; }
    float GetWidthY() const { return m_width[1]; }
    float GetGain() const { return m_gain; }
    float GetPitch(int iview) const;
    
    bool isInsideCRM(float x, float y) const;
    
    int GetTimeBin(float tval) const
    { return m_QProj->GetTimeBin(tval); }
      
    
    // quantize data
    void   Quantize(bool val){ m_quantize = val; }

    //actions to do at the begining of event
    void   BeginEvent(); 
    //actions to do at the end of event
    void   EndEvent();
    
    // add noise
    void   AddNoise(std::vector<BasicNoise*> &Noise);
    
    void Fill( int pid, double edep, double edepq,
	       double xvox, double yvox, double zvox,
	       double tvox, double dt, double x, double y,
	       double q, double sigmaL, double sigmaT );
    
  private:
    
    // add data
    void AddData(float qval, int tbin, LArRawData::DataFragment<float>* frag);

    // shaping
    void ApplyShaping(LArRawData::DataFragment<float>* qdata);

    LArVoxel::VoxelInfo     *m_vxl;
    LArVoxel::VoxelInfoCRM  *m_vxlcrm;
    LArRawData::DataCRM     *m_datcrm;
    LArRawData::Dtype        m_dtype;
    int   m_id; 
    float m_pos[3];
    float m_width[2];
    float m_gain;
    float m_pitch[NVIEWS];
    float m_ped;
    float m_pedrms;
    float m_freq;
    int   m_bitadc;
    float m_mVtoADC;
    bool  m_quantize;
    
    // projector for charge
    QProjectorCRM *m_QProj;

    unsigned int m_nchans[NVIEWS];
    unsigned int m_nsamples;
    std::vector<float> m_respvec;  // electronics response
    
    // temporary storage of simulation data
    std::vector< std::vector< LArRawData::DataFragment<float>* > > m_fltdata;

    // calibration data
    calidaq::ChPedestal *fChPed;
  };

} // end name space


#endif
