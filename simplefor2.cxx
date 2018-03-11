
//C++
#include "iostream"
#include "string"
#include <stdio.h> 

//Root
#include "TTree.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH1F.h"


using namespace std;

void myDisp(char a[]){


printf("%s",a);

}





void simplefor2(){

	TFile file("/home/maxim/Buffer/root_exampls/myRun/2018_10_18_H8_pion/	recoDataSimple_5538_4plane_offset10_0.root");

	TTree* t;

	file.GetObject("simpleEvent",t);
	cout << "return tree"<< endl;

	cout << "initilise success" << endl;
	long long entery = t->GetEntries();

	struct myEvent{
		int run;
		int evtNum;
		int nuclear;
		int nuclearRaW;
	}myEv;

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
		double thxErr_in;
		double thyErr_in;
		double thx_out;
		double thy_out;
		double thxErr_out;
		double thyErr_out;
		double dO_x;
		double dO_y;
		double dO_xErr;
		double dO_yErr;
		double chi2_x;
		double chi2_y;

	}track;

	
	cout << "Num of enteries: " << entery << endl;
	int isTrack;
	int isHit;
	char myDate[80];
	char myTime[80];
	string strDate;
	string strTime;
	t->SetBranchAddress("Date",&myDate);
	t->SetBranchAddress("Time",&myTime);

	TBranch* brEvent = t->GetBranch("Event");
	TBranch* bgonioPosition= t->GetBranch("GonioPos");
	TBranch* bMultihits = t->GetBranch("MultiHits");
	TBranch* bTracks = t->GetBranch("Tracks");

	brEvent->SetAddress(&myEv);
	bgonioPosition->SetAddress(&gPos);
	bMultihits->SetAddress(&multHits);
	bTracks->SetAddress(&track);

	
	double maxX,maxY,minX,minY;
	t->GetEntry(0);
	maxX = track.thx_in;
	minX = track.thx_in;
	maxY = track.thy_in;
	minY = track.thy_in;

	for(int i=1; i<entery;i++){
		t->GetEntry(i);
		if(maxX < track.thx_in) maxX = track.thx_in;
		if(minX > track.thx_in) minX = track.thx_in;
		if(maxY < track.thy_in) maxY = track.thy_in;
		if(minY > track.thy_in) minY = track.thy_in;
		
	}
	cout << "x: from " << minX << " to " << maxX << endl;
	cout << "y: from " << minY << " to " << maxY << endl;
	TCanvas* c2 = new TCanvas("c2","Teta_in_x/y",1200,800);
	c2->Divide(2,1);
	c2->cd(1);
	TH1D *X = new TH1D("X","thetaX_in",100,0,10);
	TH1D *Y = new TH1D("Y","thetaY_in",100,minY*1e6,maxY*1e6);
	double x[100];
	double y[100];

	for(int i=0; i<1e4;i++){
		
		t->GetEntry(i);
		X->Fill(track.thx_in*1e6);
		Y->Fill(track.thy_in*1e6);
		cout << track.thx_in*1e6 << endl;
		x[i]=track.thx_in;
		y[i]=track.thy_in;

	}
	X->Draw();
	///c2->cd(2);
	//tetY->Draw();
	
	//TGraph *f= new TGraph(100,x,y);
	//f->Draw();

		/*strDate += date[0]; strDate += date[1]; strDate += "/"; strDate +=date[2];strDate += date[3]; strDate += "/"; 
		strDate += date[4];strDate += date[5];
		strTime += time[0]; strTime +=time[1]; strTime += " : "; strTime += time[2]; strTime += time[3]; strTime += " : ";
		strTime += time[4]; strTime +=time[5];
		cout << "date[" << i << "]: " <<strDate  << "  time: " << strTime <<  endl; 

		strDate = "";
		strTime = "";*/
}
