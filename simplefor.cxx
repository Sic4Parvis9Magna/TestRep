
//C++
#include "iostream"
#include "string"
#include <stdio.h> 

//Root
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"



using namespace std;





void simpleFit(TH1D* data){
	
	
	
	
	TF1* f1 = new TF1("f1","gaus",-5150,-4800);

	TF1* f2 = new TF1("f2","gaus",-200,200);
	
	
	TCanvas* c6 = new TCanvas("c6","Result fit",600,400);
	
	c6->cd();
	
	data->Fit(f1,"R","N",-5060,-4900);
	data->Fit(f2,"R+","N",-150,150);
	printf("F1: NDF = %d\t F2: NGF = %d \n\n",f1->GetNDF(),f2->GetNDF());
	printf("F1: param0(%s) = %f     F2: param0(%s) = %f\n",f1->GetParName(0),f1->GetParameter(0),
														f2->GetParName(0),f2->GetParameter(0));
	printf("F1: param1(%s) = %f     F2: param1(%s) = %f\n",f1->GetParName(1),f1->GetParameter(1),
														f2->GetParName(1),f2->GetParameter(1));
	printf("F1: param2(%s) = %f     F2: param2(%s) = %f\n\n",f1->GetParName(2),f1->GetParameter(2),
														f2->GetParName(2),f2->GetParameter(2));
	printf("F1: Chi2 = %f     F2: Chi2 = %f\n\n",f1->GetChisquare(),f2->GetChisquare());
																											
	
	data->Draw("C");
	//numOfPeaks = data->ShowPeaks();
	

	TAxis* axis = data->GetXaxis();
	double val1 = data->Integral(axis->FindBin(f1->GetXmin()),axis->FindBin(f1->GetXmax()) ) ;
	double val2 = data->Integral(axis->FindBin(f2->GetXmin()),axis->FindBin(f2->GetXmax()) );
	printf( "dF1 integral=  %f\n"  ,val1);
	//printf("f1 int= %f\n",f1->Integral(f1->GetXmin(),f1->GetXmax()));
	
	printf( "dF2 integral=  %f\n"  ,val2);
	printf("summ(dF1+dF2) = %f\n", (val1 + val2));
	printf("deflection = %f\n",val1*100/(val1+val2));
	printf("Full Integrad from data = %f\n\n",data->Integral());
	printf("F1: Xmax= %f; Xmin= %f\n",f1->GetXmax(),f1->GetXmin());
	printf("F2: Xmax= %f; Xmin= %f\n",f2->GetXmax(), f2->GetXmin());

	printf("\n|CUSTOM INTAGRALS|\n");
	
	double peak1;
	double peak2;
	double sigma1 =  f1->GetParameter(2);
	double sigma2 =  f2->GetParameter(2);
	double mean1 = f1->GetParameter(1);
	double mean2 = f2->GetParameter(1);
	peak1 = data->Integral(axis->FindBin(mean1 -(3*sigma1)),axis->FindBin(mean1 +(3*sigma1)));
	peak2 = data->Integral(axis->FindBin(mean2 -(3*sigma2)),axis->FindBin(mean2 +(3*sigma2)));
	printf("peak1:= %f     peak2:= %f\n",peak1,peak2);
	double peakDeflection = 100*peak1/(peak1+peak2);
	printf("Deflection = %f %%\n",peakDeflection);
	
	
	TAxis* xaxis =  f2->GetXaxis();
	double bmax = xaxis->GetXmax();
	double bmin = xaxis->GetXmin();
	double countInt = f2->Integral(bmin,bmax);
	
	//printf("f2 int = %f\n", countInt);
	//gStyle->SetOptStat(1111111);
   // gStyle->SetOptFit(1111111);
}



