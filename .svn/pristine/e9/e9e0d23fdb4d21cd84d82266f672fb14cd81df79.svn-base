// $Id: g3tgeoConfig.C 341 2008-05-26 11:04:57Z ivana $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------


/// \file E01/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VMC for Example01 

#include<iostream>

void Config()
{
/// The configuration function for Geant3+TGeo VMC for Example01
/// called during MC application initialization. 

  new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry");
  
  //process
  gMC->SetProcess("HADR", 4);
  gMC->SetProcess("DRAY", 1);
  gMC->SetProcess("LOSS", 1); //4= no fluctuations, 2 full landau fluct

  //geant3 cuts
/*   gMC->SetCut("CUTELE", 0.00001);
  gMC->SetCut("CUTGAM", 0.00001);
  gMC->SetCut("DCUTE", 0.00001);
  gMC->SetCut("DCUTM", 0.00001); */
  
  /* cuts are defined in SpecialCuts.C */

  std::cout << "Geant3 has been created." << std::endl;
  
  
}


