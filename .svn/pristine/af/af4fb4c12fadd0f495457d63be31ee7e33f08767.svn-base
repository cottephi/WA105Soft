//file created by D.Lussi, Jul 2011

void Config()
{
  
  /*******************************************************************************************/
  // Run configuration:
  // TG4RunConfiguration* runConfiguration 
  //   = new TG4RunConfiguration(const TString &userGeometry, 
  //   const TString &physicsList,const TString &specialProcess);
  
  // userGeometry:
  // "geomRoot"            -->    geometry defined via Root, Root navigation
  // "geomRootToGeant4"    -->    geometry defined via Root, G4 native navigation

  // physics list:
  // "emStandard"          -->    standard em physics (default)
  // "emStandard+optical"  -->    standard em physics + optical physics
  // "<XYZ>"               -->    selected hadron physics list (<XYZ> = LHEP, QGSP, ...)
  // "<XYZ>+optical"       -->    selected hadron physics list + optical physics
  
  // special processes:
  // "stepLimiter"        -->    step limiter (default)
  // "specialCuts"        -->    VMC cuts
  // "specialControls"    -->    VMC controls for activation/inactivation selected processes
  // "stackPopper"        -->    stack popper physics
  /*******************************************************************************************/
  
  TString userGeometry   = "geomRootToGeant4";
  //TString physicsList    = "emStandard";
  //TString physicsList    = "QGSP_BERT_HP";
  TString physicsList    = "QGSP_BERT";
  //  TString physicsList    = "QGSP_BIC";
  TString specialProcess = "stepLimiter+specialCuts";
  
  Bool_t specialStacking ;  //qui
  
  TG4RunConfiguration* runConfiguration = new TG4RunConfiguration(userGeometry, physicsList, specialProcess);
  
  TGeant4* geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
}
