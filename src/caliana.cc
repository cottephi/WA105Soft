///////////////////////////////////////////////////////////////////////////////////
//
// Basic analysis of calibration data. Two possiblities are included
// right now 'pedestal' and 'pulser' (or not pedestal)
// The input are raw daq files and the output is an ASCII with the
// name like pedestal_DDMMYY_HHMMSS.ped (in case of pedestal) and
// pulser_DDMMYY_HHMMSS.pul for pulser analysis
//
// The pedestal analysis is basicly looping over all events and calcutating
// mean and RMS of ADC samples for each channel
//
// The pulser analysis attempts to find a pulse in a given channel. For this
// one need to specify some threshold for pulse amplitude (sufficiently higher
// than noise RMS). And optinally a time window (in digital ticks) where
// the pulse is expected to appear.
//
// Note that the output is produced in the coordinate system of the DAQ channels
// as well as the view channels. For this to work one needs to specify DAQ to CRP
// channel map to use (current default is the map for the 3x1x1).
//
//
///////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <bitset>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <utility>
#include <cmath>
#include <ctime>

#include <TApplication.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>

#include "ChPedestal.h"
#include "ChMapInterface.h"
#include "EventDecoder.h"
#include "RootUtils.h"
#include "CaliAnaPed.h"
#include "CaliAnaPulser.h"
#include "LogMsg.h"

using namespace std;

//
// some global defaults (can be reset via command arguments)
//
std::string cmname  = "3x1x1";
ssize_t nch         = 1280; // total number of channels
ssize_t ntdc        = 1667; // total number of ADC samples per ch

// raw data decoder
dlardaq::EventDecoder *DaqDecoder = NULL;

//
//
//
string ReadData( vector<string> &files, ssize_t nev, std::vector<caliana::CaliAnaDaqCh*> &pana ){
	size_t chsamples = DaqDecoder->GetNSample();

	dlardaq::runheader_t rnh;
	dlardaq::evheader_t  evh;
	dlardaq::footer_t    eft;


	string formatstr = "%d%m%y_%H%M%S";
	string timestr   = "";
	size_t counter   = 0;
	fstream out;
	out.open("./out.dat",fstream::out);
	for(size_t f=0;f<files.size();f++){
		dlardaq::msg_info<<"Reading "<<files[f]<<endl;
		DaqDecoder->Open(files[f]);

		rnh = DaqDecoder->GetRunHeader();
		eft = DaqDecoder->GetFileFooter();

		dlardaq::msg_info<<"Run number      "<<rnh.run_num<<endl;
		dlardaq::msg_info<<"Number of event in the file "<<eft.num_events<<endl;

		if( nev <=0 ){ // process all
			nev = DaqDecoder->GetTotEvents();
		}
		else{ // process some
			ssize_t tmp = DaqDecoder->GetTotEvents();
			if( nev > tmp ) nev = tmp;
		}
		if(!nev){continue;}

		for(size_t i=0;i<(size_t)nev;i++){
			std::vector<dlardaq::adc16_t> adc;
			DaqDecoder->GetEvent( i, evh, adc );
			if( i % 20 == 0 ){dlardaq::msg_info<<">>>>> Read event from file "<<i<<" / "<<nev<<endl;}

			if( timestr.empty() ){
				char mbstr[100];
				strftime( mbstr, sizeof(mbstr), formatstr.c_str(), localtime(&evh.trig_info.ts.tv_sec) );
				timestr = mbstr;
				//dlardaq::msg_info<<"Local timestamp "<<timestr<<endl;
			}
			//dlardaq::msg_info<<">>> Event number "<<evh.ev_num<<endl;
			//dlardaq::msg_info<<">>> Event size   "<<evh.ev_size<<endl;
			//dlardaq::msg_info<<">>> Trig number  "<<evh.trig_info.num<<endl;
			//dlardaq::msg_info<<">>> Time stamp   "<<evh.trig_info.ts.tv_sec<<" s "
			//<<evh.trig_info.ts.tv_nsec<<" ns"<<endl;

			// process only comlete events
			if( !EVDQFLAG(evh.dq_flag) ) {continue;}
			//dlardaq::msg_warn<<">>> flags event "
			//<<evh.ev_num<<" detected, DQBits = "<<bitset<8>(evh.dq_flag)<<endl;

			//dlardaq::msg_info<<">>> Number of ADC samples "<<adc.size()<<endl;

			// fill
			for(size_t j=0;j<pana.size();j++){
				pana[j]->ProcessEvent( evh.ev_num, &adc[ pana[j]->GetCh() * chsamples ], chsamples );
			}
			counter++;
		}
	}

	dlardaq::msg_info<<"Processed "<<counter<<" events"<<endl;

	return timestr;
}

