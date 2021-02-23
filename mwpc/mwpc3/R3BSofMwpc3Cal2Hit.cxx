// ----------------------------------------------------------------------
// -----                     R3BSofMwpc3Cal2Hit                     -----
// -----             Created 14/10/19  by G. García Jiménez         -----
// -----             by modifying J.L classes for MWPC0             -----
// ----------------------------------------------------------------------

#include "TClonesArray.h"
#include "TMath.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

// MWPC headers
#include "R3BSofMwpc3Cal2Hit.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcHitData.h"

bool compare(pair<int, int> p1, pair<int, int> p2) { return p1.second < p2.second; }

/* ---- R3BSofMwpc3Cal2Hit: Default Constructor ---- */

R3BSofMwpc3Cal2Hit::R3BSofMwpc3Cal2Hit()
    : FairTask("R3B Hit-MWPC3 Task", 1)
    , fMwpcCalDataCA(NULL)
    , fMwpcHitDataCA(NULL)
    , fTofWallHitDataCA(NULL)
    , fwy(5.000)
    , fwx(3.125)
    , fSizeX(900.0)
    , fSizeY(600.0) // in mm
    , fOnline(kFALSE)
    , fTofWallMatching(kFALSE)
{
}

/* ---- R3BSofMwpc3Cal2Hit: Standard Constructor ---- */
R3BSofMwpc3Cal2Hit::R3BSofMwpc3Cal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMwpcCalDataCA(NULL)
    , fMwpcHitDataCA(NULL)
    , fTofWallHitDataCA(NULL)
    , fwy(5.000)
    , fwx(3.125)
    , fSizeX(900.0)
    , fSizeY(600.0) // in mm
    , fOnline(kFALSE)
    , fTofWallMatching(kFALSE)
{
}

/* ---- Virtual R3BSofMwpc3Cal2Hit: Destructor ---- */
R3BSofMwpc3Cal2Hit::~R3BSofMwpc3Cal2Hit()
{
    LOG(INFO) << "R3BSofMwpc3Cal2Hit: Delete instance";
    if (fMwpcCalDataCA)
        delete fMwpcCalDataCA;
    if (fMwpcHitDataCA)
        delete fMwpcHitDataCA;
    if (fTofWallHitDataCA)
        delete fTofWallHitDataCA;
}

/* ---- Public method Init   ---- */
InitStatus R3BSofMwpc3Cal2Hit::Init()
{
    LOG(INFO) << "R3BSofMwpc3Cal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpcCalDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3CalData");
    if (!fMwpcCalDataCA)
    {
        return kFATAL;
    }

    fTofWallHitDataCA = (TClonesArray*)rootManager->GetObject("TofWHitData");
    if (!fTofWallHitDataCA)
    {
        fTofWallMatching = kFALSE;
    }
    else
        fTofWallMatching = kTRUE;

    // OUTPUT DATA
    // Hit data
    fMwpcHitDataCA = new TClonesArray("R3BSofMwpcHitData", 10);

    if (!fOnline)
    {
        rootManager->Register("Mwpc3HitData", "MWPC3 Hit", fMwpcHitDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("Mwpc3HitData", "MWPC3 Hit", fMwpcHitDataCA, kFALSE);
    }

    return kSUCCESS;
}

/* ----   Public method ReInit   ---- */
InitStatus R3BSofMwpc3Cal2Hit::ReInit() { return kSUCCESS; }

