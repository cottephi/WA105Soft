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


#include <string>
#include <cstdlib>
#include "Preferences.h"
#include <iostream>

using namespace std;

Preferences* myPrefs;
void InitPreferences(void)
{
  myPrefs = Preferences::Instance();
}
/*--------------------------------------------------------------------------------------------*/
/*                                   C L A S S    M E T H O D S                               */
/*--------------------------------------------------------------------------------------------*/

Preferences* Preferences::pInstance = NULL; 

Preferences* Preferences::Instance()
{
  if(!pInstance) pInstance = new Preferences();
  return pInstance;
}

void Preferences::DestroyInstance()
{
  if(pInstance)
    {
      delete pInstance;
      pInstance = NULL;
    }
}

Preferences::Preferences(void)
{
  
  // INFORMATION

  // in case you have to add more preferences, add it in:
  // -the class declaration (Preferences.h) under "public:"
  // -here (the constructor) with an initial value
  // -the methods:
  //       GetType(const char *name, int &nTypes)
  //       Set(const char *name, <type> t, int n=0)
  

 
  // general vmc preferences
  sVMCTransportCode = "G4";
  //iVMCTracks = 1;
  iGRandomSeed=1;

  // qdrift
  fForceRecomb        = -1.; // -1 icarus parameterization
  fForceLifetime      = -1;  // -1 infinite lifetime 
  fDiffusionDL        = 0.0; //  0 no diffusion
  fDiffusionDT        = 0.0; //  0 no diffusion
  
  sLemEffMapFile = "";
 

  //standard particle gun settings
  fPrimaryTheta[0]=fPrimaryTheta[1]=0;
  fPrimaryPhi[0]=fPrimaryPhi[1]=0;
  fPrimaryPabs[0]=fPrimaryPabs[1]=0;
  fPrimaryVx[0]=fPrimaryVx[1]=0.;
  fPrimaryVy[0]=fPrimaryVy[1]=0.;
  fPrimaryVz[0]=fPrimaryVz[1]=0.;
  iPrimarySeed=1;
 
  
  // output preferences
  iVerboseLevel = 0;
 

  // GENIE preferences
  reinteraction = 0;

  //WA105 Event preferences
  iWa105Beam          = 0;
  iWa105CosmicBkg     = 0;
  iWa105SingleCREvent = -1;
  fWa105CosmicRate    = 0;
  fWa105CosmicT0      = 0;
  fWa105CosmicWin     = 0;

  //
  iWa105BHBkg  = 0;
  fWa105BHRate = 0;
  fWa105BHT0   = 0;
  fWa105BHWin  = 0;

  fEFieldValue   = 500.0;
  sEFieldType    = "UNIFORM"; //UNIFORM / EMAPFILE
  sEFieldMapFile = "";
  sEFieldMapTags = "";
  sEFieldMapHistos = "";


  LRO_NPMTS = 0;
  LRO_SAMPLING_TIME = -1;
  LRO_TIME_WIN_MIN = 0;
  LRO_TIME_WIN_MAX = 0;
  LRO_TIME_WIN_NBINS = -1;

  LRO_PMTS_QEFF = 1;

  sLightMapFile_LAr = ""; 
  sLightMapTags_LAr = "";
  sLightMapHistos_LAr = "";
  sLightMapHIndex_LAr[0] = -1;
  sLightMapHIndex_LAr[1] = -1;


  sLightMapFile_GAr = ""; 
  sLightMapTags_GAr = "";
  sLightMapHistos_GAr = "";
  sLightMapHIndex_GAr[0] = -1;
  sLightMapHIndex_GAr[1] = -1;


}

Preferences::~Preferences()
{;}

