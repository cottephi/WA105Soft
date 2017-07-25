/****************************************









*******************************************/

#ifndef GLARPROCCONFIG_H
#define GLARPROCCONFIG_H

//
#include <cmath>

//
#include <TMath.h>
#include <TGraph.h>
#include <TF1.h>

//
#include "HEPSystemOfUnits.h"


using namespace CLHEP;


namespace GLArDB {


  static const double Avo = 6.022e23;

  enum Phase {kLiquid,kGas};



  namespace Argon {

    static const int Z = 18;

    static const double A = 39.948; /*molar mass g/mol*/

    static const double Wg = 19.5*eV;

    static const double Wi = 23.6*eV;

    namespace Liquid {

      static const double TempBoilingPoint = 87.3*kelvin; /* 1atm*/
      static const double TempMeltingPoint = 83.8*kelvin; /* 1atm*/
      static const double TempTriplePoint = 83.8*kelvin; /* 0.687 bar*/

      static const double Temperature = TempBoilingPoint;

      static const double density = 1.393*g/cm3;  /*at boiling point temp*/

      static const double nDensity = GLArDB::Avo*(density/(g/cm3))/A;

      namespace Lifetime {

        namespace Ele{
            static const double tau = 5*ms;
        }

      }//Lifetime

      namespace Scintillation{

        namespace OpticalPhoton{

        static const double EneMean = 9.69*eV;
        static const double EneWidth = 0.22*eV;

        }//OP

        static const double tauSingl = 6*ns;
        static const double tauTripl = 1600*ns;
        static const double tauRecomb = 0.8*ns;//kubota 1976
        static const double tauRise = 1*ns;

        namespace ElectronRecoil {
        namespace SingletToTriplet{

            static const double RatioX = 0.36;
            static const double RatioXwidth = 0.06;

            static const double RatioR = 0.5;
            static const double RatioRwidth = 0.2;

            static const double par[4] = {0.2701,0.003379,-4.7338e-5,8.1449e-6,};

            inline double RatioExcit(double LET /* MeV*cm2/g */) {

                if(LET<3) return RatioX;

                return par[0] + par[1]*LET + par[2]*LET*LET
                + par[3]*LET*LET*LET;
            }


            inline double RatioRecomb(double LET /* MeV*cm2/g */) {

                if(LET<3) return RatioR;
                else return RatioExcit(LET);
            }

        }//SingleToTripletRatio
        }//ElectronRecoil

        namespace Alpha {
        namespace SingletToTriplet{
            //Dawson 05 and Tey. 11 (arXiv:1103.3689)

            static const double par[4]={-0.065492,1.9996,0.08254,2.1811};

            inline double RatioExcit(double dE /*MeV*/){
                if(dE==0.) return 0;
                return (par[0]+par[1]*exp(-dE))/(1+par[2]/(dE*dE)) + par[3];
            }
            inline double RatioRecomb(double dE /*MeV*/){
                return RatioExcit(dE);
            }

        }//SingletToTriplet
        }//Alpha

        namespace NuclearRecoil{
        namespace SingletToTriplet{
                //Hitachi et al. Phys. Rev. B 27 (1983) 5279, Akimov 2002

            static const double par[2]={0.22218,0.48211};

            inline double RatioRecomb(double E /*keV*/){
                return par[0]*pow(E,par[1]);
            }

            inline double RatioExcit(double E /*keV*/){
                return RatioRecomb(E);
            }


        }//SingletToTriplet
        }//NuclearRecoil


    }//Scint



      namespace ExcitationRatio {

        static const double BiExcitation = 0.6;

        static const double ResolutionScale = 0.107; //Doke 1976


        namespace ElectronRecoil {

          static const double par[1] = {0.21}; //Aprile et al.

          inline double Ratio(double EField=0) { return par[0];}

        }//ElectronRecoil

        namespace NuclearRecoil {

          static const double par[4]={0.69337,0.3065,-0.008806,0.76313};

          inline double Ratio(double EField /* V/cm */) {
            return par[0]+par[1]*exp(par[2]*pow(EField,par[3]));
          }

        }//NuclearRecoil


        inline double Ratio(double EField=0) {
          return ElectronRecoil::Ratio(EField);
        }


    }//ExcitationRatio



    namespace RecombData {
        /* ICARUS */
        static const double A = 0.8;
        static const double K = 0.0486*(kilovolt/cm)*(g/cm2)/MeV;
    }