/* ----   Public method Execution   ---- */
void R3BSofMwpc3Cal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (fTofWallMatching)
        ReconstructHitWithTofWallMatching();

    else
    {
        // Reading the Input -- Cal Data --
        Int_t nHits = fMwpcCalDataCA->GetEntries();
        if (!nHits)
            return;

        // Data from cal level
        R3BSofMwpcCalData** calData;
        calData = new R3BSofMwpcCalData*[nHits];
        Int_t planeId;
        Int_t padId;
        Int_t padmx = -1, padmy = -1;
        Double_t q = 0, qmx = 0, qmy = 0, qleft = 0, qright = 0, qdown = 0, qup = 0;
        Double_t x = -1000., y = -1000.;

        for (Int_t i = 0; i < Mw3PadsX; i++)
            fx[i] = 0;
        for (Int_t i = 0; i < Mw3PadsY; i++)
            fy[i] = 0;

        for (Int_t i = 0; i < nHits; i++)
        {
            calData[i] = (R3BSofMwpcCalData*)(fMwpcCalDataCA->At(i));
            planeId = calData[i]->GetPlane();
            padId = calData[i]->GetPad();
            q = calData[i]->GetQ();
            if (planeId == 1)
                fx[padId] = q;
            else
                fy[padId] = q;
            if (q > qmx && planeId == 1)
            {
                qmx = q;
                padmx = padId;
            }
            if (q > qmy && planeId == 3)
            {
                qmy = q;
                padmy = padId;
            }
        }
        // Add Hit data ----
        if (padmx > 0 && padmy > 0 && padmx + 1 < Mw3PadsX && padmy + 1 < Mw3PadsY && qmx > 0 && qmy > 0)
        {
            // Obtain position X ----
            qleft = (Double_t)fx[padmx - 1];
            qright = (Double_t)fx[padmx + 1];
            // std::cout<<qleft<<" "<<qright<<std::endl;
            if (qleft > 0 && qright > 0)
                x = GetPositionX(qmx, padmx, qleft, qright);

            // Obtain position Y ----
            qdown = fy[padmy - 1];
            qup = fy[padmy + 1];
            // if(padmy==64) std::cout << padmy << " " << qmy << " " << qdown << " " << qup << std::endl;
            // if(padmy==63) std::cout << padmy << " " << qmy << " " << qdown << " " << qup << std::endl;
            if (qdown > 0 && qup > 0)
                y = GetPositionY(qmy, padmy, qdown, qup);

            AddHitData(x, y);
        }

        if (calData)
            delete calData;
    }
    return;
}