void dispDates(){

	//**************************************************************************************************************
	//*****************************************Data Extraction******************************************************
	//**************************************************************************************************************
	const int num_of_files = 7;
	string path = "/home/maxim/Buffer/root_exampls/myRun/2018_10_18_H8_pion/";
	string files[num_of_files];
	files[0]="recoDataSimple_5538_4plane_offset10_0.root";
	files[1]="recoDataSimple_5538_4plane_offset10_1.root";
	files[2]="recoDataSimple_5539_4plane_offset10_0.root";
	files[3]="recoDataSimple_5541_4plane_offset10_0.root";
	files[4]="recoDataSimple_5542_4plane_offset10_0.root";
	files[5]="recoDataSimple_5544_4plane_offset10_0.root";
	files[6]="recoDataSimple_5545_4plane_offset10_0.root";
	

	for(int i=0; i < num_of_files; i++)
	   printf("file # %d :: %s\n",(i+1),(files[i]).c_str());

	printf("Type number of file: ");

	int choise=0;

	cin>>choise;

	printf("You choose file #%d.\n Open file..\n",choise);
	string abspath = path + files[choise-1];

	TString _fileName = abspath;
	TChain* fChain = new TChain("simpleEvent");
    fChain->Add(_fileName.Data());

   int nentries = fChain->GetEntries();
	printf("default print func!\n");
	fChain->Print();
	
	printf("total number of enteris = %d",nentries);
	char ans;
	do{
	printf("\nAre you ready to proceed ?[y/n] ");
	cin >> ans;
	}while(ans != 'n' && ans != 'y');

	if(ans == 'n') return;

	//************************************************************************************************
	//**********************************DATA PRE-SETUP************************************************
	//************************************************************************************************


	struct myEvent{
		int run;
		int evtnum;
		int nuclear;
		int nuclearRaw;
	};

	myEvent myEv;
	

	struct myGonioPosition{
		double x;
		double y;
		double z;
	}gPos;

	struct myMultihits{
		int p_nHits[5];
		double thx_in;
		double thy_in;
		double thxErr_in;
		double thyErr_in;
		double dO_x;
		double dO_y;
		double dO_xErr;
		double dO_yErr;
		
	}multHits;

	struct myTracks{
		double thx_in;
		double thy_in;
		double thx_out;
		double thy_out;
		double thxErr_in;
		double thyErr_in;
		double thxErr_out;
		double thyErr_out;
		double dO_x;
		double dO_y;
		double dO_xErr;
		double dO_yErr;
		double chi2_x;
		double chi2_y;

	}track;

	
	
	int isTrack;
	int isHit;
	char myDate[80];
	char myTime[80];
	string strDate;
	string strTime;
	//************************************************************************************************
	//************************************************************************************************

    TBranch* brEvent           = fChain->GetBranch("Event");
    TBranch* bTime             = fChain->GetBranch("Time");
    TBranch* bDate             = fChain->GetBranch("Date");
    TBranch* bgonioPosition    = fChain->GetBranch("GonioPos");
    TBranch* bMultiHit         = fChain->GetBranch("MultiHit");
    TBranch* bMultihits        = fChain->GetBranch("MultiHits");
    TBranch* bSingleTrack      = fChain->GetBranch("SingleTrack");
    TBranch* bTracks           = fChain->GetBranch("Tracks");

	printf( "Branches were created\n");
	brEvent->SetAddress(&myEv);
	bTime->SetAddress(&myTime);
	bDate->SetAddress(&myDate);
	bgonioPosition->SetAddress(&gPos);
	bMultiHit->SetAddress(&isHit);
	bMultihits->SetAddress(&multHits);
	bSingleTrack->SetAddress(&isTrack);
	bTracks->SetAddress(&track);

	printf( "connections is setted\n");
	fChain->GetEntry(0);
	TString runNumber = "";
		runNumber = std::to_string(myEv.run);
	TString eventNumber = "";
		eventNumber = std::to_string(myEv.evtnum);
	//**************************************************************************************************************
	//*****************************************Work with Data*******************************************************
	//**************************************************************************************************************
	int numOfBins = (int)sqrt(nentries);
	printf("for %d enteries we have %d bins\n",nentries,numOfBins);
	
	//X Deflection vs X Impact Position
	TH2D *th1 = new TH2D();
	TH2D *th2 = new TH2D();
	TH2D *th3 = new TH2D();
    TH2D *th4 = new TH2D();

	TH2D *th5 = new TH2D();
	TH2D *th6 = new TH2D();
	TH2D *th7 = new TH2D();
   

	TH1D *h1 = new TH1D();

	bool plotFlag1 = false;
	bool plotFlag2 = false;

	//************************************************************************************************
	///*************************************CUTS******************************************************
	//************************************************************************************************
	double minXImpCut = -0.5;//[mm]
	double maxXImpCut = 0.0;//[mm]

	double minYImpCut = -1.0;//[mm]
	double maxYImpCut = 1.0;//[mm]

	double minXthCut = 12;//[mkrad]
	double maxXthCut = 22;//[mkrad]

	double minYthCut = -15;//[mkrad]
	double maxYthCut = 15;//[mkrad]
	//************************************************************************************************
	//************************************************************************************************
	int  cutEvents = 0;

	for(int i=0;i<nentries;i++){
		fChain->GetEntry(i);
		if((track.dO_x < maxXImpCut && track.dO_x > minXImpCut)&&
		 (track.thx_in*1e6 > minXthCut && track.thx_in*1e6 < maxXthCut)){
			
		 
		 																
			 cutEvents++;
		 } 
	}
	
	
	int newNum = (int)sqrt(cutEvents);
	

	printf("Num of cut events = %d, new number of bins for cuted events is %d\n",cutEvents,newNum);

	h1->SetBins(newNum,-5200,200);

	th1->SetBins(numOfBins,-1,0.5,numOfBins,-200,200);
	th2->SetBins(numOfBins,-5,5,numOfBins,-200,200);
	th3->SetBins(numOfBins,-200,200,numOfBins,-200,200);
    th4->SetBins(numOfBins,-200,200,numOfBins,-200,200);

	th5->SetBins(numOfBins,-50,50,numOfBins,-5700,700);
	th6->SetBins(newNum,-2,2,newNum,-5200,200);
	th7->SetBins(newNum,-50,50,newNum,-5200,200);
    

	printf( "th2d bins is seted\n");
	printf( "Begin filling \n");

	int simpleCounter = 0;
	int const countRange = 1e4;

	for(int i=0; i<nentries;i++){
		
		fChain->GetEntry(i);
				
		if((track.dO_x < maxXImpCut && track.dO_x > minXImpCut)&&
		(track.thx_in*1e6 > minXthCut && track.thx_in*1e6 < maxXthCut) ){
			
		 
		 th2->Fill(track.dO_y,(track.thy_out-track.thy_in)*1e6);//YImp vs YDef																){
				
             
		h1->Fill((track.thx_out-track.thx_in)*1e6);

			if(plotFlag2){
				
				th6->Fill(track.dO_x,(track.thx_out-track.thx_in)*1e6);
				th7->Fill((track.thx_in*1e6),(track.thx_out-track.thx_in)*1e6);
			}
		}
		if(plotFlag2) {th5->Fill((track.thx_in*1e6),(track.thx_out-track.thx_in)*1e6);}//X ImpAngl vs X Def

		if(plotFlag1){


		th1->Fill(track.dO_x,(track.thx_out-track.thx_in)*1e6);//XImp vs XDef
		//th2->Fill(track.dO_y,(track.thy_out-track.thy_in)*1e6);//YImp vs YDef
		th3->Fill((track.thx_in*1e6),(track.thx_out-track.thx_in)*1e6);//X theta_in vs X Def
        th4->Fill((track.thy_in)*1e6,(track.thy_out-track.thy_in)*1e6);//Y theta_in vs Y Def
		}
		

	}
	printf( "End of filling \n");
	h1->SetTitle("X Deflection ;X Deflection [mkrad] ;Counts ");

	if(plotFlag1){
	TString title1 = "X Deflection vs X Impact Position\n Run number: ";
	title1.Append(runNumber);
	title1.Append( " EventNumber: ");
	title1.Append( eventNumber);
	title1.Append( ";X Impact [mm] ; X Deflection [mkrad]");
	th1->SetTitle(title1);

	TString title2 = "Y Deflection vs Y Impact Position\n Run number: ";
	title2.Append(runNumber);
	title2.Append( " EventNumber: ");
	title2.Append( eventNumber);
	title2.Append( ";Y Impact [mm] ; Y Deflection [mkrad]");

	th2->SetTitle(title2);

	TString title3 = "X Deflection vs X Theta IN\n Run number: ";
	title3.Append(runNumber);
	title3.Append( " EventNumber: ");
	title3.Append( eventNumber);
	title3.Append( ";X Theta IN [mkrad] ; X Deflection [mkrad]");
	th3->SetTitle(title3);

	TString title4 = "Y Deflection vs Y Theta IN\n Run number: ";
	title4.Append(runNumber);
	title4.Append( " EventNumber: ");
	title4.Append( eventNumber);
	title4.Append( ";Y Theta IN [mkrad] ; Y Deflection [mkrad]");
	th4->SetTitle(title4);


	

	TCanvas* c3 = new TCanvas("c3","Two D plots",1200,1200);
	c3->Divide(2,2);
	gStyle->SetOptStat(0);
	
	c3->cd(1);	
	th1->Draw("colz");

	c3->cd(2);	
	th2->Draw("colz");

	c3->cd(3);
	th3->Draw("colz");
	
	c3->cd(4);
	th4->Draw("colz");
	}

	if(plotFlag2){
		th5->SetTitle("X Deflection vs X Impact angle;X Impact Angle [mkrad];X Deflection [mkrad];");
		th6->SetTitle("X Deflection vs X Impact position;X Impact Position [mm];X Deflection [mkrad];");
		th7->SetTitle("X Deflection vs X Theta IN;X ThetaIN [mkrad];X Deflection [mkrad];");

		TCanvas* c4 = new TCanvas("c4","Def 2D",1200,1200);
		c4->Divide(2,2);
		gStyle->SetOptStat(0);
	
		c4->cd(1);	
		th5->Draw("colz");

		c4->cd(2);	
		th6->Draw("colz");

		c4->cd(3);
		th7->Draw("colz");
	
		c4->cd(4);
		h1->Draw("C");
	}
	printf("Plotting canvas\n");
	simpleFit(h1);
	
}


