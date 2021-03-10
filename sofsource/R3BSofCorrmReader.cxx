#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofCorrmMappedData.h"
#include "R3BSofCorrmReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofcorrm.h"
}

#include <iostream>

using namespace std;

R3BSofCorrmReader::R3BSofCorrmReader(EXT_STR_h101_SOFCORRM* data, UInt_t offset)
    : R3BReader("R3BSofCorrmReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofCorrmMappedData")) // class name
    , fNumEntries(0)
{
}

R3BSofCorrmReader::R3BSofCorrmReader(EXT_STR_h101_SOFCORRM* data, UInt_t offset, Int_t num)
    : R3BReader("R3BSofCorrmReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofCorrmMappedData")) // class name
    , fNumEntries(0)
{
}

R3BSofCorrmReader::~R3BSofCorrmReader()
{
    LOG(INFO) << "R3BSofCorrmReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofCorrmReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofCorrmReader::Init";
    EXT_STR_h101_SOFCORRM_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFCORRM, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofCorrmReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("SofCorrmMappedData", "SofCorrm", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("SofCorrmMappedData", "SofCorrm", fArray, kFALSE);
    }
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFCORRM_onion* data = (EXT_STR_h101_SOFCORRM_onion*)fData;
    data.SOFCORRM_TRM = 0; 
    data.SOFCORRM_TTM = 0;

    return kTRUE;
}

Bool_t R3BSofCorrmReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFCORRM_onion* data = (EXT_STR_h101_SOFCORRM_onion*)fData;

    uint32_t nTR = data->SOFCORRM_TRM;
    uint32_t nTT = data->SOFCORRM_TTM;
    if(nTR == nTT && nTT==1){
    	uint32_t idTR = 
    }
    // loop over all detectors
    for (int d = 0; d < fNumSci; d++)
    {
        uint32_t numberOfPMTsWithHits_TF = data->SOFCORRM[d].TFM;
        uint32_t numberOfPMTsWithHits_TC = data->SOFCORRM[d].TCM;
        if (numberOfPMTsWithHits_TF != numberOfPMTsWithHits_TC)
        {
            LOG(ERROR) << "R3BSofCorrmReader::Read() Error in unpacking, unconsistency between TF and TC for SofCorrm !";
        }
        else
        {
            // loop over channels with hits
            uint32_t curChannelStart = 0;
            for (Int_t pmmult = 0; pmmult < numberOfPMTsWithHits_TF; pmmult++)
            {
                uint32_t pmtid_TF = data->SOFCORRM[d].TFMI[pmmult];
                uint32_t pmtid_TC = data->SOFCORRM[d].TCMI[pmmult];
                if (pmtid_TF != pmtid_TC)
                {
                    LOG(ERROR) << "R3BSofCorrmReader::Read() Error in unpacking, unconsistency between the PMT id for TF "
                                  "and TC for SofCorrm !";
                }
                uint32_t nextChannelStart = data->SOFCORRM[d].TFME[pmmult];
                // put the mapped items {det,pmt,finetime, coarsetime} one after the other in the fArray
                for (Int_t hit = curChannelStart; hit < nextChannelStart; hit++)
                {
                    auto item = new ((*fArray)[fNumEntries++]) R3BSofCorrmMappedData(d + 1, // 1-based numbering
                                                                                   pmtid_TF,
                                                                                   data->SOFCORRM[d].TCv[hit],
                                                                                   data->SOFCORRM[d].TFv[hit]);
                }
                curChannelStart = nextChannelStart;
            }
        }
    } // end of for(d)
    return kTRUE;
}

void R3BSofCorrmReader::Reset()
{
    // Reset the output array
    fArray->Clear();
    fNumEntries = 0;
}

ClassImp(R3BSofCorrmReader)
