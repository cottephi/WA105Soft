//file created by D.Lussi, Aug 2011

void SpecialCuts()
{
  
  /*******************************************************************************************/
  // Define special cuts (energy threshold)
  // G3 case: always active
  // G4 case: only active if specialCuts are activated (see g4tgeoConfig.C) 
  // this is the default!
  /*******************************************************************************************/
  
  const double GeV=1.;
  const double MeV=1e-3;
  const double keV=1e-6;
  const double eV=1e-9;
 
  
  // use standard cuts except for fiducial volume
  // the fiducial medium (=liquid_argon_trcking) has ID 1

  double fiducialCut=10.0*keV;
  int medID = 1;

  gMC->Gstpar(medID,"CUTELE", fiducialCut);
  gMC->Gstpar(medID,"CUTGAM", fiducialCut);
  gMC->Gstpar(medID,"DCUTE",  fiducialCut);
  gMC->Gstpar(medID,"DCUTM",  fiducialCut);
  gMC->Gstpar(medID,"BCUTE",  fiducialCut);
  gMC->Gstpar(medID,"BCUTM",  fiducialCut);  
 
 ((TGeant4 *)gMC)->ProcessGeantMacro("g4config.in");
}
