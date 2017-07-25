//
// 
//
#include <iostream>
#include <iostream>
#include <sstream>
#include <string>

#include <TApplication.h>

#include "RawViewer.h"

//
//
//
void usage()
{
  std::cout<<"Usage: "<<std::endl;
  std::cout<<"  -i <input_file> "<<std::endl;
  std::cout<<"  -s <serverip:port>"<<std::endl;
  exit(-1);
}

//
//
//
void server_str(std::string &ip, unsigned short &port)
{
  size_t found = ip.find(":");
  if(found==std::string::npos)
    {
      std::cerr<<"Bad input format. Should be 'ip:port'"<<std::endl;
      exit(-1);
    }
  port = atoi(ip.substr(found+1).c_str());
  ip = ip.substr(0, found);
}

//
//
//
int main (int argc, char **argv)
{
  std::string finname    = "";
  std::string host_ip    = "";
  unsigned short port    = 0;
  char cc;
  while((cc = getopt(argc, argv, "s:i:")) != -1)
    {
      switch(cc)
	{
	case 'i': 
	  finname = optarg;
	  break;
	case 's':
	  host_ip = optarg;
	  server_str(host_ip, port);
	  break;
	default:
	  usage();
	}
    }

  TApplication theApp("MyApp",&argc,argv);

  // Popup the GUI...
  RawViewer *gui = new RawViewer(gClient->GetRoot(), 500, 500);
  
  if( !host_ip.empty() ) // start in online mode
    gui->ConnectToServer(port, host_ip.c_str());
  else if(!finname.empty()) 
    gui->OpenNewFile( finname );
  
  theApp.Run();
  return 0;
}