    namespace DokeBirks {

      namespace ZeroField{
	static const double parA[1]={0.0003};
	static const double parB[1]={0.};
	static const double parC[1]={0.75};

	inline double A(float EField) {return parA[0];}
	inline double B(float EField) {return parB[0];}
	inline double C(float EField) {return parC[0];}
      }//ZeroField

      namespace NonZeroField{

	/* Fit used int NEST
	   taken from
	   Obodowsky 15th IEEE International Conference on Dielectric Liquids (ICDL)
	   26 June Ð 1 July, 2005 in Coimbra, Portugal.
	*/
	static const double parA[2] = {0.07, -0.85};
	static const double parB[1] = {0.};
	static const double parC[1] = {0.};

	
	inline double A(double EField /* kilovolt/cm */) {
	  return parA[0]*pow(fabs(EField),parA[1]);
	}
	inline double B(double EField) { return parB[0];}
	inline double C(double EField) { return parC[0];}


	/* Fit from M. Sorel  arXiv:1405.0848v2  */
	static const double parA2[2] = {0.05, -0.85};
	static const double parB2[2] = {0.06,-0.85};
	static const double parC2[1] = {1./6.};

	inline double A2(double EField /* kilovolt/cm */) {
	  return parA2[0]*pow(fabs(EField),parA2[1]);
	}
	inline double B2(double EField) {
	  return parB2[0]*pow(fabs(EField),parB2[1]);
	}

	inline double C2(double EField) { return parC2[0]; }


      }//NonZeroField


      /* default */
      inline double A(double EField=0.) {
	//std::cout<<" AAA "<<EField<<" "<<ZeroField::A(EField)<<" "<<NonZeroField::A(EField)<<std::endl;
	if( EField==0.) return ZeroField::A(EField);
	else return NonZeroField::A(fabs(EField));
      }

      inline double B(double EField=0.) {
	if( EField==0.) return ZeroField::B(EField);
	else return NonZeroField::B(fabs(EField));
      }

      inline double C(double EField=0.) {
	if( EField==0.) return ZeroField::C(EField);
	else return NonZeroField::C(fabs(EField));
      }

      }//DokeBirks



      namespace ThomasImel {

            namespace ZeroField{

              static const double par[1] ={ 0.099 };

              inline double Factor(double EField /* V/cm */) { return par[0]; }

              namespace NuclearRecoil {
                static const double par[1] ={ 0.25 };
                inline double Factor(double EField /* V/cm */) { return par[0]; }
              }

        }//ZeroField

        namespace NonZeroField{

          static const double par[2]={0.156977,-0.1};

          inline double Factor(double EField /* V/cm */) { return par[0]*pow(fabs(EField),par[1]); }
        }//NonZeroField

        inline double Factor(double EField /* V/cm */) {
          if(EField==0.) return ZeroField::Factor(fabs(EField));
          else return NonZeroField::Factor(fabs(EField));
        }//Factor

        namespace NuclearRecoil{
          inline double Factor(double EField /* V/cm */) {
            if(EField==0.) return ZeroField::NuclearRecoil::Factor(fabs(EField));
            else return NonZeroField::Factor(fabs(EField));
          }
        }//NuclearRecoil

     }//ThomasImel






      namespace Lindhard {


        static const double epsilon_pars[2]={11.5, -7./3.};

        static const double gamma_pars[4]={3., 0.15, 0.7, 0.6};

        static const double kappa_pars[4]={0.133, 2./3.,-1./2, 2./3.};

        static const double yield_pars[2] ={0.23,-5};

        inline double epsilon(double E /* keV */ ) {
          return epsilon_pars[0]*E*pow(Argon::Z,epsilon_pars[1]);
        }

        inline double gamma(double E /* keV */ ) {

          double eps = epsilon(E);

          return gamma_pars[0]*pow(eps,gamma_pars[1])
            +gamma_pars[2]*pow(eps,gamma_pars[3])
            +eps;
        }//gamma

        inline double kappa(double a2){
          return kappa_pars[0]*pow(Argon::Z,kappa_pars[1])*pow(a2,kappa_pars[2])*kappa_pars[3];
        }

        inline double YieldFactor(double E /* keV */,double a2=0) {

          double e = epsilon(E);

          double Y = yield_pars[0] *(1+ exp(yield_pars[1]*e) );

          return Y;
        }

      }//Lindhard



