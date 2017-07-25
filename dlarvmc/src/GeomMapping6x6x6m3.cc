   /**
 * \file GeomMapping6x6x6m3.cc
 * \author Devis Lussi
 * \date 20-Oct-2012
 */

#include <TGeoMatrix.h>
#include <TGeoCompositeShape.h>
#include <TGeoParaboloid.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGeoTube.h>

#include "GeomMapping6x6x6m3.h"


GeomMapping6x6x6m3::GeomMapping6x6x6m3()
{;}


GeomMapping6x6x6m3::~GeomMapping6x6x6m3()
{;}

/*
void GeomMapping6x6x6m3::ConstructMaterials()
{ ConstructAllMaterial(); }
*/ 

// create geometry
void GeomMapping6x6x6m3::CreateGeometry(void)
{
  std::cout<< " Creating geometry for 6x6x6 "<<std::endl;
  
  if (!gGeoManager)
    new TGeoManager("TGeo", "Root geometry manager in BasicGeomMapping");   
  
  ConstructMaterials();
  
  // hall dimensions
  double xyzHall[3]={1600.,1600.,1600.}; 

  // main defining parameters:

  //fiducial
  heightFiducial = geomDrift(); 
  widthFiducial  = 600;         
  lengthFiducial = widthFiducial;
  
  // check geometry
   CheckGeometry(widthFiducial,lengthFiducial,heightFiducial);
   
  //inner vessel
  double heightInner = 811.;          // height inner vessel (gas)
  double widthInner  = 829.;          // width inner vessel

  //other parameters
  double thicknessMembrane   = 0.12;     // stainless membrane layer
  double thicknessInsulation = 120.;     // foam insulation
  double thicknessConcrete   = 50.;      // reinforced concrete base
  double distFiducialTop     = 51.;
  
  //beam pipe
  double bpRadius = 10.;
  //double tubeThickness=0.2;
  //double tubeWindowThichness=0.2;
  //double tubeWindowRadius=10.2;
  
  //beam pipe length
  double bpLength  = 350.0;
  double beamAngle = 0;    // beam angle form horizontal plane in degrees
  double particlePosition[3] = {-600,-600,150};

  TGeoRotation bpRotation("bpRotation",-45,90-beamAngle,0.);
  TGeoTranslation bpTranslationPos(particlePosition[0],particlePosition[1],particlePosition[2]);
  TGeoTranslation bpTranslationDistToOrigin(0,0,-bpLength/2);
  TGeoHMatrix *bpTransformation = new TGeoHMatrix(bpTranslationPos*bpRotation*bpTranslationDistToOrigin);
  bpTransformation->SetName("h1");
  bpTransformation->RegisterYourself();
  
  TGeoRotation mainRotation("mainRotation",0,0,0);
  
  TGeoTranslation trTank(0,0,-heightInner/2+heightFiducial/2+distFiducialTop);
  
  TGeoHMatrix *hTank2=new TGeoHMatrix(trTank*mainRotation);
  hTank2->SetName("hTank2");
  hTank2->RegisterYourself();
  
  //experimental hall
  aGeoTopVolume = gGeoManager->MakeBox("Experimental Hall", air_med, xyzHall[0], xyzHall[1], xyzHall[2]);


  //tube
  TGeoTube *bpVacuum_shape = new TGeoTube("bpVacuum_shape",0,bpRadius, bpLength/2);
  //TGeoTube *tubeMetal_shape = new TGeoTube("tubeMetal_shape",tubeRadius,tubeRadius+tubeThickness,tubeLength);
  //TGeoTube *tubeWindow_shape = new TGeoTube("tubeWindow_shape",0,tubeWindowRadius, tubeWindowThichness);

  TGeoBBox *cubeFid_shape = new TGeoBBox("cubeFid_shape", widthFiducial/2,widthFiducial/2,heightFiducial/2);
  TGeoBBox *cubeGAr_shape = new TGeoBBox("cubeGAr_shape",widthInner/2, widthInner/2, distFiducialTop/2);
  TGeoBBox *cubeLAr_shape = new TGeoBBox("cubeLAr_shape",widthInner/2, widthInner/2, (heightInner-distFiducialTop)/2);

  // tank shapes
  TGeoBBox *cube1_shape = new TGeoBBox("cube1_shape",widthInner/2, widthInner/2, heightInner/2);
  TGeoBBox *cube2_shape = new TGeoBBox("cube2_shape",widthInner/2+thicknessMembrane,
				       widthInner/2+thicknessMembrane, 
				       heightInner/2+thicknessMembrane);
  TGeoBBox *cube3_shape = new TGeoBBox("cube3_shape",widthInner/2+thicknessMembrane+thicknessInsulation,
				       widthInner/2+thicknessMembrane+thicknessInsulation,
				       heightInner/2+thicknessMembrane+thicknessInsulation);
  TGeoBBox *cube4_shape = new TGeoBBox("cube4_shape",widthInner/2+thicknessMembrane+thicknessInsulation+thicknessConcrete,
				       widthInner/2+thicknessMembrane+thicknessInsulation+thicknessConcrete,
				       heightInner/2+thicknessMembrane+thicknessInsulation+thicknessConcrete);
  TGeoCompositeShape *membrane_shape = 
    new TGeoCompositeShape("membrane_shape","(cube2_shape-cube1_shape)");
  TGeoCompositeShape *insulation_shape = 
    new TGeoCompositeShape("insulation_shape","(cube3_shape-cube2_shape)");
  TGeoCompositeShape *concrete_shape = 
    new TGeoCompositeShape("concrete_shape","(cube4_shape-cube3_shape)");
  

  // volumes:
  TGeoVolume *gar_volume    = new TGeoVolume("GAR VOLUME", cubeGAr_shape, gargon_med);
  TGeoVolume *lar_volume    = new TGeoVolume("LAR VOLUME", cubeLAr_shape, largon_med);
  TGeoVolume *active_volume = new TGeoVolume("ACTIVE VOLUME", cubeFid_shape, largon_tracking_med);
  TGeoVolumeAssembly *tank_volume = new TGeoVolumeAssembly("TANK");
  TGeoVolume *membrane_volume     = new TGeoVolume("MEMBRANE VOLUME",membrane_shape,stainless_med);
  TGeoVolume *insulation_volume   = new TGeoVolume("INSULATION VOLUME",insulation_shape,foam_med);
  TGeoVolume *concrete_volume     = new TGeoVolume("CONCRETE VOLUME",concrete_shape,concrete_med);
  tank_volume->AddNode(membrane_volume,1);
  tank_volume->AddNode(insulation_volume,2);
  tank_volume->AddNode(concrete_volume,3);
  TGeoVolume *bpVacuum_volume = new TGeoVolume("BEAM PIPE VACUUM",bpVacuum_shape,vacuum_med);
  //TGeoVolume *bpTube_volume = new TGeoVolume("BEAM PIPE VACUUM",bpTube_shape,stainless_med);
  //TGeoVolume *bpWindow_volume = new TGeoVolume("BEAM PIPE WINDOW",bpWindow_shape,stainless_med);
  
  //draw options
  membrane_volume->SetLineColor(12);
  insulation_volume->SetLineColor(46);
  concrete_volume->SetLineColor(14);
  gar_volume->SetLineColor(7);
  lar_volume->SetLineColor(4);
  active_volume->SetLineColor(8);
 
  TGeoTranslation trGAr(0,0,+distFiducialTop/2+heightFiducial/2);
  TGeoTranslation trLAr(0,0,-(heightInner-distFiducialTop)/2+heightFiducial/2);

  // add active volume to LAr volume
  lar_volume->AddNode(active_volume, 1,
		      new TGeoTranslation("trActive",0,0,(heightInner-distFiducialTop)/2-heightFiducial/2));

  // draw nodes
  gGeoManager->SetTopVolume(aGeoTopVolume);
  //aGeoTopVolume->AddNodeOverlap(active_volume, 1, new TGeoHMatrix(mainRotation));
  aGeoTopVolume->AddNodeOverlap(gar_volume, 1, new TGeoHMatrix(trGAr*mainRotation));
  aGeoTopVolume->AddNodeOverlap(lar_volume, 1, new TGeoHMatrix(trLAr*mainRotation));
  aGeoTopVolume->AddNode(tank_volume,1, new TGeoHMatrix(trTank*mainRotation));
  aGeoTopVolume->AddNodeOverlap(bpVacuum_volume,1,bpTransformation);
 
  
  // finallize geometry  
  //gGeoManager->CheckOverlaps(0.000001,"");
  //gGeoManager->PrintOverlaps();  
  gGeoManager->CloseGeometry();  
  
  //TCanvas *canv=new TCanvas();
  //aGeoTopVolume->Draw("ogl");
  //aGeoTopVolume->Export("tmp.root");
  //canv->WaitPrimitive();
}
