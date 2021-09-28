#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Consumer.h>

#include "ThreeVecEx.h"

#include <vector>

/*
class DataConsumer : public Gaudi::Functional::Consumer<void(const ThreeVecEx&)> {
public:
    DataConsumer(const std::string& name, ISvcLocator* svcLoc)
             : Consumer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"})) {}

    void operator()(const ThreeVecEx&) const override;
};
*/

class DataConsumer : public Gaudi::Functional::Consumer<void(const std::vector<float>&)> {
public:
    DataConsumer(const std::string& name, ISvcLocator* svcLoc)
             : Consumer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"})) {}

    void operator()(const std::vector<float>&) const override;
};
