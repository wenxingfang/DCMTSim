#include "DataMaker.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataMaker)

/*
ThreeVecEx DataMaker::operator()() const{
    info() << "executing DataMaker" << endmsg;
    return ThreeVecEx{1, 2, 3};
}
*/

std::vector<float> DataMaker::operator()() const{
    info() << "executing DataMaker" << endmsg;
    sleep(8);
    std::vector<float> tmp;
    //tmp.push_back(10.0);
    //tmp.push_back(9.0);
    //tmp.push_back(8.0);
    //tmp.push_back(7.0);
    //tmp.push_back(6.0);
    //tmp.push_back(5.0);
    //tmp.push_back(4.0);
    tmp.push_back(3.0);
    tmp.push_back(2.0);
    tmp.push_back(1.0);
    info() << "end DataMaker" << endmsg;
    return tmp;
}
