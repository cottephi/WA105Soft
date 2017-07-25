#include "ProcessDir.h"

using namespace std;

///////////////////////////////////////////////////////////////////////
//                     GetRootFiles                                  //
///////////////////////////////////////////////////////////////////////
int ProcessDir::GetRootFiles(const char* dirname, vector<string> &files)
{
  //Returns absolut path for the files with extansions .root 
  //in a given directory
  string str;
  string ext(".root");
  string dir(dirname);
  
  DIR *dp;
  struct dirent *dirp;
  
  if((dp  = opendir(dirname)) == NULL)
    {
      cerr<<"Error "<<errno<<". Could not open directory"<<dirname<<endl;
      return errno;
    }

  while((dirp = readdir(dp))!=NULL)
    {
      if(dirp->d_type == DT_REG)
	{
	  str.append(dirp->d_name);
	  if(str.find(ext) != string::npos)
	    files.push_back(dir+'/'+str);
	  str.clear();
	}
    }

  closedir(dp);

#ifdef DEBUG
  Print(files);
#endif

  return 0;
}

///////////////////////////////////////////////////////////////////////
//                     Print                                         //
///////////////////////////////////////////////////////////////////////
void ProcessDir::Print(std::vector<std::string> files)
{
  int n = files.size();
  
  if(!n)
    {
      cout<<"No files in the list"<<endl;
      return;
    }
  for(int i=0;i<n;i++)
    cout<<files[i]<<endl;
}
