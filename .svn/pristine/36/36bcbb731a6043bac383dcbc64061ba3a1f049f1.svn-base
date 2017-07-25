// A class used to store information on LAr properties
// A.Meregaglia - A.Rubbia (Oct 2004)

// Singleton class : LiquidARgonMedium::Instance()

#ifndef _LIQUIDARGONMEDIUM_H_
#define _LIQUIDARGONMEDIUM_H_

class LiquidArgonMedium
{
 public:
  // dtor (ctor should be private)
  ~LiquidArgonMedium();
  
  static LiquidArgonMedium* Instance();
  static void DestroyInstance();

  //methods
  double RefIndex(const double lambda);
  double Epsilon(const double lambda);
  double RayLength(const double lambda,const double temperature);
  double BetheBloch(double KE,int pid);
  double BetheBloch(double KE, double Mass);
  double CalculateBeta(double KE, double M);
  double BetheBlochRange(double Range,int pid);
  double BetheBlochRange(double Range,float Mass);
  double QuenchingFactor(double dEdX,double EField);
  double DriftVelocity(double EField,double temperature=89.0);
  double ConvertEdepToQ(double edep);
  double ConvertEdepToNe(double edep);
 
  
  double GetDiffusionDt(const double Efield);
  double GetDiffusionDl(const double Efield);

  void GetDiffusionConsts(const double Efield, double &Dl, double &Dt);

  // get methods
  double GetLArT(){ return LArT; }
  double GetDyne(){ return dyne; }
  double GetLambdaE(){ return LambdaE; }
  double GetGArRhi(){ return GArRho; }
  double GetLArRhi(){ return LArRho; }
  double GetBoltzK(){ return k; }
  double GetLArKT(){ return LArKT; }
  double GetLArWion(){ return Wion; }
  double GetLArScYield(){ return LArScintillationYield; }
  double GetLArScTauFast(){ return LArScintillationFastTimeConstant; }
  double GetLArScTauSlow(){ return LArScintillationSlowTimeConstant; }
  double GetLArScYieldRatio(){ return LArScintillationYieldRatio; }

 private:
  
  double LArT;     // temperature in Kelvin
  double dyne;     // compressibility in CGS units
  double LambdaE;  //
  double GArRho;   // gas density 
  double LArRho;   // liquid density
  double k;        // Boltzmann constant
  double LArKT;    // LAr isothermal compressibility
  double Wion;     // ionization energy in LAr
  
  // Sc light properties
  double LArScintillationYield;
  double LArScintillationResolutionScale;
  double LArScintillationFastTimeConstant;
  double LArScintillationSlowTimeConstant;
  double LArScintillationYieldRatio;

  LiquidArgonMedium(); 
  
  static LiquidArgonMedium* pInstance;
};

#endif


