//////////////////////////////////////////////////////////////////////////////////
// 
//
//  Implementation of non-negative least squares based on active set method 
//  by C. Lawson and R. Hanson in Solving Least Squares Problems
//
//  Created: Mon May  4 02:49:21 CEST 2015
//
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef __NNLS_H__
#define __NNLS_H__

#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompLU.h>

#define NNLSMAXBINS 100

class NNLS
{
public:
  NNLS();
  NNLS(std::vector<float> &resp);
  ~NNLS();

  // Solve non-negative least squares problem: A x = b with constraint x >= 0
  double Solve(std::vector<float> &dataIn, size_t nparam, std::vector<float> &vecOut);

  // Define response vector
  void SetResponse(std::vector<float> &resp)
  {
    m_respvec  = resp;
    m_nresp    = m_respvec.size();
    m_samesize = false; // force to recompute conv matrix later
  }
  
  // Set methods  
  void SetVerbosity(int val){ m_verbosity = val; }  
  //void SetTolerance(double val){ m_tol = val; }

  // Get methods
  
  //double GetTolerance(){ return m_tol; }

private:
  void SetSize(size_t dsize, size_t nparam);
  void RebinData(std::vector<float> &vecin, 
		 std::vector<float> &vecout,
		 int rebinf);
  
  void MoveToSet(int val, std::vector<int> &V1, std::vector<int> &V2);
  void MakeConvMatrix(std::vector<float> resp); //, size_t nparam);
  void UpdateSolution(TMatrixD &A, TVectorD &s, TVectorD &b, std::vector<int> &P);
  double GetResiduals(TMatrixD &A, TVectorD &x, TVectorD &b, TVectorD &w);
  double GetMaxElement(TVectorD &v, std::vector<int> &R, int &maxidx);

  std::vector<float> m_respvec;
  int       m_verbosity;
  bool      m_samesize;      //flag to avoid recomputing conv matrix
  double    m_tol;           //tolerance
  size_t    m_nrows;         //number of rows in conv matrix 
  size_t    m_ncols;         //number of cols in conv matrix
  size_t    m_nresp;         //size of the response vector
  TMatrixD  m_cmat;          //convolution matrix
  float     m_zero;
  int       m_rbfctr;         
};

#endif