//
//
//
//
void usage()
{
	std::cout<<"Usage: "<<std::endl;
	std::cout<<"  -a <pedestal or pulser> (mandatory)"                                <<std::endl;
	std::cout<<"  -i <input_file> (mandatory)"                                        <<std::endl;
	std::cout<<"  -e <number of events to proces> (deault is all)"                    <<std::endl;
	std::cout<<"  -N <number of input files from the run> (default is one)"           <<std::endl;
	std::cout<<"  -p <path to save calibration results> (default is ./)"              <<std::endl;
	std::cout<<"  -o <output file> (optional, if provided will produce ROOT file)"    <<std::endl;
	std::cout<<"  -n <number of daq ch in total> (default 1280 for 3x1x1)"            <<std::endl;
	std::cout<<"  -r <number of adc samples per ch> (default 1667 for 3x1x1)"         <<std::endl;
	std::cout<<"  -s <daq ch to select> (optinally select a given ch used for debug)" <<std::endl;
	std::cout<<"  -C <calibration norm for pulser> (mandatory for pulser analysis)"   <<std::endl;
	std::cout<<"  -P <use pedestal 0/1> (optional) "                                  <<std::endl;
	std::cout<<"  -T <ADC threshold for pulse finding> (default is 200 ADC)"          <<std::endl;
	std::cout<<"  -W <time window in format start-end> (default is not used)"         <<std::endl;
	exit(-1);
}

