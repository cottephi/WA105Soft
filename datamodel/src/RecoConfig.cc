/////////////////////////////////////////////////////////////////////////////////////////
//
//  Class to hold configuration parameters for reconstruction algorithms
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "RecoConfig.h"  

using namespace std;

ClassImp(RecoConfig) 

//
//
//
RecoConfig::RecoConfig()
{
  fTaskName = "";
  fAlgoName = "";
  fInFlName = "";
  fRunTime  = 0;
}

//
//
//
RecoConfig::RecoConfig(std::string taskname)
{
  fTaskName = taskname;
  fAlgoName = "";
  fInFlName = "";
  fRunTime  = 0;
}


//
//
//
void RecoConfig::AddValue(std::string key, std::vector<float> &value)
{
  std::map< std::string, std::vector<float> >::iterator it;
  it = fAlgoPara.find(key);
  if(it != fAlgoPara.end())
    {
      std::cerr<<"ERROR: the key "<<key<<" already exists"<<endl;
      return;
    }
  
  fAlgoPara[key] = value;
}

//
//
//
std::vector<float> RecoConfig::GetValue(std::string key)
{
  std::vector<float> rvec;
  std::map< std::string, std::vector<float> >::iterator it;
  it = fAlgoPara.find(key);
  return it == fAlgoPara.end() ? std::vector<float>() : it->second;
}


//
//
//
void RecoConfig::Print()
{
  std::cout<<std::endl<<"Reco task name "<<fTaskName<<std::endl;
  std::cout<<"Reco algorithm "<<fAlgoName<<std::endl;
  std::map< std::string, std::vector<float> >::iterator it;
  for(it = fAlgoPara.begin();it!=fAlgoPara.end();it++)
    {
      std::cout<<it->first;
      std::vector<float> vec = it->second;
      for(size_t i=0;i<vec.size();i++)
	std::cout<<setw(10)<<vec[i];
      std::cout<<std::endl;
    }

  std::cout<<"Algorithm executed at "<<fRunTime<<std::endl;
}
