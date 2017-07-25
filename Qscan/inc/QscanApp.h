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
   A. Rubbia (March 2002)
*/

#ifndef _QQSCANAPP_ 
#define _QQSCANAPP_

#define MAXFILES 10000

#include <TROOT.h>
#include <TTree.h>
#include <TBranch.h>
#include <TApplication.h>

#include "EventLooper.h"

class QScanApp : public TApplication
{
 public:
  QScanApp(Int_t* argc, char** argv, void* options = 0, Int_t numOptions = 0);
  virtual ~QScanApp();

  EventLooper* myEventLooper(void) { return (_myEventLooper); };
  vector<int> eventMask;
  void SetBatch() { isBatch=true; }; 
  void ReadDatacards(const char* name);
  void ReadInputFile(const char* name, const char* type);
  //void SaveEvent();
  void RunApplication();
  bool NextEvent(); 

 private:
  int status;
  int nFiles;
  int iFile;
  bool isBatch; 
  bool saveTree;
  EventLooper* _myEventLooper;
  string input[MAXFILES];
  string output;
  char evType[256];
   
  //bool readDatacard(ifstream &ifun, string &word);
  bool readDatacard(istringstream &line, string &word);
  virtual void createMainEventLooper();
};
#endif

