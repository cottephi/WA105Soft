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

#include "NNLS.h"

using namespace std;

//
NNLS::NNLS()
{
  m_tol          = 1.0E-6;
  m_samesize     = false;
  m_verbosity    = 0;
  m_zero         = 0;
  m_rbfctr       = 1;
  m_nrows = m_ncols = m_nresp = 0;
}

//
NNLS::NNLS(std::vector<float> &resp)
{
  m_tol          = 1.0E-6;
  m_samesize     = false;
  m_verbosity    = 0;
  m_zero         = 0;
  m_rbfctr       = 1;
  m_nrows = m_ncols = m_nresp = 0;

  SetResponse(resp);
}

// dtor
NNLS::~NNLS()
{;}

// set size
void NNLS::SetSize(size_t dsize, size_t nparam)
{
  m_rbfctr = 1;
  if(nparam >  NNLSMAXBINS) 
    m_rbfctr = (int)floor((float)(nparam)/NNLSMAXBINS + 0.5);
  
  //if(m_rbfctr!=1)
  //cout << " Apply rebin factor "<<m_rbfctr<<" to dsize "<<dsize<<endl;

  //m_samesize = (nparam == m_ncols);

  // set dimensions of the matrix  
  m_ncols    = nparam / m_rbfctr;
  m_nrows    = dsize  / m_rbfctr; //data.size();
  //m_nrows    = m_nresp + m_ncols - 1;
}

/////////////////////////////////////////////
// Solve deconvolution using NNLS method
double NNLS::Solve(std::vector<float> &dataIn, size_t nparam, std::vector<float> &vecOut)
{
  double rval = -1.0;
  vecOut.clear();

  if(m_respvec.empty())
    {
      cerr<<"ERROR: response vector is not defined "<<endl;
      return rval;
    }

  if(nparam > dataIn.size())
    {
      cerr<<"ERROR: NNLS the number of parameters is greater than equations"<<endl;
      return rval;
    }
  /*
  if(m_nresp > data.size())
    {
      cerr<<"ERROR: NNLS data size is smaller than response vector"<<endl;
      cerr<<"       "<<m_nresp<<" "<<data.size()<<endl;
      return rval;
    }
  */
  SetSize(dataIn.size(), nparam);

  std::vector<float> elecresp, data;
  RebinData(m_respvec, elecresp, m_rbfctr);
  RebinData(dataIn, data, m_rbfctr);

  // make convolution matrix from response vector
  MakeConvMatrix(elecresp);
  
  
  //while(m_nrows > data.size()) //pad with zeros
  //data.push_back(0.0);

  // allocate arrays
  std::vector<int> R, P; //active and passive sets
  
  TVectorD b(m_nrows); //data
  TVectorD x(m_ncols); //uknowns
  TVectorD s(m_ncols); //candidates
  TVectorD w(m_ncols); //residuals
  
  // init in one loop
  for(size_t i=0;i<m_nrows;i++)
    {
      if(i<m_ncols)
	{
	  x[i] = 0;
	  R.push_back(i);
	}
      b[i] = data[i];
    }
     
  // residuals with the first guess
  rval = GetResiduals(m_cmat, x, b, w);
  
  bool loopexit = false;
  int itouter   = 0;
  int iter      = 0;
  int maxiter   = 5*nparam; 

  int    maxidx;
  double maxval;

  //main loop
  while((!R.empty()) && (maxval = GetMaxElement(w, R, maxidx) > m_tol)) //outer loop
    {
      itouter++;  //increment outer loop index
      
      if(maxval < m_tol && !R.empty())
	{
	  cout<<"NNLS the tolerance is satisfied, but R is not empty."<<endl;
	  loopexit = true;
	}
      
      MoveToSet(maxidx, R, P);         //updata active & passive sets
      UpdateSolution(m_cmat, s, b, P); //get candidate solution vector
      
      while(s.Min() < m_zero) //inner loop
	{
	  // compute step scale
	  double alpha = 2.0;
	  for(size_t i=0;i<P.size();i++)
	    {
	      int idx = P[i];
	      
	      // skip solutions that are already +ve
	      if(s[idx] >= m_zero)  continue;  //m_zero)
	      
	      double val = x[idx] / (x[idx] - s[idx]);

	      if(val < alpha)
		alpha = val;
	    }

	  // choose new x and update sets
	  R.clear(); P.clear();
	  for(size_t i=0;i<m_ncols;i++)
	    {
	      x[i] += alpha * (s[i] - x[i]);
	      if(fabs(x[i]) < m_tol)
		R.push_back(i);
	      else
		P.push_back(i);
	    }

	  // update solution vector with new P
	  UpdateSolution(m_cmat, s, b, P);

	  iter++;
	  if(iter >= maxiter)
	    {
	      cout<<"NNLS loop is too long. Exiting ..."<<endl;
	      loopexit = true;
	      break;
	    }
	}
      
      // update x and get new residuals
      x = s;
      rval = GetResiduals(m_cmat, x, b, w);

      if( loopexit ) 
	{
	  cout<<" Total number of parameters in R "<<R.size()<<endl;
	  cout<<" Iterations reached "<<iter<<" total residual sq2 "<<rval<<endl;
	  break;
	}
    }

  // copy to output
  std::vector<float> xvec;
  if(m_verbosity >= 1)
    cout<<"Solution vector: "<<rval<<" "<<itouter<<" "<<iter<<endl;
  for(size_t i=0;i<m_ncols;i++)
    {
      xvec.push_back(x[i]);
      if(m_verbosity >= 1)
	cout<<setw(5)<<i<<setw(15)<<x[i]<<setw(15)<<w[i]<<endl;
    }
  
  // go to original binning
  RebinData(xvec, vecOut, -m_rbfctr);
  
  return rval;
}

