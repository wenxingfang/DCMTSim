//#pragma once

#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Transformer.h>

#include <vector>
#include "edm4hep/SimTrackerHitCollection.h"
#include "k4FWCore/DataHandle.h"

using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<Gaudi::Algorithm>;

struct DataConsumerEDM : public Gaudi::Functional::Transformer< std::vector<float>(const edm4hep::SimTrackerHitCollection&), BaseClass_t > {
public:
    DataConsumerEDM(const std::string& name, ISvcLocator* svcLoc)
             : Transformer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"}),
               KeyValue("OutputLocation", {"MyOutVec"}) ) {}
    std::vector<float> operator()(const edm4hep::SimTrackerHitCollection& input) const override{
        std::vector<float> vect; 
        info() << "input size=" << input.size()<<endmsg;
        for(unsigned int i=0; i<input.size();i++){
            edm4hep::SimTrackerHit SimHit = input.at(i); 
            info() << "cell id=" << SimHit.getCellID()<<endmsg;
            vect.push_back(SimHit.getCellID());
        }
        info() << "saved size=" << vect.size()<<endmsg;
        return vect;   
    }
};

DECLARE_COMPONENT(DataConsumerEDM)
