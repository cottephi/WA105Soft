/*
 * SpaceMap3.h
 *
 *  Created on:
 *      Author:
 *
 *
 */

#ifndef SPACEMAP3_H_
#define SPACEMAP3_H_

//
#include <TString.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TGeoMatrix.h>


//
#include "SpaceMap.h"


//
class TGraph;
class TGraph2D;
class TMultiGraph;

//
namespace Map {

class SpaceMap3: public TH3F {

public:
  enum Axis { kAxisX, kAxisY, kAxisZ };
  enum Ref { kLocal, kMaster, kGlobal };


public:
	SpaceMap3();
	SpaceMap3(const TString & name, const TString & tit
			,int nx, double xmin, double xmax
			,int ny, double ymin, double ymax
			,int nz, double zmin, double zmax);

	SpaceMap3(const SpaceMap3 &);
	SpaceMap3(const TH3F &);
	virtual ~SpaceMap3();

	void Import(TH3F &);
	void ImportBinning(TH3F &);

	//
	//
	//
	bool IsBorder(double x, double y, double z);
	bool IsBorder(const Axis &,double);
	bool IsBorderX(double);
	bool IsBorderY(double);
	bool IsBorderZ(double);
	bool IsInRange(double x, double y, double z);
	bool IsInRangeX(double);
	bool IsInRangeY(double);
	bool IsInRangeZ(double);
	bool IsInRange(const Axis &,double);
	double GetBinCenterMin(const Axis &);
	double GetBinCenterMax(const Axis &);

	// A.T. 2105
	double Extrapolate(double x, double y, double z);
	double InterpolateAtEdge(double x, double y, double z); 
	double ExtrapolateAtEdge(double x, double y, double z);
	double GetSymmetric(double x, double xc);

	TAxis* GetAxis(const Axis &);

	void SetRotation(double,double,double);
	void SetCenter(double,double,double);
	void SetTranslation(double,double,double);
	double GetCenter(const Axis &);
	double GetCenter(const Axis &, const Ref &);
	void ResetCenter();
	void ResetRotation();
	void ResetRefSystem();

	double GetTranslation(const Axis& ) const;
	bool HasTranslation() const;
	bool HasRotation() const;
	bool HasTransformation() const;

	void SetPoint(double x, double y, double z);


	void SetRefSystem(const Ref &);

	TGeoRotation& GetRotationMatrix() { return fRotMatrix; }
	const TGeoRotation& GetRotationMatrix() const { return fRotMatrix; }


	//
	//
	//
	float GetGlobalNativeScale() const { return fGlobNativeScale;}
	float GetGlobalScale()       const { return fGlobScale;}
	float GetGlobalOffset()      const { return fGlobOffset;}
	void  SetGlobalNativeScale(float);
	void  SetGlobalScale(float);
	void  SetGlobalOffset(float);

	//
	//
	virtual float Evaluate(double x, double y, double z);
	virtual float Evaluate();

	//
	//
	//
	TGraph* BuildGraph(const Axis &, const TString& name, const TString& tit
			   , double y, double z, int nbins, double xmin, double xmax);

	TGraph* BuildGraphX(const TString& name, const TString& tit
			    , double y, double z, int nbins, double xmin, double xmax);

	TGraph* BuildGraphY(const TString& name, const TString& tit
			    , double y, double z, int nbins, double xmin, double xmax);

	TGraph* BuildGraphZ(const TString& name, const TString& tit
			    , double y, double z, int nbins, double xmin, double xmax);


	TGraph2D* BuildGraph2D(const Axis& first, Axis second, const TString& name, const TString& tit
				       ,double third
				       ,int n1, double min1, double max1
				       ,int n2, double min2, double max2);

	TGraph2D* BuildGraphXY(const TString& name, const TString& tit
				       ,double third
				       ,int n1, double min1, double max1
				       ,int n2, double min2, double max2);

	TMultiGraph* BuildMultiGraph(const Axis& first, Axis scan, const TString& name, const TString& tit
				       ,double third
				       ,int n1, double min1, double max1
				       ,int n2, double min2, double max2);

	void PrintRanges();
	void PrintCenters();

	void PrintPoint();

	SpaceMap* CalibMap();
private:
	void Init();




 private:
	float fGlobNativeScale;
	float fGlobOffset;
	float fGlobScale;




private:

	TGeoRotation fRotMatrix;

	double fLocal[3];
	double fMaster[3];
	double fGlobal[3];
	double fCenter[3];
	double fTranslation[3];

	bool fHasTranslation;
	bool fHasRotation;

	Ref refSyst;

private:
	SpaceMap3 *fCalibMap;

protected:
	SpaceMap3* map() const;


	//ClassDef(SpaceMap3,1)
};

} /* namespace Map */
#endif /* SPACEMAP3_H_ */
