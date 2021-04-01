// ------------------------------------------------------------
// -----                R3BVftxTrigOnlineSpectra           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BVftxTrigOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BVftxTrigMappedData.h"
#include "R3BSofSciTcalData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TVector3.h"

#include "TClonesArray.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TMath.h"
#include "TRandom.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

R3BVftxTrigOnlineSpectra::R3BVftxTrigOnlineSpectra()
    : FairTask("VftxTrigOnlineSpectra", 1)
    , fVftxTrigMapped(NULL)
    , fSciTcal(NULL)
		, fIdFirstModuleS2(1)
		, fNumModulesS2(2)
		, fIdFirstModuleCaveC(3)
		, fNumModulesCaveC(4)
		, fNEvents(0)
{
}

R3BVftxTrigOnlineSpectra::R3BVftxTrigOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fVftxTrigMapped(NULL)
    , fSciTcal(NULL)
		, fIdFirstModuleS2(1)
		, fNumModulesS2(2)
		, fIdFirstModuleCaveC(3)
		, fNumModulesCaveC(4)
    , fNEvents(0)
{
}

R3BVftxTrigOnlineSpectra::~R3BVftxTrigOnlineSpectra()
{
    LOG(INFO) << "R3BVftxTrigOnlineSpectra::Delete instance";
    if (fVftxTrigMapped)
        delete fVftxTrigMapped;
    if (fSciTcal)
        delete fSciTcal;
}

InitStatus R3BVftxTrigOnlineSpectra::Init()
{
    LOG(INFO) << "R3BVftxTrigOnlineSpectra::Init ";

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BVftxTrigOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------- --- //
    // --- VFTX TRIG at MAPPED --- //
    // --- ------------------- --- //
    fVftxTrigMapped = (TClonesArray*)mgr->GetObject("VftxTrigMappedData");
    if (!fVftxTrigMapped)
    {
        LOG(ERROR) << "R3BVftxTrigOnlineSpectra:: VftxTrigMappedData not found";
        return kFATAL;
    }

    // --- ----------- --- //
    // --- SCI at TCAL --- //
    // --- ----------- --- //
    fSciTcal = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fSciTcal)
    {
        LOG(ERROR) << "R3BVftxTrigOnlineSpectra:: SofSciTcalData not found";
        return kFATAL;
    }

    // Create histograms
    char Name1[255];
    char Name2[255];


		// --- ------------------------ --- //
		// --- DELTA Trigger time at S2 --- //
		// --- ------------------------ --- //
		if(fNumModulesS2>1){
			cDTS2 = new TCanvas("DTS2","DTS2",10,10, 800,700);
			cDTS2->Divide(1,(fNumModulesS2-1));
			fh1_DTS2 = new TH1F*[fNumModulesS2-1];
			for(int m=0; m<fNumModulesS2-1;m++)
			{
				sprintf(Name1,"S2_DeltaTrig_%i",m);
				fh1_DTS2[m] = new TH1F(Name1,Name1,10000,-5000,5000);
				fh1_DTS2[m]->GetXaxis()->SetTitle(Form("Trig[0]-Trig[%i]",m+1));
				cDTS2->cd(m+1);
				fh1_DTS2[m]->Draw();
			}
		} // end of if fNumModulesS2 >1

		// --- ---------------------------- --- //
		// --- DELTA Trigger time at Cave C --- //
		// --- ---------------------------- --- //
		if(fNumModulesCaveC>1){
			cDTCC = new TCanvas("DTCC","DTCC",10,10, 800,700);
			cDTCC->Divide(1,(fNumModulesCaveC-1));
			fh1_DTCC = new TH1F*[fNumModulesCaveC-1];
			for(int m=0; m<fNumModulesCaveC-1;m++)
			{
				sprintf(Name1,"CC_DeltaTrig_%i",m);
				fh1_DTCC[m] = new TH1F(Name1,Name1,10000,-5000,5000);
				fh1_DTCC[m]->GetXaxis()->SetTitle(Form("Trig[0]-Trig[%i]",m+1));
				cDTCC->cd(m+1);
				fh1_DTCC[m]->Draw();
			}
		} // end of if fNumModulesS2 >1

		// --- --------------------------------------------------------- --- //
		// --- DELTA Trigger time between first modules at S2 and Cave C --- // 
		// --- --------------------------------------------------------- --- //
		if(fNumModulesS2>0 && fNumModulesCaveC>0)
		{
			cDTS2CC = new TCanvas("DTS2CC","DTS2CC",10,10,800,700);
			fh1_DTS2CC = new TH1F("S2CC_DeltaTrig","S2CC_DeltaTrig",10000, -5000, 5000);
			fh1_DTS2CC->GetXaxis()->SetTitle(Form("TtrigS2 - TtrigCC"));
			cDTS2CC->cd();
			fh1_DTS2CC->Draw();
		}


    // Folder
    TFolder* FoldCorr = new TFolder("VftxTr", "VftxTriggerTime");
    if(fVftxTrigMapped){
			if(fNumModulesS2>1)
			{
				FoldCorr->Add(cDTS2);
			}
			if(fNumModulesCaveC>1)
			{
				FoldCorr->Add(cDTCC);
			}
			if(fNumModulesS2>0 && fNumModulesCaveC>0)
			{
				FoldCorr->Add(cDTS2CC);
			}
		}
		run->AddObject(FoldCorr);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_VftxTr_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));
    return kSUCCESS;
}

