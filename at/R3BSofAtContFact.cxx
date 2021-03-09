// ----------------------------------------------------- //
//                                                       //
//  Factory for the parameter containers in libR3BSofAt  //
//                                                       //
// ----------------------------------------------------- //

#include "R3BSofAtContFact.h"
#include "R3BSofAtCalPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofAtContFact gR3BSofAtContFact;

R3BSofAtContFact::R3BSofAtContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofAtContFact";
    fTitle = "Factory for parameter containers in libR3BSofAt";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofAtContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the STS library.

    FairContainer* p1 = new FairContainer("atCalPar",  "ACtive Target Cal Parameters", "AtCalParContext");
    p1->addContext("AtCalParContext");

    containers->Add(p1);
}

FairParSet* R3BSofAtContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofAtContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "atCalPar") == 0)
    {
        p = new R3BSofAtCalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

void R3BSofAtContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the At
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofAtParRootFileIo* p=new R3BSofAtParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofAtParAsciiFileIo* p=new R3BSofAtParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofAtContFact)
