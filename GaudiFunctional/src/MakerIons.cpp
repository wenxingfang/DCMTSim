#include "MakerIons.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(MakerIons)

IonVec MakerIons::operator()() const{
    info() << "executing MakerIons" << endmsg;
    IonVec ions;
    ions.data = 1;
    for(unsigned i=0; i<10; i++){
        ions.x.push_back(0.1*i);
        ions.y.push_back(1.0*i);
        ions.cell_id.push_back(100*i);
    }
    info() << "saved Ions with size= "<<ions.x.size()<<" into "<< outputLocation() << endmsg;
    return ions;
}