void R3BVftxTrigOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BVftxTrigOnlineSpectra::Reset_Histo";
		// Reset histograms
    if (fVftxTrigMapped)
    {
			if(fNumModulesS2>1)
			{
				for(int m=0; m<fNumModulesS2-1; m++)
				{
					fh1_DTS2[m]->Reset();
				}
			}
			if(fNumModulesCaveC>1)
			{
				for(int m=0; m<fNumModulesCaveC-1; m++)
				{
					fh1_DTCC[m]->Reset();
				}
			}
			if(fNumModulesS2>0 && fNumModulesCaveC>0)
			{
				fh1_DTS2CC->Reset();
			}
    }
}

void R3BVftxTrigOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BVftxTrigOnlineSpectra::Exec FairRootManager not found";
   
		UInt_t CoarseTime[fNumModulesS2+fNumModulesCaveC];
		for(int m=0; m<fNumModulesS2+fNumModulesCaveC; m++)
		{
			CoarseTime[m] = 0.;
		}
 
		if(fVftxTrigMapped && fVftxTrigMapped->GetEntriesFast()>0)
		{
			for(int entry=0; entry<fVftxTrigMapped->GetEntriesFast(); entry++)
			{
				R3BVftxTrigMappedData * hit = (R3BVftxTrigMappedData*)fVftxTrigMapped->At(entry);
				if(!hit)
					continue;
				CoarseTime[hit->GetModuleID()-1] = hit->GetTimeTrig();	
			}
		}	

		Int_t iStop;
	  for(int m=0; m<fNumModulesS2-1; m++){
			iStop = fIdFirstModuleS2 + m;
			fh1_DTS2[m]->Fill(CoarseTime[fIdFirstModuleS2-1]-CoarseTime[iStop]); 
		}	
	  for(int m=0; m<fNumModulesCaveC-1; m++){
			iStop = fIdFirstModuleCaveC + m;
			fh1_DTCC[m]->Fill(CoarseTime[fIdFirstModuleCaveC-1]-CoarseTime[iStop]); 
		}	
		if(fNumModulesS2>0 && fNumModulesCaveC>0)
		{
			fh1_DTS2CC->Fill(CoarseTime[fIdFirstModuleS2-1]-CoarseTime[fIdFirstModuleCaveC-1]);
		}	
		fNEvents += 1;
}

void R3BVftxTrigOnlineSpectra::FinishEvent()
{
    if (fVftxTrigMapped)
        fVftxTrigMapped->Clear();
    if (fSciTcal)
        fSciTcal->Clear();
}

void R3BVftxTrigOnlineSpectra::FinishTask()
{
		// Write
    if (fVftxTrigMapped)
    {
			if(fNumModulesS2>1)
			{
				cDTS2->Write();	
			}
			if(fNumModulesCaveC>1)
			{
				cDTCC->Write();
			}
			if(fNumModulesS2>0 && fNumModulesCaveC>0)
			{
				cDTS2CC->Write();
			}
    }
}

ClassImp(R3BVftxTrigOnlineSpectra)