/*--------------------------------------------------------------------------------------------*/
void Preferences::Dump()
{
  
  cout << "General virtual Monte Carlo preferences:" << endl 
       << "transport code = " << sVMCTransportCode << endl 
       << "Primary Seed =" << iPrimarySeed <<endl
       << "gRandom seed   = " << iGRandomSeed << endl << endl;
  cout << "MC simulation preferences:" << endl
       << "recombination factor = " << fForceRecomb << endl
       << "lifetime             = " << fForceLifetime << " us" << endl
       << "longitudianl diffusion const = "<<fDiffusionDL<<" cm2/s "<<endl
       << "transverse diffusion const   = "<<fDiffusionDT<<" cm2/s "<<endl;

  cout << "Primary generator preferences:" << endl    
       << "primary theta = " << fPrimaryTheta[0] << " -> " 
       << fPrimaryTheta[1] << " rad," << endl
       << "primary phi   = " << fPrimaryPhi[0]   << " -> " 
       << fPrimaryPhi[1]   << " rad "  <<endl
       << "primary Pabs  = " << fPrimaryPabs[0]  << " -> " 
       << fPrimaryPabs[1]  << " GeV/c" <<  endl
       << "primary Vx    = " << fPrimaryVx[0]    << " -> " 
       << fPrimaryVx[1]    << " cm, "<< endl
       << "primary Vy    = " << fPrimaryVy[0]    << " -> " 
       << fPrimaryVy[1]    << " cm, "<< endl
       << "primary Vz    = " << fPrimaryVz[0]    << " -> " 
       << fPrimaryVz[1]    << " cm, "<< endl
       << "GENIE rescattering = " << reinteraction << endl;


  cout << "Electric field "<< sEFieldType << endl;
  if( sEFieldType.compare("UNIFORM") == 0)
    cout << " Field value = " << fEFieldValue << " V/cm" << endl;
  else if( sEFieldType.compare("EMAPFILE") == 0)
    cout << " Field file = " << sEFieldMapFile << endl;


  cout << "Output preferences:" << endl;
  if (iSaveTGeo>0) cout << "Save Geometry TGeo." << endl;
  
}
/*--------------------------------------------------------------------------------------------*/
bool Preferences::Set(string &keyword,istringstream &line)
{
 
   
  
  // general vmc preferences
  if (keyword == "VMC_TRANSPORTCODE")
    line >> sVMCTransportCode;  
  else if (keyword == "GRANDOM_SEED")
    line >> iGRandomSeed;
  //geometry
  else if (keyword == "DLArCONFIG")  
    line >>  sDLArConfFile; 

  // qdrift
  else if (keyword == "FORCERECOMB")
    line >>  fForceRecomb;
  else if (keyword == "FORCELIFETIME")
    line >> fForceLifetime;
  else if (keyword == "DIFFUSIONCONST_DL")
    line >> fDiffusionDL;
  else if (keyword == "DIFFUSIONCONST_DT")
    line >> fDiffusionDT;

  // lem efficiency map
  else if (keyword == "LEMEFFICIENCY_MAP")
   line >> sLemEffMapFile;

  
  // primary generator
  else if (keyword =="PRIMARY_SEED")
   line >> iPrimarySeed;
  
  else if (keyword == "PRIMARY_THETA_MINMAX")
    line >> fPrimaryTheta[0] >> fPrimaryTheta[1];
  else if (keyword == "PRIMARY_PHI_MINMAX")
    line >> fPrimaryPhi[0] >> fPrimaryPhi[1];
  else if (keyword == "PRIMARY_PABS_MINMAX")
    line >> fPrimaryPabs[0] >> fPrimaryPabs[1];
  else if (keyword == "PRIMARY_VX_MINMAX")
    line >> fPrimaryVx[0] >> fPrimaryVx[1];
  else if (keyword == "PRIMARY_VY_MINMAX")
    line >> fPrimaryVy[0] >> fPrimaryVy[1];
  else if (keyword == "PRIMARY_VZ_MINMAX")
    line >> fPrimaryVz[0] >> fPrimaryVz[1];
 
  else if (keyword == "RESCATTER")
    line >> reinteraction;

  // Cosmic ray background controls
  else if (keyword == "WA105MC_BEAM")
    {
      iWa105Beam = 1;
      line >> sWa105BeamPdgName;
    }
  else if(keyword == "WA105MC_CRY")
    {
      iWa105CosmicBkg = 1;
      line >> sWa105CosmicBkgFile;
    }
  else if(keyword == "WA105MC_CRWIN")
    line >> fWa105CosmicT0 >> fWa105CosmicWin;
  else if(keyword == "WA105MC_CRRATE")
    line >> fWa105CosmicRate;
  else if(keyword == "WA105MC_CREVNT")
    line >> iWa105SingleCREvent;


  else if(keyword == "WA105MC_BH")
    {
      iWa105BHBkg = 1;
      line >> fWa105BHRate  
	   >> fWa105BHProfile[0]
	   >> fWa105BHProfile[1]
	   >> fWa105BHProfile[2]
	   >> fWa105BHProfile[3]
	   >> fWa105BHProfile[4]
	   >> fWa105BHProfile[5]
	   >> fWa105BHProfile[6]
	   >> fWa105BHProfile[7]
	   >> fWa105BHProfile[8]
	   >> fWa105BHProfile[9];
    }
  else if(keyword == "WA105MC_BHWIN")
    line >> fWa105BHT0 >> fWa105BHWin;

  else if(keyword == "EFIELDTYPE")
    {
      std::string argvstr;
      line >> sEFieldType >> argvstr;
      if( sEFieldType.compare("UNIFORM") == 0)
	{
	  fEFieldValue = atof(argvstr.c_str());
	  sEFieldMapFile = "";
	}
      else if( sEFieldType.compare("EMAPFILE") == 0)
	{
	  sEFieldMapFile = argvstr;
	  fEFieldValue   = 0;
	}
    }
  
  else if(keyword == "EMAPFILE_TAGS"){

    std::string argvstr;
    line >> sEFieldMapTags;
    
  }
  else if(keyword == "EMAPFILE_HISTOS"){

    std::string argvstr;
    line >> sEFieldMapHistos;
    
  }


  else if(keyword == "LRO_NPMTS"){    
    line>>LRO_NPMTS;
  }

  else if(keyword == "LRO_PMTS_QEFF"){    
    line>>LRO_PMTS_QEFF;
  }

  
  else if(keyword == "LRO_SAMPLING_TIME"){    
  
    LRO_TIME_WIN_NBINS = -1;

    line>>LRO_SAMPLING_TIME;
  }

  else if(keyword == "LRO_TIME_WIN"){

    LRO_SAMPLING_TIME = -1;
    
    line>>LRO_TIME_WIN_NBINS>>LRO_TIME_WIN_MIN>>LRO_TIME_WIN_MAX;
  }


  
  else if(keyword == "LIGHTMAP_LAR"){    
    line>>sLightMapFile_LAr;
  }

  else if(keyword == "LIGHTMAP_LAR_TAGS"){    
    line>>sLightMapTags_LAr;
  }

  else if(keyword == "LIGHTMAP_LAR_HISTOS"){    
    line>>sLightMapHistos_LAr;
  }

 else if(keyword == "LIGHTMAP_LAR_HINDEX"){    
   line>>sLightMapHIndex_LAr[0]>>sLightMapHIndex_LAr[1];
  }




  else if(keyword == "LIGHTMAP_GAR"){    
    line>>sLightMapFile_GAr;
  }

  else if(keyword == "LIGHTMAP_GAR_TAGS"){    
    line>>sLightMapTags_GAr;
  }

  else if(keyword == "LIGHTMAP_GAR_HISTOS"){    
    line>>sLightMapHistos_GAr;
  }

 else if(keyword == "LIGHTMAP_GAR_HINDEX"){    
   line>>sLightMapHIndex_GAr[0]>>sLightMapHIndex_GAr[1];
  }







  // output preferences
  else if (keyword == "VERBOSE_LEVEL")
    line >> iVerboseLevel;
  
  
  else if (keyword == "SAVE_TGEO")
    line >> iSaveTGeo;
  
  else if (keyword == "END")
    return false;


  else {
    cerr << "ERROR reading datacards: keyword " << keyword.c_str() << " is not accepted." << endl;
    return false;
  }

  return true;
}
