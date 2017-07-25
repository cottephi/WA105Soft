#include <iostream>
#include <iostream>
#include <string>

#include <TApplication.h>
#include "MonBenchDist.h"

#include "DQMonViewer.h"


int main (int argc, char **argv)
{
  TApplication theApp("MyApp",&argc,argv);

  // Popup the GUI...
  DQMonViewer gui(gClient->GetRoot(), 500, 500);
  
  MonBenchDist reco_bench;
  reco_bench.SetPollTime(5 /* seconds */ );
  gui.AddTaskToTree( &reco_bench );

  theApp.Run();
  return 0;
}
