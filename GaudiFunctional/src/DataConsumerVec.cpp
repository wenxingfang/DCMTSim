#include "DataConsumerVec.h"
#include <GaudiKernel/MsgStream.h>
#include <cmath>
DECLARE_COMPONENT(DataConsumerVec)

float DataConsumerVec::operator()(const float &input) const{
    float out = 0;
    for(unsigned i=1; i<int(100000*input); i++){
        out += sqrt(i);
    }
    sleep(input);
    info() << "isReEntrant=" << isReEntrant()<<endmsg;
    info() << "executing DataConsumerVec: input=" << input<<",out="<<out << endmsg;
    return out;
}
