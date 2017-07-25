#ifndef __CR_FLUX_hh__
#define __CR_FLUX_hh__

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

#include <TRandom3.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

typedef struct{
  Float_t x,y,z,dirx,diry,dirz,eneKin,time;
  Int_t pdg;
} particle_fld;

// base class for CR inputs
class WA105CRFlux
{
public:
  WA105CRFlux();
  ~WA105CRFlux();
  
  virtual void SetNormRate(float val)
  { m_normrate = val; }

  float GetDetX() { return m_detx; }
  float GetDetY() { return m_dety; }
  float GetDetZ() { return m_detz; }
  float GetSizeX() { return m_szx; }
  float GetSizeY() { return m_szy; }
  float GetSizeZ() { return m_szz; }
  float GetRate() { return m_rate; }
  float GetNormRate() { return m_normrate; }
  
  void SetSeed(int seed);

  // To be implemented by specific class
  virtual int GetCosmics(float t0, float twin, 
			 std::vector<particle_fld> &crflux) = 0;
  int GetTiming(float t0, float twin, std::vector<float> &evtimes);

protected:

  //Random number generator
  int m_seed;

  float m_detx; // Detector X center in CR simulation
  float m_dety; // Detector Y center in CR simulation
  float m_detz; // Detector Z center in CR simulation
  float m_szx;  // Detector X size in CR simulation
  float m_szy;  // Detector Y size in CR simulation
  float m_szz;  // Detector Z size in CR simulation

  float m_normrate; // Cosmic rate
  float m_rate;     // Detector Rate normalization
};


// class to read CRY output files
class WA105CRYFlux : public WA105CRFlux
{

public:
  WA105CRYFlux();
  ~WA105CRYFlux();
  
  void OpenFile(const char *fname);
  void SetNormRate(float val);
  Long64_t GetNentries(){ return m_ntot; }

  int GetCosmics(float t0, float twin, std::vector<particle_fld> &crflux);
  int GetEntryVector(float t0, float twin, std::vector<float> &evtimes, 
		     std::vector<Long64_t> &events);
  int GetEntry(Long64_t eventnum, particle_fld &crevent);
  
private:
  TFile     *m_fin;
  TTree     *m_tree;
  Long64_t   m_ntot;
  float     m_runtime;
  float     m_murate;
  particle_fld aParticle;
  std::vector<Long64_t> m_used;

};


#endif
