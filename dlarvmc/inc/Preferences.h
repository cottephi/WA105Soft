/* fullreco -- Full reconstruction software
 * (C) 2000 - 2004,  The fullreco Team
 *
 * fullreco is mainly intended for academic purposes. Proper
 * credit must be given if you use fullreco or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Basic class for main event loop, reconstruct and analysis
   do not handle without care
   A.Bueno/A. Rubbia Oct 2000
   J.Rico Nov 2001
*/

#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include <string>
#include <sstream>
#include <iostream>
#include "TString.h"


//using namespace std;
void InitPreferences(void);

class Preferences 
{
 public:
  
  static Preferences* Instance();
  static void DestroyInstance();

  //std::string sDetType;  
  std::string sDLArConfFile;

  // preference for WA105 event with cosmic backgrounds
  int iWa105Beam;
  int iWa105CosmicBkg;
  int iWa105SingleCREvent;
  TString sWa105BeamPdgName;
  TString sWa105CosmicBkgFile;
  float fWa105CosmicRate;
  float fWa105CosmicT0;
  float fWa105CosmicWin;

  // beam halo background
  int   iWa105BHBkg;
  float fWa105BHRate;
  float fWa105BHProfile[10];
  float fWa105BHT0;
  float fWa105BHWin;

  // efield types
  std::string sEFieldType;
  std::string sEFieldMapFile;
  std::string sEFieldMapTags;
  std::string sEFieldMapHistos;

  float fEFieldValue;


  float LRO_SAMPLING_TIME;
  float LRO_TIME_WIN_MIN;
  float LRO_TIME_WIN_MAX;
  int   LRO_TIME_WIN_NBINS;

  int LRO_NPMTS;
  float LRO_PMTS_QEFF; 

  //Scintillation Light Map
  std::string sLightMapFile_LAr;
  std::string sLightMapTags_LAr;
  std::string sLightMapHistos_LAr;
  int sLightMapHIndex_LAr[2];


  std::string sLightMapFile_GAr;
  std::string sLightMapTags_GAr;
  std::string sLightMapHistos_GAr;
  int sLightMapHIndex_GAr[2];


  
  // general VMC preferences
  std::string sVMCTransportCode;
  int iGRandomSeed;


  float fForceRecomb;
  float fForceLifetime;
  
  float fDiffusionDL; // longitudinal diffusion const cm2/s
  float fDiffusionDT; // transverse diffusion const cm2/s 

  //
  //TString seffmapfile;
  TString sLemEffMapFile;
  
  
  // preferences for primary generator
  float fPrimaryTheta[2];
  float fPrimaryPhi[2];
  float fPrimaryPabs[2];
  float fPrimaryVx[2];
  float fPrimaryVy[2];
  float fPrimaryVz[2];
   
  int iPrimarySeed;
  
  
  // output preferences
  int iVerboseLevel; 
  int iAddMCHitInfo;
  int iSaveTGeo;
  

  // GENIE preferences
  int reinteraction;


  //public:
  ~Preferences();
  
  void Dump();
  bool Set(std::string &keyword, std::istringstream &line);
  
 private:
  Preferences();
  static Preferences* pInstance;

};

extern Preferences *myPrefs;

#endif