        namespace Drift {

            namespace Ele{

                static const double DefaultConstant = 1.6*mm/microsecond;

                //W.Walkowiak, nucl. instr. meth. A 449, 288-294, 2000
                namespace Walk {  /* Walkoviak */

                    static const double range_E[2]={0.5,12}; /*kV*/
                    static const double range_T[2]={87.,94,}; /*kelvin*/

                    static const double T0 = 90.371; /* K */

                    static const double P[6]={
                                            -0.01481 /*1/K*/
                                            ,-0.0075 /*1/K*/
                                            ,0.141 /* (kV/cm)^-1 */
                                            ,12.4 /* kV/cm */
                                            ,1.627 /* (kV/cm)^P[5] */
                                            ,0.317};

                  static const double PErr[6]={0.00095,0.0028,0.023,2.7,0.078,0.021};

                  inline double Velocity(double E /* kV/cm */, double T= Liquid::Temperature /* K */) { /* mm/microsec */

                            //if(E<range_E[0] || E>range_E[1]) return 0;
                            //if(T<range_T[0] || T>range_T[1]) return 0;


                            double v = (P[0]*(T-T0)+1)
                                              *(P[2]*E*log(1+P[3]/E) +P[4]*pow(E,P[5]))
                                              +P[1]*(T-T0);
                            return v;
                 }//Velocity

                }//Walkoviak

                namespace ICARUS { /* Amoruso et al. NIM A 516 (2004) 68Ð79*/

                    static double const Temp = 89;/* kelvin */
                    static const double range_E[3]={0.056,1.012}; /*kV*/
                    static const double range_T[3]={89.,89.}; /*kelvin*/

                    static const int NP = 12;

                    static const double Ep[NP]={/*kV/cm*/
                        0.056,0.075,0.1,0.15
                        ,0.304,0.405,0.506,0.607,0.708,0.810,0.911,1.012
                    };

                    static const double vp[NP]={/*mm/microsec*/
                        0.289,0.381,0.499,0.694
                        ,1.169,1.385,1.551,1.682,1.792,1.893,1.964,2.057
                    };

                    static const double velerr[NP]={/*mm/microsec*/
                        0.002,0.006,0.01,0.008
                        ,0.003,0.005,0.015,0.012,0.025,0.025,0.006,0.011
                    };

                    static const double fit_par[6]={/*pol5 fit to points above*/
                        -0.0135,5.861,-8.635,8.510,-5.237,1.559
                    };

                     const TGraph g(NP,Ep,vp);//graph as interpolator

                     inline double Velocity(double E /* kV/cm */, double T= ICARUS::Temp /* K */) { /* mm/microsec */

                            //if(E<range_E[0] || E>range_E[1]) return 0;
                            //if(range_T[0] || T>range_T[1]) return 0;

                            //intepolation to data points
                            //double v = g.Eval(E);

                            //use polfit function
                            double v=fit_par[0]+fit_par[1]*E+fit_par[2]*E*E+fit_par[3]*E*E*E
                            +fit_par[4]*E*E*E*E + fit_par[5]*E*E*E*E*E;

                            return v;

                    }//Velocity

                }//ICARUS


                //
                inline double Velocity(double E/*kV/cm*/,double T=Liquid::Temperature /* K*/) {

                    if( E<ICARUS::range_E[1] )
                        return  ICARUS::Velocity(E,T);

                    return Walk::Velocity(E,T);

                }//






                namespace Diffusion {
                    //cross check which is transf and longit...
                    //Transverse Diffusion from ICARUS NIM A527 (2004) 329
                    static const double Ln= 4.8; /* cm2/s */
                    // Longitudinal Diffusion from atrazhev  Timoshkim theory at 1kV/cm
                    // Ln = 18*cm2/s; ?
                    //Transverse, WA105105 TDR
                    static const double Tr = 13; /* cm2/s */

                    //Fit to filed dependence /* NEST */
                    static const double par[2]={93.342,0.041322};

                    inline double FitTr(double EField /*V/cm*/) { /*cm2/s*/
                        double Nd = GLArDB::Argon::Liquid::nDensity;
                        return par[0]*pow(EField/Nd,par[1]);
                    }

                    inline double FitLn(double EField /*V/cm*/){/*cm2/s*/
                        if(EField<100/*<100 V/cm*/) return 8; /*cm2/s*/
                        return 0.15*FitTr(EField);
                    }