/////////////////////////////////////////////
// construct convolution matrix
void NNLS::MakeConvMatrix( std::vector<float> resp )
{
  //if(m_samesize) return; //already done
  
  m_cmat.Clear();
  m_cmat.ResizeTo(m_nrows, m_ncols);
  
  size_t nresp = resp.size();

  for(size_t i=0;i<m_ncols;i++)
    {
      for(size_t j=0;j<nresp;j++)
	{
	  if((j+i) >= m_nrows) break; //cut matrix size to data
	  m_cmat[j+i][i] = resp[j];
	}
    }
}

/////////////////////////////////////////////
// move to set
void NNLS::MoveToSet(int val, std::vector<int> &V1, std::vector<int> &V2)
{
  //classic erase-remove idiom to remove value from v1 set
  V1.erase( std::remove( V1.begin(), V1.end(), val), V1.end() );
  
  //append value to v2 set
  V2.push_back(val);  
}

/////////////////////////////////////////////
// find max element index
double NNLS::GetMaxElement(TVectorD &v, std::vector<int> &R, int &maxidx)
{
  maxidx = -1;
  
  double maxval = -1.0E+6;
  /*
  for(int i=0;i<v.GetNrows();i++)
    {
      if(v[i]>maxval)
	{
	  maxval = v[i];
	  maxidx = i;
	}
    }
  */
  for(size_t i=0;i<R.size();i++)
    {
      if(v[R[i]] > maxval)
	{
	  maxval = v[R[i]];
	  maxidx = R[i];
	}
    }
	
  //cout<<" Max element "<<maxval<<" "<<maxidx<<endl;
  if(maxidx<0)
    {
      cerr<<"ERROR: NNLS problem with finding max element"<<endl;
      //for(size_t i=0;i<R.size();i++)
      //{
      //cout << i <<" "<< v[R[i]] << endl;
      //}
    }
  return maxval;
}

/////////////////////////////////////////////
// Compute residuals: w = A^T ( x - A b)
double NNLS::GetResiduals(TMatrixD &A, TVectorD &x, TVectorD &b, TVectorD &w)
{
  int N   = w.GetNrows();
  int M   = A.GetNrows();

  double rval = 0.0;
  for(int i=0;i<N;i++)
    {
      w[i] = 0;
      for(int j=0;j<M;j++)
	{
	  double ax = 0.0;
	  for(int ii=0;ii<N;ii++)
	    ax += A[j][ii] * x[ii];
	  
	  w[i] += A[j][i]*(b[j] - ax);
	}
      rval += w[i] * w[i];
    }

  return rval;
}

/////////////////////////////////////////////
// Compute updated solution A s ~ b with constriants in set P
void NNLS::UpdateSolution( TMatrixD &A, TVectorD &s, TVectorD &b, 
			   std::vector<int> &P )
{
  // 
  int M = A.GetNrows();
  int N = A.GetNcols();
  
  std::sort(P.begin(), P.end());

  TMatrixD Ap(M, P.size()); // restricted matrix
  int k = 0;
  for(size_t i=0;i<P.size();i++)
    {
      for(int j=0;j<M;j++)
	Ap[j][k] = A[j][P[i]];
      
      k++;
    }

  TMatrixD At = TMatrixD(TMatrixD::kTransposed, Ap);  
  TMatrixD AA = TMatrixD(Ap, TMatrixD::kTransposeMult, Ap);
  TVectorD Ab(P.size());
  Ab = At*b;
  

  //TDecompLU lu(AA);
  //bool ok
  //TVectorD sp = lu.Solve(b, ok);
  //TVectorD sp(P.size());

  TVectorD sp = AA.Invert() * Ab;

  s.Clear();
  s.ResizeTo(N);
  for(size_t i=0;i<P.size();i++)
    {
      s[P[i]] = sp[i];
    }

  for(int i=0;i<N;i++)
    if(s[i] == 0) s[i] = m_zero;
}

//////////////////////////////////////////////
// rebin data
void NNLS::RebinData(std::vector<float> &vecin, 
		     std::vector<float> &vecout, 
		     int rebinf)
{
  vecout.clear();

  size_t n_in  = vecin.size();
  size_t n_out = 0;
  int    nbins = (int)(fabs(rebinf));

  if(nbins == 1) //nothing to do
    {
      vecout = vecin;
      return;
    }

  // do rebinning
  if(rebinf > 1)       //rebin
    n_out = (size_t)(((float)n_in) / nbins);
  else if(rebinf < -1) //rebin inverse
    n_out = (size_t)(nbins * n_in);
  
  if(n_out == 0)
    {
      std::cerr<<"ERROR: rebinning for rebin factor "<<rebinf<<std::endl;
      vecout = vecin;

      return;
    }

  vecout.resize(n_out);
  
  if( rebinf > 1 ) // rebin
    {
      for(size_t i=0;i<n_out;i++)
	{
	  float sum = 0;
	  for(int j=0;j<nbins;j++)
	    {
	      size_t idx = (size_t)(i*nbins + j);
	      if(idx < n_in)
		sum += vecin[idx];
	    }
	  vecout[i] = sum;
	}
    }
  else if(rebinf < -1) // rebin inverse
    {
      for(size_t i=0;i<n_in;i++)
	{
	  float val = vecin[i];
	  for(int j=0;j<nbins;j++)
	    {
	      size_t idx = (size_t)(i*nbins + j);
	      vecout[idx] = val/nbins;
	    }
	}
    }
}
