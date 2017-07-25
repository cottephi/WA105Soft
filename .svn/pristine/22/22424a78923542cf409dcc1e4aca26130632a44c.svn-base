/**
 * \file BasicGeomMapping.cc
 * \author Andre Rubbia
 * \date 7-Mar-2010
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BasicGeomMapping.h"
#include "Geometry.h"

//ClassImp(BasicGeomMapping)

BasicGeomMapping::BasicGeomMapping()
{
  aGeoTopVolume = 0;
}

BasicGeomMapping::~BasicGeomMapping()
{
  if (gGeoManager) delete gGeoManager;
}


//
void BasicGeomMapping::ConstructMaterials()
{
  ConstructAllMaterial();
}


// check geometry
void BasicGeomMapping::CheckGeometry(double x, double y, double z)
{   
  
  if ( x != ageom->GetGG_SRxdimension() || 
       y != ageom->GetGG_SRydimension() || 
       z != ageom->GetGG_SRzdimension() ) 
    {
      std::cout<<"Please check geometry: CRM dimensions don't match dimension of active volume in GeomMapping" <<std::endl;
      std::cout<<"CRM dimensions          :  "
	       <<ageom->GetGG_SRxdimension()<<"  "
	       <<ageom->GetGG_SRydimension()<<"  "
	       <<ageom->GetGG_SRzdimension()<<std::endl;
      std::cout<<"active volume dimensions:  "<<x<<"  "<<y<<"  "<<z<<std::endl;
      exit(1);
    }
  else
    std::cout<<"---> GeometryCheck ok "<<std::endl;
}
  


 
// construct all materials needed by membrane tank and dlar tpc
void BasicGeomMapping::ConstructAllMaterial(void)
{
  if (!gGeoManager)
    new TGeoManager("TGeo", "Root geometry manager in BasicGeomMapping");   

  double envTemperature = STP_temperature;
  double envPressure    = STP_pressure;
  double LArTemperature = 87.0; //temperature of LAr in Kelvin
  
  // Define elements
  TGeoElementTable *table = gGeoManager->GetElementTable(); 

  TGeoElement *hydrogen_el   = table->GetElement(1);
  TGeoElement *carbon_el     = table->GetElement(6);
  TGeoElement *nitrogen_el   = table->GetElement(7);
  TGeoElement *oxygen_el     = table->GetElement(8);
  TGeoElement *sodium_el     = table->GetElement(11);
  TGeoElement *aluminum_el   = table->GetElement(13);
  TGeoElement *silicon_el    = table->GetElement(14);
  TGeoElement *phosphorus_el = table->GetElement(15);
  TGeoElement *sulfur_el     = table->GetElement(16);
  TGeoElement *argon_el      = table->GetElement(18);
  //TGeoElement *potassium_el = table->GetElement(19);
  TGeoElement *calcium_el    = table->GetElement(20);
  TGeoElement *chromium_el   = table->GetElement(24);
  TGeoElement *manganese_el  = table->GetElement(25);
  TGeoElement *iron_el       = table->GetElement(26);
  TGeoElement *nickel_el     = table->GetElement(28);
  //TGeoElement *copper_el = table->GetElement(29);
  TGeoElement *molybdenum_el = table->GetElement(42);
  //TGeoElement *antimony_el = table->GetElement(51);
  //TGeoElement *lead_el = table->GetElement(82);

  ////////////////////////////////////////////////////////
  // Define materials
  
  // vacuum
  TGeoMaterial *vacuum_mat = new TGeoMaterial("Vacuum",0,0,0);

  // air
  TGeoMixture *air_mat = new TGeoMixture("Air",4,0.1205000E-02);
  air_mat->DefineElement(0,carbon_el,0.000124);
  air_mat->DefineElement(1,nitrogen_el,0.755267);
  air_mat->DefineElement(2,oxygen_el,0.231781);
  air_mat->DefineElement(3,argon_el,0.012827);
  air_mat->SetPressure(envPressure);
  air_mat->SetTemperature(envTemperature);
  air_mat->SetState(TGeoMaterial::kMatStateGas);

  // liquid argon
  TGeoMaterial *largon_mat = new TGeoMaterial("Liquid_argon",argon_el,1.396);
  largon_mat->SetState(TGeoMaterial::kMatStateLiquid);
  largon_mat->SetTemperature(LArTemperature);
  
  // cold gas argon
  TGeoMaterial *gargon_mat = new TGeoMaterial("Gas_argon",argon_el,5.664e-3);
  gargon_mat->SetState(TGeoMaterial::kMatStateGas);
  gargon_mat->SetTemperature(LArTemperature);
  
  // stainless steel
  TGeoMixture *stainless_mat = new TGeoMixture("INOX", 9, 8.03);
  stainless_mat->DefineElement(0, carbon_el, .0003);
  stainless_mat->DefineElement(1, manganese_el, .02);
  stainless_mat->DefineElement(2, silicon_el, .01);
  stainless_mat->DefineElement(3, phosphorus_el, .00045);
  stainless_mat->DefineElement(4, sulfur_el, .0003);
  stainless_mat->DefineElement(5, nickel_el, .12);
  stainless_mat->DefineElement(6, chromium_el, .17);
  stainless_mat->DefineElement(7, molybdenum_el, .025);
  stainless_mat->DefineElement(8, iron_el, .654);
  stainless_mat->SetTemperature(LArTemperature);
  stainless_mat->SetState(TGeoMaterial::kMatStateSolid);
  
  // concrete mixture
  TGeoMixture *concrete_mat = new TGeoMixture("CONCRETE",6,2.50000);
  concrete_mat->DefineElement(0,oxygen_el,0.52);
  concrete_mat->DefineElement(1,silicon_el,0.325);
  concrete_mat->DefineElement(2,calcium_el,0.6000000E-01);
  concrete_mat->DefineElement(3,sodium_el,0.1500000E-01);
  concrete_mat->DefineElement(4,iron_el,0.2000000E-01);
  concrete_mat->DefineElement(5,aluminum_el,0.6000000E-01);
  
  //preliminary: polyurethane foam with a density of 0.1 g/cm3
  TGeoMixture *foam_mat = new TGeoMixture("FOAM",4,0.1000);
  foam_mat->DefineElement(0,nitrogen_el,0.2);
  foam_mat->DefineElement(1,hydrogen_el,0.2);
  foam_mat->DefineElement(2,carbon_el,0.2);
  foam_mat->DefineElement(3,oxygen_el,0.4);    
  
  
  //viewing properties
  vacuum_mat->SetTransparency(80);
  air_mat->SetTransparency(80);
  largon_mat->SetTransparency(70);
  gargon_mat->SetTransparency(70);
  stainless_mat->SetTransparency(70);
  concrete_mat->SetTransparency(70);
  foam_mat->SetTransparency(10);     
   
  //vacuum_mat->Print();
  //air_mat->Print();
  //largon_mat->Print();
  //gargon_mat->Print();
  //stainless_mat->Print();
  //concrete_mat->Print();
  //foam_mat->Print();
   
  // Default tracking parameters
  Double_t par[8];
  par[0]=0;
  par[1]=0;                 // ifield
  par[2]=0.;                // fieldm    -->   magnetic field
  par[3]=20.;               // tmaxfd    -->   Maximum angle due to field deflection 
  par[4]=0.01;              // stemax    -->   Maximum displacement for multiple scat  (before 0.01)
  par[5]=0.3;               // deemax    -->   Maximum fractional energy loss, DLS (before: 0.3)
  par[6]=0.001;             // epsil     -->   Tracking precision
  par[7]=0.001;             // stmin  0.001
  
  //- tracking media
  int medID = 1;
  //!!! Important: should be 1 for fiducial volume (Liquid_argon_trcking)
  largon_tracking_med = new TGeoMedium("Liquid_argon_trcking",medID++,largon_mat, par); 
  largon_med    = new TGeoMedium("Liquid_argon", medID++, largon_mat, par);
  gargon_med    = new TGeoMedium("Gas_argon", medID++, gargon_mat, par);
  vacuum_med    = new TGeoMedium("Vacuum", medID++, vacuum_mat, par);
  air_med       = new TGeoMedium("Air", medID++,  air_mat, par);
  stainless_med = new TGeoMedium("StainlessSteel", medID++, stainless_mat, par);
  concrete_med  = new TGeoMedium("Concrete", medID++, concrete_mat, par);
  foam_med      = new TGeoMedium("Foam", medID++, foam_mat, par);  
}

