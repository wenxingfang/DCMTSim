#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Consumer.h>
#include <GaudiAlg/ScalarTransformer.h>

#include "ThreeVecEx.h"

#include <vector>


struct DataConsumerVec : public Gaudi::Functional::ScalarTransformer< DataConsumerVec, std::vector<float>(const std::vector<float>&) > {
public:
    DataConsumerVec(const std::string& name, ISvcLocator* svcLoc)
             : ScalarTransformer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"}),
               KeyValue("OutputLocation", {"MyOutVec"}) ) {}
    using ScalarTransformer::operator();
    float operator()(const float&) const;
};
