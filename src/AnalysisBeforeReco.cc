#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "EventDecoder.h"
#include "ChMapInterface.h"
#include <sys/stat.h>
#include <unistd.h>
#include "LogMsg.h"
#include <TApplication.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include "RootUtils.h"
#include <fstream>

using namespace std;
using namespace dlardaq;

std::string chan_map_geometry_name  = "3x1x1";
ssize_t channel_number = 1280; // total number of channels
ssize_t timebin_number = 1667; // total number of ADC samples per ch
dlardaq::EventDecoder *myEventDecoder = NULL;

inline bool exists_test (const string& name) {
   struct stat buffer;
   return (stat (name.c_str(), &buffer) == 0);
}

string get_basename( std::string const& pathname){
   size_t last_slash = pathname.find_last_of("/");
   if(last_slash != std::string::npos){
      return pathname.substr( last_slash + 1 );
   }
   return pathname;
}

string get_dirname( std::string const& pathname){
   size_t last_slash = pathname.find_last_of("/");
   if(last_slash != std::string::npos){
      return pathname.substr(0, last_slash+1);
   }
   return pathname;
}

vector<float> Load_ped(string path_to_ped){
   vector<float> ped_vector;
   fstream ped_file;
   ped_file.open(path_to_ped.c_str(), ios::in);
   while(!ped_file.eof()){
      float tmp, ped;
      ped_file>>tmp>>tmp>>tmp>>tmp>>ped>>tmp;
      ped_vector.push_back(ped);
   }
   ped_file.close();
   ped_vector.pop_back();
   return ped_vector;
}

void get_help(){
   cout<<"Usage: "<<endl;
   cout<<" -i <input file>  - input file (mandatory)"<<endl;
   cout<<" -o <output file> - output file (defaults to Analysed_<input_file>)"<<endl;
   cout<<" -h               - print this message"<<endl;
   cout<<" -n <file number>  - Number of input files to analyse. Default is 1."<<endl;
   exit(-1);
}

void getFileNames( string fname, size_t nfile, vector<string> &files ){
   files.push_back( fname );

   size_t pos1 = fname.find_last_of('-');
   if( pos1 == std::string::npos ){
      dlardaq::msg_err<<"Bad name format for the input file in name "<<fname<<endl;
      return;
   }

   // find extension
   size_t pos2 = fname.find_first_of('.', pos1);
   if( pos2 == std::string::npos ){
      dlardaq::msg_err<<"Unrecognized extension in "<<fname<<endl;
      return;
   }

   int cfile = atoi(files[0].substr(pos1+1, pos2-pos1).c_str());
   for(size_t i=1;i<nfile;i++)
   {
      cfile++;
      stringstream ss;
      ss<<files[0].substr(0, pos1+1)<<cfile<<files[0].substr(pos2);
      files.push_back( ss.str() );
   }

}

