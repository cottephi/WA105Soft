/* fullreco -- Full reconstruction software
 * (C) 2000 - 2010,  The fullreco Team
 *
 * fullreco is mainly intended for academic purposes. Proper
 * credit must be given if you use fullreco or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Basic Scan application 
   A. Rubbia (March 2002, revived March 2010 during JSPS visit program)
*/
#include <iostream>
#include <fstream>
#include <sstream>

#include "QscanApp.h"
#include "EventLooper.h"
#include "Preferences.h" 

using namespace std;

/*-------------------------------------------------------------------------------------------------------*/
/*                                    C L A S S    I M P L E M E N T A T I O N                           */
/*-------------------------------------------------------------------------------------------------------*/
/*constructor*/
QScanApp::QScanApp(Int_t* argc, char** argv, void* options, Int_t numOptions):TApplication("QScanApp",argc,argv)
{
  status=-1;
  nFiles=0;
  _myEventLooper=NULL;
  isBatch=false; 
  saveTree=false;
  iFile=0;
  
//  InitProgressReporter();
  InitPreferences();

  createMainEventLooper();
}

/*--------------------------------------------------------------------------------------------*/
/*destructor*/
QScanApp::~QScanApp()
{
  cout << "qscanapp destructor called" << endl;
  // Farewell message 
  if (status==0) 
    {
      cout << ' ' << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "              THAT'S ALL FOLKS                " << endl;
      cout << "PROGRAM END: Maximum number of events reached " << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << ' ' << endl;
    }
  else if (status==1)
   {
      cout << ' ' << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "              THAT'S ALL FOLKS                  " << endl;
      cout << "PROGRAM END: All input files have been analyzed " << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << ' ' << endl;
    } 
  else if(status==2) 
    {
      cout << ' ' << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "              THAT'S ALL FOLKS                " << endl;
      cout << "PROGRAM END: ERROR ocurred                    " << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << ' ' << endl;
    }

else //status<0
    {
      cout << ' ' << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "              THAT'S ALL FOLKS                " << endl;
      cout << "              NOTHING WAS READ ????           " << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << ' ' << endl;
    }

  
}

/*--------------------------------------------------------------------------------------------*/
void QScanApp::createMainEventLooper()
{
  if(_myEventLooper)
    delete _myEventLooper;
  _myEventLooper = new EventLooper();
}
/*--------------------------------------------------------------------------------------------*/
void QScanApp::ReadInputFile(const char *name, const char *type)
{
  input[nFiles++]=name;
  strcpy(evType,type);
}


/*--------------------------------------------------------------------------------------------*/
void QScanApp::ReadDatacards(const char *name)
{
  ifstream ifun(name);
  string word; 
  string tmpString;

  if (!ifun.is_open()) {
    cerr << "cannot find datacard file " << name << endl;
    exit(1);
  }

  while (ifun >> word) {

    // skip comment
    if (word[0]=='#') {
      getline(ifun,tmpString);
      continue;
    }
    
    //read datacard
    getline(ifun,tmpString);
    istringstream line(tmpString);
    if (!readDatacard(line,word)) {
      if (word == "END") {
//	myPrefs->Dump();
	return ;
      }
      else {
	cerr << "Abort." << endl;
	exit(1);
      }
    }
  }
  
  if (word != "END") {
    cerr << "QscanApp::ReadDataCards() ERROR: Datacard file must end with the keyword END. Exit." << endl;
    exit(1);
  }
}

/*--------------------------------------------------------------------------------------------*/
bool QScanApp::readDatacard(istringstream &line, string &keyword)
{
  string out;
  bool ret;
  // Total number of events
  if(keyword == "MAXEVENTS") {
    int maxev;
    line >> maxev;
    //cout<<"Max events "<<maxev<<endl;
    _myEventLooper->SetMaxEventNumber(maxev);
    return true;
  }
  else if(keyword == "IFILE") {
    line >> keyword;
    input[nFiles++] = keyword;
    return true;
  }
  // Histo output file
  else if(keyword == "OFILE") {
    line >> keyword;
    output=keyword;
    saveTree=true;
    return true;
  }  
  // Type of events to be read
  else if(keyword == "EVENTTYPE") {
    string value=keyword;
    line >> keyword;
    strcpy(evType,keyword.c_str());
    return true;
  }
  else {
    ret =  myPrefs->Set(keyword,line);
  }
  return ret;
}

/*--------------------------------------------------------------------------------------------*/
/*main application, to be called after initialization */
void QScanApp::RunApplication()
{
  //check input
  if(nFiles>MAXFILES) 
    {
      cerr << "QscanApp::RunApplication() ERROR: Too many input files! Terminate." << endl;
      exit(1);
    }
  if(nFiles==0)
    {
      cerr << "QscanApp::RunApplication() ERROR: No input files! Terminate." << endl;
      exit(1); 
    }

  //load first input file
  if (!_myEventLooper->FileOpen(input[iFile].c_str(),evType))
    {
      cerr << "QscanApp::RunApplication() ERROR: cannot find file " <<input[iFile].c_str() << " ! Exit." << endl;
      exit(1); 
    }
      
  //load output tree
  if (saveTree && !_myEventLooper->LoadTree(output.c_str()))
    {
      cerr << "QscanApp::RunApplication() ERROR: cannot create output file " << output.c_str() << " ! Exit." << endl;
      exit(1); 
    }
      
  //event loop
   if (isBatch) { //program runs in batch mode
    if (eventMask.size()) //analyze only selected events
      for (unsigned int ii=0;ii<eventMask.size();ii++) {
	cout <<" to be done "<<endl;
      }
    else //analyze all events
      while (NextEvent()){
	_myEventLooper->SaveEvent();}
  }
 
  
  if (saveTree) _myEventLooper->CloseTree();
  _myEventLooper->FileClose();
  
  if(_myEventLooper)
    delete _myEventLooper;
}

/*--------------------------------------------------------------------------------------------*/
/*loads the following event, can deal with several input files*/
bool QScanApp::NextEvent()
{
  status=_myEventLooper->NextEvent();
    
  //status ok
  if (status<0)
    return true;
  
  
  return false;
}

