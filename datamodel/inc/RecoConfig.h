/////////////////////////////////////////////////////////////////////////////////////////
//
//  Class to hold configuration parameters for reconstruction algorithms
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RECOCONFIG_H_
#define _RECOCONFIG_H_

#include <vector>
#include <string>
#include <map>

#include <TObject.h>

class RecoConfig : public TObject
{
public:
  RecoConfig();
  RecoConfig(std::string taskname);
  ~RecoConfig(){;}
  
  void SetRunTime(unsigned int val){ fRunTime = val; }
  void SetTaskName(std::string val){ fTaskName = val; }
  void SetAlgoName(std::string val){ fAlgoName = val; }
  void SetInFileName(std::string val){ fInFlName = val; }

  unsigned int GetRunTime() { return fRunTime; }
  std::string& GetTaskName() { return fTaskName; }
  std::string& GetAlgoName() { return fAlgoName; }
  std::string& GetInFileName() { return fInFlName; }
  
  void Print();
  //template<typename T> T GetValue(std::string key);

  void AddValue(std::string key, std::vector<float> &value);
  std::vector<float> GetValue(std::string key);
  
 private:
  unsigned int fRunTime;
  std::string fTaskName;
  std::string fAlgoName;
  std::string fInFlName;
  std::map< std::string, std::vector<float> > fAlgoPara;
 
 public:  
  ClassDef(RecoConfig,1);
};
 

#endif
