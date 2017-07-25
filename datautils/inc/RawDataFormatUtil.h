///////////////////////////////////////////////////////////////////////////////////////
//
//
//  Cast raw data into appropriate format for CRP readout
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __RAWDATAFORMATUTIL_H__
#define __RAWDATAFORMATUTIL_H__

#include "dlardaq.h"
#include "ChMapInterface.h"

#include "RunHeader.h"
#include "GeomConfig.h"
#include "EventHeader.h"
#include "DLArCRP.h"

//
//
//
// find search path from env
void GetSearchPathsFromEnv(std::string envname, 
			   std::vector<std::string> &paths);

//
//
//
//
class RawDataFormatUtil
{
 public:
  RawDataFormatUtil( RunHeader *rh, GeomConfig *gc,
		     EventHeader *eh, LArRawData::DataCRP *crpdata );

  ~RawDataFormatUtil(){;}

  void FillRun(dlardaq::runheader_t &rh);
  void FillEvent(dlardaq::evheader_t &eh, std::vector<dlardaq::adc16_t> &adc);

  // to improve ...
  double GetEfield(){ return 500.0; } //in V/cm
  
  short  GetNAdc() const { return m_nadc; }
  size_t GetNCh() const { return m_nch; }
  size_t GetNSample() const { return m_nsample; }

 protected:
  
  void InitGeoConfig( std::string gcfile );
  std::string GeoConfigFileName( int geo_type );
  
  // search paths for the config file
  std::vector<std::string> m_spath;
  
  // 
  std::string m_gcfile;
  
  short  m_nadc;
  size_t m_nch;
  size_t m_nsample;

  RunHeader *m_runh;
  GeomConfig *m_gconf;
  EventHeader *m_eveh;
  LArRawData::DataCRP *m_crp;
  
  // map for DAQ channels
  std::string m_chmap_name;
};

#endif