//
// get list of files assuming format runnum-xx.ext
//
void getFileNames( string fname, size_t nfile, vector<string> &files )
{
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

//
//
//
void getTimeWindow( std::string input, float &tlo, float &thi )
{
	tlo = thi = 0;

	size_t pos = input.find('-');
	if( pos == std::string::npos )
	{
		dlardaq::msg_err<<"Bad format for time window "<<input<<endl;
		return;
	}

	std::stringstream ss1(input.substr(0, pos));
	std::stringstream ss2(input.substr(pos+1));
	ss1 >> tlo;
	ss2 >> thi;
	if( tlo > thi ) std::swap( tlo, thi );
}

//
//
//
// the main
//
int main(int argc, char** argv)
{
	// some defaults
	ssize_t chan   = -1;   // channel to analyze (-1 means all)
	ssize_t nev    = -1;   // analyze all events
	std::string fname = "";
	ssize_t nfiles = 1;
	bool anaped;
	std::string anatype = "";
	std::string twin    = "";

	//
	std::string dirout   = "./";

	//
	std::string frootout = "";

	calidaq::ChPedestal *caliPed = NULL;

	//
	// for pulse finding
	float thresh  = 200;   // threshold
	float tlo     = 0;     // time window low value
	float thi     = 0;     // time window hi value
	float norm    = 0;     // calibration norm for pulser

	char cc;
	while((cc = getopt(argc, argv, "a:e:i:N:n:o:p:n:r:s:T:W:C:P:")) != -1)
	{

		switch(cc)
		{
			case 'a':
			anatype = optarg;
			break;

			case 'e':
			nev = atoi(optarg);
			break;

			case 'i':
			fname = optarg;
			break;

			case 'N':
			nfiles = atoi( optarg );
			break;

			case 'o':
			frootout = optarg;
			break;

			case 'p':
			dirout = optarg;
			break;

			case 'n':
			nch = atoi( optarg );
			break;

			case 'r':
			ntdc = atoi( optarg );
			break;

			case 's':
			chan = atoi( optarg );
			break;

			case 'C':
			norm = atof(optarg);
			break;

			case 'T':
			thresh = atof( optarg );
			break;

			case 'P':
			if( atoi( optarg ) > 0 )
			caliPed = &calidaq::ChPedestal::Instance();
			break;

			case 'W':
			twin = optarg;
			getTimeWindow(twin, tlo, thi);
			break;

			default:
			dlardaq::msg_err<<"Unrecognized option "<<cc<<endl;
			break;
		}
	}

	if(fname.empty())
	usage();

	if(anatype.empty())
	usage();

	if(*dirout.rbegin() != '/') dirout += "/";

	anaped = false;
	if( anatype.compare( "pedestal" ) == 0 )
	anaped = true;
	if(!anaped && norm == 0)
	{
		dlardaq::msg_err<<"Need to specify normalization constant for "<<anatype<<endl;
		usage();
	}
	dlardaq::msg_info<<"Analysis type \'"<<anatype<<"\'"<<endl;
	dlardaq::msg_info<<"Saving output to directory "<<dirout<<endl;
	dlardaq::msg_info<<"Using channel map for "<<cmname<<endl;

	if(nch<0 || ntdc < 0 )
	usage();

	// data DaqDecoderoder
	DaqDecoder = new dlardaq::EventDecoder( nch, ntdc );

	// calculate channel map
	dlardaq::ChannelMap *chmap =  dlardaq::ChMapInterface::Instance().GetMap(cmname);
	if(!chmap)
	{
		dlardaq::msg_err<<"Cannot find channel map for "<<cmname
		<<". Aborting..."<<endl;
		exit(-1);
	}

	// analysis type
	vector<caliana::CaliAnaDaqCh*> pana;
	if(chan >= 0 && chan < nch)
	{
		if( anaped )
		{
			pana.push_back( new caliana::CaliAnaPed( chan ) );
		}
		else
		{
			caliana::CaliAnaPulser *chana = new caliana::CaliAnaPulser( chan, norm );
			chana->SetSearchThresh( thresh );
			chana->SetSearchTimeWindow( tlo, thi );
			if(caliPed)
			{
				int icrm, iview, ichv;
				if( chmap->MapToCRP( chan, icrm, iview, ichv ) > 0)
				{
					float tmp1, tmp2;
					tmp1 = caliPed->Pedestal( icrm, iview, ichv, tmp2 );
					chana->SetPedestal( tmp1, tmp2 );
				}
			}
			pana.push_back( chana );
		}
	}
	else
	{
		for(size_t i=0;i<(size_t)nch;i++)
		{
			if( anaped )
			{
				pana.push_back( new caliana::CaliAnaPed( i ) );
			}
			else
			{
				caliana::CaliAnaPulser *chana = new caliana::CaliAnaPulser( i, norm );
				chana->SetSearchThresh( thresh );
				chana->SetSearchTimeWindow( tlo, thi );
				if(caliPed)
				{
					int icrm, iview, ichv;
					if( chmap->MapToCRP( i, icrm, iview, ichv ) > 0)
					{
						float tmp1, tmp2;
						tmp1 = caliPed->Pedestal( icrm, iview, ichv, tmp2 );
						chana->SetPedestal( tmp1, tmp2 );
					}
				}
				pana.push_back(chana);
			}
		}
	}

	// get data files
	std::vector<string> files;
	getFileNames( fname, nfiles, files );

	// read data
	string runtmstr = ReadData( files, nev, pana );

	// file name to produce
	string toproduce = anatype + "_" + runtmstr + "." + anatype.substr(0,3);
	dlardaq::msg_info<<"Saving results to file "<<toproduce<<endl;
	toproduce = dirout + toproduce;
	ofstream out( toproduce.c_str() );
	if(!out.is_open())
	{
		dlardaq::msg_err<<"Could not open "<<toproduce<<endl;
		exit(-1);
	}

	// some histograms
	TH1F hpedval("pedestal_mean", "pedestal_mean", 50, 0, 50);
	hpedval.SetDirectory(0);
	hpedval.GetXaxis()->SetTitle( "ped (ADC)" );

	TH1F hpedrms("pedestal_rms","pedestal_rms", 50, 0, 20);
	hpedrms.SetDirectory(0);
	hpedrms.GetXaxis()->SetTitle( "ped rms (ADC)" );

	TH1F hchan_peds("chan_ped", "chan_ped", nch, 0, nch);
	hchan_peds.SetDirectory(0);
	hchan_peds.GetXaxis()->SetTitle("ch");
	hchan_peds.GetYaxis()->SetTitle("Pedestal (ADC)");

	TH1F hchan_pedrms("chan_pedrms", "chan_pedrms", nch, 0, nch);
	hchan_pedrms.SetDirectory(0);
	hchan_pedrms.GetXaxis()->SetTitle("ch");
	hchan_pedrms.GetYaxis()->SetTitle("Pedestal RMS (ADC)");


	TH1F hpulse_ampl("signal_peak", "signal_peak", nch, 0, nch);
	hpulse_ampl.SetDirectory(0);
	hpulse_ampl.GetXaxis()->SetTitle("ch");
	hpulse_ampl.GetYaxis()->SetTitle("Pulse Amplitude (ADC)");

	TH1F hpulse_intg("signal_psum", "signal_psum", nch, 0, nch);
	hpulse_intg.SetDirectory(0);
	hpulse_intg.GetXaxis()->SetTitle("ch");
	hpulse_intg.GetYaxis()->SetTitle("Pulse summed (ADC)");

	TH1F hpulse_tpk("signal_tpeak", "signal_tpeak", nch, 0, nch);
	hpulse_tpk.SetDirectory(0);
	hpulse_tpk.GetXaxis()->SetTitle("ch");
	hpulse_tpk.GetYaxis()->SetTitle("Pulse peak time (tdc)");

	TH1F hpulse_afrac("signal_peak_var", "signal_peak_var", 100, -0.5, 0.5);
	hpulse_afrac.SetDirectory(0);
	hpulse_afrac.GetXaxis()->SetTitle("(x - #mu)/#mu");

	TH1F hpulse_ifrac("signal_psum_var", "signal_psum_var", 100, -0.5, 0.5);
	hpulse_ifrac.SetDirectory(0);
	hpulse_ifrac.GetXaxis()->SetTitle("(x - #mu)/#mu");
	hpulse_ifrac.SetLineColor(2);

	std::vector<float> tmp1, tmp2;
	float avg1 = 0;
	float avg2 = 0;
	for(size_t i=0;i<pana.size();i++){
		size_t ch = pana[i]->GetCh();

		float mean, rms;
		map< std::string, std::pair< double, double > > res;
		pana[i]->GetResults( res );

		out<<setw(5)<<ch;

		int icrm, iview, ichv;
		if( chmap->MapToCRP( ch, icrm, iview, ichv ) < 0){continue;}
		out<<setw(3)<<icrm<<setw(2)<<iview<<setw(4)<<ichv;

		//
		if( !anaped ){
			//
			mean = res["pheight"].first;
			rms  = res["pheight"].second;
			hpulse_ampl.SetBinContent( ch+1, mean );
			hpulse_ampl.SetBinError( ch+1, rms );

			//
			float psum     = res["psum"].first;
			float psumrms  = res["psum"].second;
			hpulse_intg.SetBinContent( ch+1, psum );
			hpulse_intg.SetBinError( ch+1, psumrms );

			float tpk    = res["tmax"].first;
			float tpkrms = res["tmax"].second;
			hpulse_tpk.SetBinContent( ch+1, tpk );
			hpulse_tpk.SetBinError( ch+1, tpkrms );

			if( mean != 0 ){
				avg1 += mean;
				tmp1.push_back(mean);
			}

			if(psum != 0){
				avg2 += psum;
				tmp2.push_back( psum );
			}

			out<<setw(15)<<mean
			<<setw(15)<<rms
			<<setw(15)<<psum
			<<setw(15)<<psumrms<<endl;
		}
		else{
			mean = res["pedval"].first;
			rms  = res["pedrms"].first;

			out<<setw(15)<<mean<<setw(15)<<rms<<endl;
		}


		//
		float ped, pedrms;
		float pedv, pedrmsv;
		ped    = res["pedval"].first;
		pedv   = res["pedval"].second;
		pedrms  = res["pedrms"].first;
		pedrmsv = res["pedrms"].second;
		hchan_peds.SetBinContent( ch+1, ped);
		hchan_peds.SetBinError( ch+1, pedv);
		hchan_pedrms.SetBinContent( ch+1, pedrms);
		hchan_pedrms.SetBinError( ch+1, pedrmsv);
		if(ped>0)    RootUtils::FillConstantBinWidth( hpedval, ped );
		if(pedrms>0) RootUtils::FillConstantBinWidth( hpedrms, pedrms );
	}

	//
	out.close();

	// calculate amplitude variation
	avg1 /= tmp1.size();
	for(size_t i=0;i<tmp1.size();i++){
		RootUtils::FillConstantBinWidth( hpulse_afrac, tmp1[i]/avg1-1 );
	}

	// calculate psum variation
	avg2 /= tmp2.size();
	for(size_t i=0;i<tmp2.size();i++){
		RootUtils::FillConstantBinWidth( hpulse_ifrac, tmp2[i]/avg2-1);
	}

	// save histogrammed data
	if( !frootout.empty() ){
		string foutname = dirout + frootout;
		TFile *fout = TFile::Open(foutname.c_str(), "RECREATE");
		fout->cd();
		//pana[0]->Show(NULL, fout, "pedval");
		hpedval.Write();
		hpedrms.Write();
		hchan_peds.Write();
		hchan_pedrms.Write();
		if( !anaped ){
			hpulse_tpk.Write();
			hpulse_ampl.Write();
			hpulse_afrac.Write();
			hpulse_intg.Write();
			hpulse_ifrac.Write();
		}
		fout->Close();
	}

	//
	// clean up
	delete DaqDecoder;
	while(!pana.empty()){
		delete pana.back();
		pana.pop_back();
	}

	//
	return 0;
}
