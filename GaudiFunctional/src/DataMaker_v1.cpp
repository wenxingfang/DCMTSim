#include "DataMaker_v1.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataMaker_v1)


std::vector<float> DataMaker_v1::operator()() const{
    info() << "executing DataMaker_v1" << endmsg;

    float c_time = -1;
    float c_peak = -1;
    float x = 0.2;
    float x_scale = 0.5;
    float x_shift = 0;
    float y = 0.2;
    float y_scale = 0.5;
    float y_shift = 0;
    float t_scale = 99.0;
    float t_shift = 166.4;
    float p_scale = 1e-2;
    float p_shift = 0;
    m_NNPred->get( x, x_scale, x_shift, y, y_scale, y_shift, t_scale, t_shift, c_time, p_scale, p_shift, c_peak);
    std::cout<<"c_time="<<c_time<<",c_peak="<<c_peak<<std::endl;
    std::vector<float> tmp;
    //tmp.push_back(4.0);
    //tmp.push_back(5.0);
    //tmp.push_back(6.0);
    info() << "end DataMaker_v1" << endmsg;
    return tmp;
}
