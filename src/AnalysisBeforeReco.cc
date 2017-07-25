#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "../../WA105Soft/include/EventDecoder.h"

using namespace std;

std::string geometry_name  = "3x1x1";
ssize_t channel_number         = 1280; // total number of channels
ssize_t timebin_number        = 1667; // total number of ADC samples per ch
dlardaq::EventDecoder *myEventDecoder = NULL;

string get_basename( std::string const& pathname){
  size_t last_slash = pathname.find_last_of("/");
  if(last_slash != std::string::npos){
    return pathname.substr( last_slash+1 );
  }
  return pathname;
}

void get_help(){
   cout<<"Usage: "<<endl;
   cout<<" -i <input file>  - input file (mandatory)"<<endl;
   cout<<" -o <output file> - output file (defaults to recotask_<input_file>)"<<endl;
   cout<<" -h               - print this message"<<endl;
   exit(-1);
}

int main(int argc, char **argv){

   string input_file_name = "";
   string output_file_name = "";

   if(argc == 1){get_help();}

   char cc;
   while((cc = getopt(argc, argv, "h:i:o:")) != -1){
      switch(cc){

         case 'i':
         input_file_name = optarg;
         break;

         case 'o':
         output_file_name = optarg;
         break;

         case 'h':
         get_help();
         break;

         default:
         get_help();
         //cerr<<"ERROR: unrecognized option : "<<cc<<endl;
         //break;
      }
   }

   if(input_file_name.empty()){
      cerr<<"ERROR: no input file provided"<<endl;
      get_help();
   }
   if(output_file_name.empty()){
      std::string bsname = get_basename(input_file_name);
      output_file_name = "recotask_" + bsname;
      size_t found = output_file_name.find_last_of(".");
      if( found == std::string::npos ){
         output_file_name += ".root";
      }
      else if( output_file_name.substr( found + 1 ) != "root" ){
         // change extension
         output_file_name.replace( found + 1, 4 , "root");
      }
      cout<<"Output file named "<<output_file_name<<"."<<endl;
   }

   myEventDecoder = new dlardaq::EventDecoder(channel_number, timebin_number);

   return 0;
}
