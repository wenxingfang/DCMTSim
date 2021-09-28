#include "DataConsumer.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataConsumer)

/*
void DataConsumer::operator()(const ThreeVecEx &input) const{
    info() << "executing DataConsumer: " << input << endmsg;
}
*/

void DataConsumer::operator()(const std::vector<float> &input) const{
    info() << "executing DataConsumer:size=" << input.size() << endmsg;
    for(unsigned i=0;i<input.size();i++)  info() << "i="<<i<<",val=" << input.at(i) << endmsg;
}
