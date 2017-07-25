///////////////////////////////////////////////////////////////////////////////////////
//
// File with results of online analysis 
// Assumed format
//  DDMMYY_HHMMSS
//  etau_vw0 etau_vw0_err etau_vw1 etau_vw1_err (in us)
//  lem_id dqds_vw0 dqds_vw0_err dqds_vw1 dqds_vw1_err
//  ...
//
// CRPAnaSummaryFile::Open() will open a file for RW if it exists
//                           otherwise the file will be opened for W
// Write to different field can be done with an appropriate Write function
// Note that it is not possible to write if a particular datum had already been read
//
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __CRPANASUMMARYFILE_H__
#define __CRPANASUMMARYFILE_H__

#include <string>
#include <fstream>
#include <vector>
#include <ctime>

class CRPAnaSummaryFile
{
 public:
  CRPAnaSummaryFile();
  ~CRPAnaSummaryFile();
  
  void Open(std::string fname);
  void Close();

  time_t GetTime() const;
  int GetPurity(double &p0, double &pe0, double &p1, double &pe1) const;
  int GetCharge(int lemid, double &dq0, double &dq0e, double &dq1, double &dq1e) const;
  
  void WriteTime(time_t val);
  void WritePurity(double p0, double pe0, double p1, double pe1);
  void WriteCharge(int lemid, double dq0, double dq0e, double dq1, double dq1e);
  
  
 private:
  void ReadLines();
  bool AddToData(int id, double d1, double d2, double d3, double d4);
  
  // check if the filestream can alreadby be read
  bool fstreamExists( const char *name )
  {
    std::ifstream infile( name );
    return infile.good();
  }

  std::fstream fFile;
  
  typedef struct datafield_t
  {
    int id;
    double data[4];
  } datafield_t;
  
  std::string fTFormat;
  size_t fLines;
  time_t fTime;
  std::vector<datafield_t> fData;
  
};

#endif
