// ------------------------------------------------------------
// -----                R3BVftxTrigOnlineSpectra           -----
// ------------------------------------------------------------

#ifndef R3BVftxTrigOnlineSpectra_H
#define R3BVftxTrigOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads FRS data and plots online histograms
 */
class R3BVftxTrigOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BVftxTrigOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BVftxTrigOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BVftxTrigOnlineSpectra();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

    /**
     * Method for event loop implementation.
     * Is called by the framework every time a new event is read.
     * @param option an execution option.
     */
    virtual void Exec(Option_t* option);

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    /**
     * Method for finish of the task execution.
     * Is called by the framework after processing the event loop.
     */
    virtual void FinishTask();

    /**
     * Methods to clean histograms.
     */
    virtual void Reset_Histo();

		void SetIdFirstModuleS2(int id) {fIdFirstModuleS2 = id;}
		void SetNumModulesS2(int num)   {fNumModulesS2 = num;}
		void SetIdFirstModuleCaveC(int id) {fIdFirstModuleCaveC = id;}
		void SetNumModulesCaveC(int num)   {fNumModulesCaveC = num;}



  private:
    TClonesArray* fVftxTrigMapped;  // Array with VftxTrig mapped items. //
    TClonesArray* fSciTcal;         // Array with SofSci tcal items.  //


		Int_t fIdFirstModuleS2;
	  Int_t fNumModulesS2;
		Int_t fIdFirstModuleCaveC;
	  Int_t fNumModulesCaveC;
    Int_t fNEvents;


		// Delta Trigger time VFTX at S2
		TCanvas* cDTS2;
		TH1F**   fh1_DTS2;

		// Delta Trigger time VFTX at Cave C
		TCanvas* cDTCC;
		TH1F**   fh1_DTCC;

		// Delta Trigger time VFTX FirstS2 - FirstCaveC
		TCanvas* cDTS2CC;
		TH1F*    fh1_DTS2CC;

  public:
    ClassDef(R3BVftxTrigOnlineSpectra, 0)
};

#endif