/* ---- Indentifying cluster and matching good hits using the tofwall information ---- */
void R3BSofMwpc3Cal2Hit::ReconstructHitWithTofWallMatching()
{
    // Getting Position information from tof wall

    Int_t twHits = fTofWallHitDataCA->GetEntries();
    if (!twHits)
        return;
    R3BSofTofWHitData** twHitData;
    twHitData = new R3BSofTofWHitData*[twHits];
    vector<double> twX;
    vector<double> twY;
    twX.clear();
    twY.clear();
    for (Int_t i = 0; i < twHits; i++)
    {
        twX.push_back(twHitData[i]->GetX());
        twY.push_back(twHitData[i]->GetY());
    }
    Int_t nHits = fMwpcCalDataCA->GetEntries();
    if (!nHits)
        return;
    // Data from cal level
    R3BSofMwpcCalData** calData;
    calData = new R3BSofMwpcCalData*[nHits];
    Int_t planeId;
    Int_t padId;
    Int_t q;
    /*fQX.clear();
    fQY.clear();
    fPadX.clear();
    fPadY.clear();*/
    fPairX.clear();
    fPairY.clear();
    fPairX.reserve(100);
    fPairY.reserve(100);
    fClusterX.clear();
    fClusterY.clear();

    for (Int_t i = 0; i < nHits; i++)
    {
        calData[i] = (R3BSofMwpcCalData*)(fMwpcCalDataCA->At(i));
        planeId = calData[i]->GetPlane();
        padId = calData[i]->GetPad();
        q = calData[i]->GetQ();
        pair<Int_t, Int_t> hit_pair = make_pair(padId, q);
        if (planeId == 1)
        {
            fPairX.push_back(hit_pair);
            // fQX.push_back(q);
            // fPadX.push_back(padId);
        }
        if (planeId == 3)
        {
            fPairY.push_back(hit_pair);
            // fQY.push_back(q);
            // fPadY.push_back(padId);
        }
    }
    if (fPairX.size() == 0 || fPairY.size() == 0)
    {
        if (calData)
            delete calData;
        return;
    }

    // sorting fPair with increasing pad number;
    sort(fPairX.begin(), fPairX.end());
    sort(fPairY.begin(), fPairY.end());

    fThresholdX = max(0.2 * GetChargeMax(fPairX), 500.);
    fThresholdY = max(0.2 * GetChargeMax(fPairY), 500.);

    while (GetChargeMax(fPairX) > fThresholdX && fPairX.size() > 3)
    {
        fClusterX.push_back(FindCluster(fPairX));
    }
    while (GetChargeMax(fPairY) > fThresholdY && fPairY.size() > 3)
    {
        fClusterY.push_back(FindCluster(fPairY));
    }

    if (fClusterX.size() == 0 || fClusterY.size() == 0)
    {
        if (calData)
            delete calData;
        return;
    }

    vector<double> Xpos;
    for (unsigned int i = 0; i < fClusterX.size(); i++)
    {
        vector<pair<int, int>> hit;
        hit = fClusterX[i];
        double x;
        int qleft = hit[0].second;
        int qmax = hit[1].second;
        int padmax = hit[1].first;
        int qright = hit[2].second;
        if (padmax > 0 && padmax + 1 < Mw3PadsX && qmax > 0 && qleft > 0 && qright > 0)
        {
            x = GetPositionX(qmax, padmax, qleft, qright);
            Xpos.push_back(x);
        }
    }

    vector<double> Ypos;
    for (unsigned int i = 0; i < fClusterY.size(); i++)
    {
        vector<pair<int, int>> hit;
        hit = fClusterY[i];
        double y;
        int qdown = hit[0].second;
        int qmax = hit[1].second;
        int padmax = hit[1].first;
        int qup = hit[2].second;
        if (padmax > 0 && padmax + 1 < Mw3PadsY && qmax > 0 && qdown > 0 && qup > 0)
        {
            y = GetPositionY(qmax, padmax, qdown, qup);
            Ypos.push_back(y);
        }
    }

    if (Xpos.size() == 0 || Ypos.size() == 0)
    {
        if (calData)
            delete calData;
        return;
    }

    if (twX.size() == twY.size())
    {
        TofWallMatching(twX, twY, Xpos, Ypos);
    }
    if (calData)
        delete calData;
    return;
}

/* ---- Matching position with the tofwall ---- */
void R3BSofMwpc3Cal2Hit::TofWallMatching(vector<double> twx,
                                         vector<double> twy,
                                         vector<double> mwpcx,
                                         vector<double> mwpcy)
{
    vector<double> good_posx;
    vector<double> good_posy;
    for (unsigned int i = 0; i < twx.size(); i++)
    {
        double tofx = twx[i];
        double tofy = twy[i];
        for (unsigned int k = 0; k < mwpcx.size(); k++)
        {
            double posx = mwpcx[k];
            if (abs(posx - tofx) < 12.5)
            {
                good_posx.push_back(posx);
            }
        }
        for (unsigned int p = 0; p < mwpcy.size(); p++)
        {
            double posy = mwpcy[p];
            if (abs(posy - tofy) < 10)
            {
                good_posy.push_back(posy);
            }
        }
    }
    if (good_posx.size() == 2 && good_posy.size() == 2)
    {
        for (unsigned int i = 0; i < good_posx.size(); i++)
        {
            AddHitData(good_posx[i], good_posy[i]);
        }
    }

    return;
}

