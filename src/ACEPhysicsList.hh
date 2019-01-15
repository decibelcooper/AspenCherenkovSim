#include "QGSP_BERT.hh"

class ACEPhysicsList : public QGSP_BERT {
   public:
    ACEPhysicsList();
    ~ACEPhysicsList();

    void ConstructParticle();
    void ConstructProcess();
    void ConstructOp();
};
