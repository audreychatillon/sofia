// ------------------------------------------------
// -----     R3BSofAtContFact source file     -----
// ------------------------------------------------

#ifndef R3BSofAtContFact_H
#define R3BSofAtContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofAtContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofAtContFact();
    ~R3BSofAtContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofAtContFact, 0) // Factory for all R3BSofAt parameter containers
};

#endif /* R3BSofAtContFact_H */