/* ---- Getting a cluster of three pairs (pad,charge) around the maximum charge found ---- */
/* ---- The three points are then removed from the original fPair to find the next cluster ---- */
vector<pair<int, int>> R3BSofMwpc3Cal2Hit::FindCluster(vector<pair<int, int>>& p1)
{
    vector<pair<int, int>> hit;
    hit.clear();
    pair<int, int> couple;

    const auto pelt = max_element(p1.begin(), p1.end(), compare);
    int padmax = pelt->first;
    int qmax = pelt->second;
    int imax = distance(p1.begin(), pelt);

    couple = { p1[imax - 1].first, p1[imax - 1].second };
    // cout << "pad= " << p1[imax-1].first << " / Q= " << p1[imax-1].second << endl;
    hit.push_back(couple);
    couple = { p1[imax].first, p1[imax].second };
    // cout << "pad= " << p1[imax].first << " / Q= " << p1[imax].second << endl;
    hit.push_back(couple);
    couple = { p1[imax + 1].first, p1[imax + 1].second };
    // cout << "pad= " << p1[imax+1].first << " / Q= " << p1[imax+1].second << endl;
    hit.push_back(couple);

    // Removing those three points from fPair
    p1.erase(p1.begin() + imax - 1, p1.begin() + imax);
    p1.erase(p1.begin() + imax - 1, p1.begin() + imax);
    p1.erase(p1.begin() + imax - 1, p1.begin() + imax);

    return hit;
}
/* ---- Getting the maximum charger of the distribution ---- */
double R3BSofMwpc3Cal2Hit::GetChargeMax(vector<pair<int, int>> p1)
{
    const auto comp = max_element(p1.begin(), p1.end(), compare);
    double Qmax = comp->second;
    return Qmax;
}

/* ----   Protected method to obtain the position X ---- */
Double_t R3BSofMwpc3Cal2Hit::GetPositionX(Double_t qmax, Int_t padmax, Double_t qleft, Double_t qright)
{
    Double_t a3 = TMath::Pi() * fwx / (TMath::ACosH(0.5 * (TMath::Sqrt(qmax / qleft) + TMath::Sqrt(qmax / qright))));
    // Double_t a2 = gRandom->Uniform(-fwx / 2,fwx / 2);
    Double_t a2 = (a3 / TMath::Pi()) * TMath::ATanH((TMath::Sqrt(qmax / qleft) - TMath::Sqrt(qmax / qright)) /
                                                    (2 * TMath::SinH(TMath::Pi() * fwx / a3)));

    return (-1. * padmax * fwx + (fSizeX / 2) - (fwx / 2) - a2); // Left is positive and right negative
}

/* ----   Protected method to obtain the position Y ---- */
Double_t R3BSofMwpc3Cal2Hit::GetPositionY(Double_t qmax, Int_t padmax, Double_t qdown, Double_t qup)
{
    Double_t a3 = TMath::Pi() * fwy / (TMath::ACosH(0.5 * (TMath::Sqrt(qmax / qdown) + TMath::Sqrt(qmax / qup))));
    // Double_t a2 = gRandom->Uniform(-fwy / 2, fwy / 2);
    Double_t a2 = (a3 / TMath::Pi()) * TMath::ATanH((TMath::Sqrt(qmax / qdown) - TMath::Sqrt(qmax / qup)) /
                                                    (2 * TMath::SinH(TMath::Pi() * fwy / a3)));

    return (padmax * fwy - (fSizeY / 2) + (fwy / 2) + a2);
}

/* ----   Public method Finish  ---- */
void R3BSofMwpc3Cal2Hit::Finish() {}

/* ----   Public method Reset  ---- */
void R3BSofMwpc3Cal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing Mwpc3HitData Structure";
    if (fMwpcHitDataCA)
        fMwpcHitDataCA->Clear();
}

/* ----   Private method AddHitData  ---- */
R3BSofMwpcHitData* R3BSofMwpc3Cal2Hit::AddHitData(Double_t x, Double_t y)
{
    // It fills the R3BSofMwpcHitData
    TClonesArray& clref = *fMwpcHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofMwpcHitData(x, y);
}

ClassImp(R3BSofMwpc3Cal2Hit)
