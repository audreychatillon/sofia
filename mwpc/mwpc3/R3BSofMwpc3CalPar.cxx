// --------------------------------------------------------
// -----                                              -----
// -----		 R3BSofMwpc3CalPar            -----
// -----       Created 11/10/2019 by G.García Jiménez -----
// --------------------------------------------------------

#include "R3BSofMwpc3CalPar.h"
#include "FairLogger.h"
#include "FairParamList.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

/* ---- Standard Constructor ---- */
R3BSofMwpc3CalPar::R3BSofMwpc3CalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumPadsX(288)
    , fNumPadsY(120)
    , fNumParamsFit(2)
{
    fPadCalParams = new TArrayF((fNumPadsX + fNumPadsY) * fNumParamsFit);
}

/* ----  Destructor ---- */
R3BSofMwpc3CalPar::~R3BSofMwpc3CalPar()
{
    clear();
    delete fPadCalParams;
}

/* ----  Method clear ---- */
void R3BSofMwpc3CalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

/* ----  Method putParams ---- */
void R3BSofMwpc3CalPar::putParams(FairParamList* list)
{

    LOG(INFO) << "R3BSofMwpc3CalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fPadCalParams->Set(array_size);

    list->add("mwpc3CalPar", *fPadCalParams);
    list->add("mwpc3PadXNumberPar", fNumPadsX);
    list->add("mwpc3PadYNumberPar", fNumPadsY);
    list->add("mwpc3ParamsFitPar", fNumParamsFit);
}

/* ----  Method getParams ---- */
Bool_t R3BSofMwpc3CalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofMwpc3CalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("mwpc3PadXNumberPar", &fNumPadsX))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc3PadYNumberPar", &fNumPadsY))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc3ParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;
    fPadCalParams->Set(array_size);

    if (!(list->fill("mwpc3CalPar", fPadCalParams)))
    {
        LOG(INFO) << "---Could not initialize mwpc3CalPar";
        return kFALSE;
    }

    return kTRUE;
}

/* ----  Method printParams ---- */
void R3BSofMwpc3CalPar::printParams()
{
    LOG(INFO) << "R3BSofMwpc3CalPar: SOFIA MWPC3 Calibration Parameters: ";
    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    for (Int_t i = 0; i < (fNumPadsX + fNumPadsY); i++)
    {
        if (i < fNumPadsX)
        {
            LOG(INFO) << "MWPC3 Plane X Pad Number: " << i + 1;
        }
        else
        {
            LOG(INFO) << "MWPC3 Plane Y Pad Number: " << i + 1 - fNumPadsX;
        }
        for (Int_t j = 0; j < fNumParamsFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fPadCalParams->GetAt(i * fNumParamsFit + j);
        }
    }
}

ClassImp(R3BSofMwpc3CalPar)