                    inline double SigmaSpaceTr(double t /*s*/) {
                        return sqrt(2*Tr*t);
                    }

                    inline double SigmaSpaceTr(double L /*cm*/, double v /*cm/s*/) {//cm
                        if(v<0 || L<0) return 0;
                        return sqrt(2*Tr*L/v);
                    }

                    inline double SigmaTimeTr(double L /*cm*/, double v /*cm/s*/) {//s
                        if(v<0 || L<0) return 0;
                        return SigmaSpaceTr(L,v)/v;
                    }

                    inline double SigmaSpaceLn(double t /*s*/) {//s
                        return sqrt(2*Ln*t);
                    }

                    inline double SigmaSpaceLn(double L /*cm*/, double v /*cm/s*/) {//cm
                        if(v<0 || L<0) return 0;
                        return sqrt(2*Ln*L/v);
                    }

                    inline double SigmaTimeLn(double L /*cm*/, double v /*cm/s*/) {//s
                        if(v<0 || L<0) return 0;
                        return SigmaSpaceLn(L,v)/v;
                    }

                }//Diffusion

            }//Ele

            namespace Ion{

                //placeholder
                static const double mobility = 1;
                static const double Velocity = 1;

            }//Ion


        }//Drift



        namespace Eloss {

            namespace Ele {

                    inline double dEdx(double Ekin/*MeV*/) { /* MeV/cm*/
                        return 0;
                    }
            }//Ele

        }//Eloss



    }//Liquid




    //==============================
    //
    //
    //
    //
    //
    //==============================



        namespace Gas {


          static const double temperature = GLArDB::Argon::Liquid::TempBoilingPoint;

          static const double density = 1.76*kg/cm3; /* at standard conditions 273 K 1bar */


          static const double Wg = 19.5*eV;

          static const double Wi = 26.4*eV;

          namespace Scintillation{

            namespace OpticalPhoton{

              static const double EneMean = 9.69*eV;
              static const double EneWidth = 0.22*eV;

            }//OP

            static const double tauSingl = 7*ns;
            static const double tauTripl = 3200*ns;
            static const double tauRecomb = 0.8*ns;

            static const double tauRise = 1*ns;

            namespace ElectroLum {

                //Conde et al., IEEE Trans. Nuc. Sci. 24 (1977) 221Ð224.
                //Monteiro et al. Phys. Lett. B 668 (2008) 167Ð170.
                static double par[3]={0.0813 /*1/V*/,139/*1/bar/cm*/,30.6/*1/cm*/};

                inline double dNOverdx(double E /*V/cm*/, double P /*bar*/) {  /* Gammas per electron per cm */
                    double n = par[0]*E - par[1]*P-par[2];
                    return n<0. ? 0. : n;
                }

                static double par2[2]={0.0813 /*1/V*/,1.9e-18/*cm2*/};


                inline double dNOverdxN(double E /*V/cm*/, double Nd/*atoms/cm3*/) { /*Gammas per electron per cm*/

                        /* Nd = rho*Avo/A */
                        if(Nd<0) return 0;
                        double n = Nd*(par2[0]*E/Nd-par2[1]);
                        return n<0. ? 0. : n;
                }

                inline double dNOverdxD(double E /*V/cm*/, double density/*g/cm3*/) { /*Gammas per electron per cm*/

                        double Nd = GLArDB::Avo*density/GLArDB::Argon::A;

                        return dNOverdxN(E,Nd);
                }

            }//ElectroLum

        }//Scint


    }//Gas





//=============================
//
//
//
//
//
//================================



        namespace ElectronLET {

              static const double par[7]={
                                                116.70
                                                ,-162.97
                                                ,99.361
                                                ,-33.405
                                                ,6.5069
                                                ,-0.69334
                                                ,0.031563
                                                  };


              inline double LET(double E /* keV */) {

                if(E>0 && E<1) return 100;

                if(E>=1){
                  double val = par[0]+par[1]*log10(E)
                    +par[2]*pow(log10(E),2)
                    +par[3]*pow(log10(E),3)
                    +par[4]*pow(log10(E),4)
                    +par[5]*pow(log10(E),5)
                    +par[6]*pow(log10(E),6);
                  return val;
                }

                return 0;

            }//LET


        }//ElectronLET


  }//Argon



}//GLArDB






#endif




