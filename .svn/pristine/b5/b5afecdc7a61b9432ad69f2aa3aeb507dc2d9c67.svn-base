//
//  Qscan.cxx
//  Event scanner based on ROOT
//
//  Created by Andr? Rubbia on 7/3/10.
//  Copyright 2010 ETHZ. All rights reserved.
//

#include <iostream> 
#include <fstream> 
#include <sstream>
#include <TROOT.h>
#include <unistd.h>
#include <string>

#include "QscanApp.h"

using namespace std;

int main(int argc, char * argv[])
{
  char eventType[256];
  char input[256];
  bool isBatch=true;
  bool isDatacard=true;
  char cc;
//  int maxEvents=-1;

  /******************READ CMD LINE*************************/
  opterr = 0;
  while((cc = getopt(argc, argv, "t:bnh")) != -1)
    {
      switch(cc)
	{

case 'b': 
           cout <<" option -b is no longer needed "<<endl;
	   cout <<" please try Qscan.exe datacard.name "<<endl;
	    return 0;	
	 
case 'h':
	  cout << "************************QSCAN HELP************************************" << endl;
	  cout << "USAGE: " << endl
	       << argv[0] << " datacard.file" << endl
	       << "OPTIONS:" << endl
	      
	       << "-h: \t\tprint this manual" << endl
	       << "Exit. " << endl << endl;
	  return 0;
	  
	case ':':
	  cerr << "ERROR: option '-" << optopt << "` needs an argument! Terminate. " << endl << endl;
	  return 1;
	 
	case '?':
	  if(isprint(optopt))
	    cerr << "ERROR: unknown option `-" << optopt << "' ! Terminate."  << endl << endl;
	  else
	    cerr << "ERROR: unknown option character `\\x" << optopt << "' ! Terminate." << endl << endl;
	  return 1;
	  
	default:
	  abort();
	}
    }
  if (optind==argc)
    {
      cerr << "ERROR: no input! Exit." << endl;
      return 1;
    }
  else if (optind<argc-1)
    {
      cerr << "ERROR: too many input files! Exit." << endl;
      return 1;
    }
  else
    strcpy(input,argv[optind]);
      
      
  /*******************RUN THE APPLICATION*************************/

  QScanApp *theApp= new QScanApp(&argc,argv);

  // initialize
  if (isBatch) theApp->SetBatch();
  
  
  if (isDatacard)
    theApp->ReadDatacards(input);
  else
    theApp->ReadInputFile(input, eventType);

  theApp->RunApplication();
  cout << "I'm back" <<endl;
  
  // delete theApp;
  
  return 0;
}
