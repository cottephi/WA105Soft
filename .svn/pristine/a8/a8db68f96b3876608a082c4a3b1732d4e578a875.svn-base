/**
 * \file GeomMapping1x1x3m3.cc
 * \author Sebastien Murphy
 * \date 24 Apr 2013
 */

#include <iostream>

#include <TGeoMatrix.h>
#include <TGeoCompositeShape.h>
#include <TGeoParaboloid.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGeoTube.h>
#include "TGeoManager.h"

#include "GeomMapping1x1x3m3.h"

GeomMapping1x1x3m3::GeomMapping1x1x3m3()
{;}

GeomMapping1x1x3m3::~GeomMapping1x1x3m3()
{;}


//void GeomMapping1x1x3m3::ConstructMaterials()
//{
//  ConstructAllMaterial();
//}

// Create geometry
void GeomMapping1x1x3m3::CreateGeometry()
{
  
  std::cout<< " Creating geometry for 3x1x1 "<<std::endl;
  
  if (!gGeoManager)
    TGeoManager *mng = new TGeoManager("TGeo", "Root geometry manager in BasicGeomMapping");
  
  //make materials
  ConstructMaterials();
  
  //construct experimental hall and make it mother volume
  double xyzHall[3]={700.,700.,700.};
  aGeoTopVolume = gGeoManager->MakeBox("Experimental Hall",air_med,xyzHall[0],xyzHall[1],xyzHall[2]);
  gGeoManager->SetTopVolume(aGeoTopVolume);
  
  // main defining parameters:
  heightFiducial  = geomDrift();      // diameter inner vessel
  widthFiducial   = 100;              // diameter inner vessel
  lengthFiducial  = 300;              // diameter inner vessel  

  CheckGeometry(widthFiducial,lengthFiducial,heightFiducial);

  //inner vessel
  double heightTopCap = 120.; //this also corresponds to the insulation thickness along the z axis in the top part
  double heightInner  = 323.5-heightTopCap; //height inner vessel including gas, before 215, now 203.5
  double widthInner   = 238.; //before 226
  double lengthInner  = 478.; //before 406
  
  //membrane
  double thicknessMembrane   = 0.12;
  double heightMembrane = heightInner+2*thicknessMembrane;
  double widthMembrane = widthInner+2*thicknessMembrane;
  double lengthMembrane = lengthInner+2*thicknessMembrane;

  //insulation
  double thicknessInsulation = 101.;
  double heightInsulation = heightInner+2*thicknessMembrane+thicknessInsulation+heightTopCap;
  double widthInsulation = widthInner+2*thicknessMembrane+2*thicknessInsulation;
  double lengthInsulation = lengthInner+2*thicknessMembrane+2*thicknessInsulation;
  
  //cryostat
  double thicknessSSplates = 0.6;
  double thicknessSStopCap = 1.0;
  double heightCryostat = heightInsulation+thicknessSStopCap+thicknessSSplates;
  double widthCryostat = widthInsulation+2*thicknessSSplates;
  double lengthCryostat = lengthInsulation+2*thicknessSSplates;

  //CRT
  double heightCRT = 180;
  double widthCRT = 180;
  double thicknessCRT = 0.5;
  
  //other parameters
  double heightLAr = 159.4;
  double distFiducialTop = heightInner-159.4; //corresponding to height GAr
  double distBotCRTtoBotCryo = 192.0;
  double thicknessIbeam = 24;
  double thicknessCRTsupport = 9.7;
  
  TGeoRotation mainRotation("mainRotation",0,0,0);
  
  //basic shapes
  TGeoBBox *cubeFid_shape = new TGeoBBox("cubeFid_shape", widthFiducial/2,lengthFiducial/2,heightFiducial/2);
  TGeoBBox *cubeGAr_shape = new TGeoBBox("cubeGAr_shape", widthInner/2, lengthInner/2, distFiducialTop/2);
  TGeoBBox *cubeLAr_shape = new TGeoBBox("cubeLAr_shape", widthInner/2, lengthInner/2, heightLAr/2);
  TGeoBBox *cube1_shape = new TGeoBBox("cube1_shape", widthInner/2, lengthInner/2, heightInner/2);
  TGeoBBox *cube2_shape = new TGeoBBox("cube2_shape", widthMembrane/2, lengthMembrane/2, heightMembrane/2);
  TGeoBBox *solidInsulation_shape = new TGeoBBox("solidInsulation_shape", widthInsulation/2, lengthInsulation/2, heightInsulation/2 );
  TGeoBBox *solidCryostat_shape = new TGeoBBox("solidCryostat_shape", widthCryostat/2, lengthCryostat/2, heightCryostat/2);
  TGeoBBox *CRT_shape = new TGeoBBox("CRT_shape", widthCRT/2, thicknessCRT/2, heightCRT/2);
  
  
  // define composite shapes
  TGeoCompositeShape *membrane_shape = new TGeoCompositeShape("membrane_shape","(cube2_shape-cube1_shape)");
  TGeoTranslation tr1(0,0,-(heightTopCap-(heightInsulation-heightMembrane)/2));
  TGeoHMatrix *htr1=new TGeoHMatrix(tr1);
  htr1->RegisterYourself();  htr1->SetName("htr1");
  TGeoCompositeShape *insulation_shape = new TGeoCompositeShape("insulation_shape","(solidInsulation_shape-cube2_shape:htr1)");
  
  TGeoTranslation tr2(0,0,-(thicknessSStopCap-(heightCryostat-heightInsulation)/2));
  TGeoHMatrix *htr2=new TGeoHMatrix(tr2);
  htr2->RegisterYourself();  htr2->SetName("htr2");
  TGeoCompositeShape *cryostat_shape = new TGeoCompositeShape("cryostat_shape","(solidCryostat_shape-solidInsulation_shape:htr2)");
  
  // volumes:
  TGeoVolume *gar_volume = new TGeoVolume("GAR VOLUME", cubeGAr_shape, gargon_med);
  TGeoVolume *lar_volume = new TGeoVolume("LAR VOLUME", cubeLAr_shape, largon_med);
  TGeoVolume *active_volume = new TGeoVolume("ACTIVE VOLUME", cubeFid_shape, largon_tracking_med);
  TGeoVolume *membrane_volume = new TGeoVolume("MEMBRANE VOLUME", membrane_shape, stainless_med);
  TGeoVolume *insulation_volume = new TGeoVolume("INSULATION VOLUME",insulation_shape,foam_med);
  TGeoVolume *cryostat_volume = new TGeoVolume("CRYOSTAT VOLUME",cryostat_shape, stainless_med);
  TGeoVolume *CRT1_volume = new TGeoVolume("CRT1 VOLUME",CRT_shape, stainless_med);
  TGeoVolume *CRT2_volume = new TGeoVolume("CRT2 VOLUME",CRT_shape, stainless_med);
  
  //draw options
  membrane_volume->SetLineColor(kBlue);
  //membrane_volume->SetFillColorAlpha(kBlue, 0.35);
  membrane_volume->SetLineWidth(1);
  insulation_volume->SetLineColor(kOrange);
  gar_volume->SetLineColor(7);
  lar_volume->SetLineColor(4);
  active_volume->SetLineColor(8);
  cryostat_volume->SetLineColor(kRed);
  CRT1_volume->SetLineColor(kBlue);
  CRT2_volume->SetLineColor(kBlue-3);
  
  TGeoTranslation trGAr(0,0,distFiducialTop/2+heightFiducial/2);
  
  double LArShift = (heightLAr-heightFiducial)/2;
  TGeoTranslation trLAr(0,0,-LArShift);
  TGeoTranslation trLArActive(0,0,+LArShift);

  double membraneShift = -(heightInner-heightFiducial)/2+distFiducialTop;
  TGeoTranslation trMembrane(0,0,membraneShift);
  
  double insulationShift = membraneShift+(heightTopCap-(heightInsulation-heightMembrane)/2);
  TGeoTranslation trInsulation(0,0,insulationShift);
  
  double cryostatShift = insulationShift+thicknessSStopCap-(heightCryostat-heightInsulation)/2;
  TGeoTranslation trCryostat(0,0,cryostatShift);
  
  double CRT_yShift = (lengthCryostat/2+thicknessCRTsupport+thicknessIbeam+thicknessCRT/2);
  double CRT_zShift = -(heightFiducial/2
			+(heightLAr-heightFiducial)/2+thicknessMembrane+thicknessInsulation)
    +distBotCRTtoBotCryo;
  TGeoTranslation trCRP1(0, -CRT_yShift, CRT_zShift);
  TGeoTranslation trCRP2(0, +CRT_yShift, CRT_zShift);

  // draw nodes
  gGeoManager->SetTopVolume(aGeoTopVolume);
  aGeoTopVolume->AddNodeOverlap(gar_volume, 1, new TGeoHMatrix(trGAr*mainRotation));
  lar_volume->AddNodeOverlap(active_volume, 1, new TGeoHMatrix(trLArActive*mainRotation));
  aGeoTopVolume->AddNodeOverlap(lar_volume, 1, new TGeoHMatrix(trLAr*mainRotation));
  aGeoTopVolume->AddNodeOverlap(membrane_volume, 1, new TGeoTranslation(trMembrane*mainRotation));
  aGeoTopVolume->AddNodeOverlap(insulation_volume,1, new TGeoHMatrix(trInsulation*mainRotation));
  aGeoTopVolume->AddNode(cryostat_volume, 1, new TGeoHMatrix(trCryostat*mainRotation));
  aGeoTopVolume->AddNode(CRT1_volume, 1, new TGeoHMatrix(trCRP1*mainRotation));
  aGeoTopVolume->AddNode(CRT2_volume, 1, new TGeoHMatrix(trCRP2*mainRotation));
  
  // finalize geometry
  //    gGeoManager->CheckOverlaps(0.000001,"");
  //    gGeoManager->PrintOverlaps();  
  gGeoManager->CloseGeometry();
  //gGeoManager->SaveAs("111Geometry.root");  
  
  //TCanvas *canv=new TCanvas();
  //aGeoTopVolume->Draw("ogl");
  //aGeoTopVolume->Export("tmp.root");
  //canv->WaitPrimitive();
}
