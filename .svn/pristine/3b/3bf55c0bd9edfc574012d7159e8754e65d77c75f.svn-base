#include <cstdlib>
#include <iostream>
#include <iostream>
#include <string>

#include <TApplication.h>

#include "RecoViewer.h"

void usage()
{
  std::cout<<"Usage: "<<std::endl;
  std::cout<<"  -i <input_file> "<<std::endl;
  exit(-1);
}

int main (int argc, char **argv)
{

  std::string finname    = "";
  char cc;
  while((cc = getopt(argc, argv, "i:")) != -1)
    {
      switch(cc)
	{
	case 'i': 
	  finname = optarg;
	  break;
	default:
	  usage();
	}
    }
  
  if(finname.empty())
    usage();

  TApplication theApp("MyApp",&argc,argv);

  //TObjArray *finput = theApp.InputFiles();
  //for(int i=0;i<finput->GetEntries();i++)
  //std::cout<<finput->At(i)->GetName()<<std::endl;

  // Popup the GUI...
  RecoViewer gui(gClient->GetRoot(), 500, 500);
  gui.OpenFile(finname);

  theApp.Run();
  return 0;
}
