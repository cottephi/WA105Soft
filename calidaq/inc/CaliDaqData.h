//////////////////////////////////////////////////////////////////////////////
// 
//  Calibration information for each channel base class
//
//  Convert the 3-fold key (CRM, view, chan) complexity into a single index.
//  This is done by mapping bits into 32bit integer
//  Current mapping:
//      - 10 bits LSB are ch number, so up-to 1023
//      - 11-12 bits are view, so up-to 4 (more than enough)
//      - 13-20 bits are CRM number, so up-to 255 (also more than enough)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __CALIDAQDATA_H__
#define __CALIDAQDATA_H__

#include <vector>
#include <string>
#include <stdint.h>

namespace calidaq
{

  static std::string chmaskfile = "chmasked";
  static std::string pedfile    = "pedestals";
  static std::string califile   = "calibrations";
  
  class CaliDaqData
  {
  public:
    CaliDaqData();
    ~CaliDaqData();
    
    //
    virtual void Print() = 0;

  protected:
    std::string GetPath() { return m_path; }
    
    // read constants from file
    virtual void LoadFile( std::string &fname ) = 0;
    
    //
    ssize_t  addIndx(uint32_t idx);
    ssize_t  findIt(uint32_t idx);
    uint32_t mapIt(uint16_t icrm, uint16_t iview, uint16_t ich);

    // inverse operation
    void invmapIt(uint32_t idx, uint16_t &icrm, uint16_t &iview, uint16_t &ich);

    std::string m_path;
    std::vector<uint32_t> m_chid;

  private:
    uint8_t _crmMask; // 8 bits for CRP module
    uint8_t _vwMask;  // 2 bits for view
    uint16_t _chMask; // 10 bits for ch number
    
    // vector of unique channal ids
    std::vector<uint32_t>::iterator _it;
  };
}

#endif
