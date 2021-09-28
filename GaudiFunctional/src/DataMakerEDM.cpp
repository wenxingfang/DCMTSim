#include "DataMakerEDM.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataMakerEDM)



edm4hep::SimTrackerHitCollection DataMakerEDM::operator()() const{
    info() << "executing DataMakerEDM" << endmsg;
    edm4hep::SimTrackerHitCollection output_col;
    for(unsigned i=0;i<10;i++){
        edm4hep::SimTrackerHit hit = output_col.create();
        hit.setCellID(i);
    }
    return output_col;
}