int main(int argc, char **argv){

   string input_file_name = "";
   string output_file_name = "";
   int file_number = 1;

   const char *thedatafiles = getenv("THEDATAFILES");
   if( !thedatafiles )
   {
      dlardaq::msg_err<<"ERROR: path to THEDATAFILES is not set"<<endl;
      return 1;
   }
   string path_to_ped = thedatafiles;
   path_to_ped.append("/pedestals/pedestals");
   if (!exists_test(path_to_ped)){
      dlardaq::msg_err<<"Pedestals file "<<path_to_ped<<" not found."<<endl;
      return 1;
   }
   vector<float> ped_vector = Load_ped(path_to_ped);

   if(argc == 1){get_help();}

   char cc;
   while((cc = getopt(argc, argv, "h:i:o:n:")) != -1){

      switch(cc){

         case 'i':
         input_file_name = optarg;
         if (!exists_test(input_file_name)){
            dlardaq::msg_err<<"ERROR: File "<<input_file_name<<" not found."<<endl;
            return 1;
         }
         break;

         case 'o':
         output_file_name = optarg;
         break;

         case 'n':
         file_number = atoi(optarg);
         cout<<file_number<<endl;
         if (file_number<1){
            cout<<"ERROR: number of input files must be superior to one."<<endl;
            return 1;
         }
         break;

         case 'h':
         get_help();
         break;

         default:
         get_help();
         //dlardaq::msg_err<<"ERROR: unrecognized option : "<<cc<<endl;
         //break;
      }
   }

   std::vector<string> files;
   getFileNames( input_file_name, file_number, files );

   if(input_file_name.empty()){
      dlardaq::msg_err<<"ERROR: no input file provided"<<endl;
      get_help();
   }
   if(output_file_name.empty()){
      std::string bsname = get_basename(input_file_name);
      std::string drname = get_dirname(input_file_name);
      output_file_name = drname + "analysed_" + bsname;
      size_t found = output_file_name.find_last_of(".");
      if( found == std::string::npos ){
         output_file_name += ".root";
      }
      else if( output_file_name.substr( found + 1 ) != "root" ){
         // change extension
         output_file_name.replace( found + 1, 4 , "root");
      }
      cout<<"Output file named "<<output_file_name<<""<<endl;
   }
   else{
      output_file_name = get_dirname(input_file_name) + output_file_name;
   }

   myEventDecoder = new dlardaq::EventDecoder(channel_number, timebin_number);
   dlardaq::ChannelMap *chmap =  dlardaq::ChMapInterface::Instance().GetMap(chan_map_geometry_name);
   if(!chmap){
      dlardaq::msg_err<<"Cannot find channel map for "<<chan_map_geometry_name
      <<". Aborting..."<<endl;
      exit(-1);
   }
   else{cout<<"Sucessfully loaded channel map "<<chan_map_geometry_name<<"."<<endl;}

   vector<adc16_t> adc_vec;
   double Q_ch = 0;
   double Q_evt = 0;
   double Qmin = 0;
   double Qmax = -1e7;
   // double mean_Q_per_channel = 0;
   // double mean_Q_per_timebin = 0;
   // double rms = 0;

   TH1F adc_histo("adc_histo","adc_histo",1000,0,1e6);

   for (size_t fileID = 0; fileID<files.size(); fileID++){
      ssize_t evt_number = myEventDecoder->Open(files[fileID]);
      cout<<"Found "<<evt_number<<" event in "<<files[fileID]<<"."<<endl;
      dlardaq::evheader_t ev_header;

      for (int evtID = 0; evtID<evt_number; evtID++){
         myEventDecoder->GetEvent(evtID,ev_header,adc_vec);
         for (int ch = 0; ch<channel_number; ch++){
            //if(ch == 0){cout<<"View 0"<<endl;}
            //if(ch == 320){cout<<"View 1"<<endl;}
            // double mean_Q_per_timebin_step = 0;
            // double rms_step = 0;
            for (int tbin = 0; tbin<timebin_number; tbin++){
               double step1 = adc_vec[ch*(tbin+1)]-ped_vector[ch];
               // double step2 = mean_Q_per_timebin_step + (step1 - mean_Q_per_timebin_step)/(tbin+1);
               // double step3 = rms_step + (step1-step2)*(step1-mean_Q_per_timebin_step);
               // mean_Q_per_timebin_step = step2;
               // rms_step = step3;
               Q_ch += step1;
            }
            // mean_Q_per_timebin = mean_Q_per_timebin_step;
            // rms = sqrt(rms_step/(timebin_number-1));
            // cout<<"Mean ADC per timebin: "<<mean_Q_per_timebin<<endl;
            // cout<<"RMS ADC per timebin: "<<rms<<endl;
            // mean_Q_per_channel+=Q_ch/channel_number;
            Q_evt += Q_ch;
            Q_ch = 0;
         }
         // cout<<"Mean ADC per channel in "<<evtID<<": "<<mean_Q_per_channel<<"fC"<<endl;
         // mean_Q_per_channel = 0;
         //cout<<"Total ADC in event "<<evtID<<": "<<adc_evt<<endl;
         //if (Qmin > Q_evt) {Qmin = Q_evt;}
         if (Qmax < Q_evt) {Qmax = Q_evt;}
         adc_histo.Fill(Q_evt*0.15);
         Q_evt=0;
      }
   }
   TCanvas mycan("mycan","mycan",1500,1000);
   adc_histo.Draw();
   adc_histo.GetXaxis()->SetRangeUser(Qmin, Qmax);
   mycan.SaveAs(string(get_dirname(input_file_name) + "mycan.pdf").data());
   mycan.SaveAs(string(get_dirname(input_file_name) + "mycan.root").data());

   return 0;
}
