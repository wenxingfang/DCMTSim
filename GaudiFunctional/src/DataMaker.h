#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "ThreeVecEx.h"

#include <vector>
/*
class DataMaker : public Gaudi::Functional::Producer<ThreeVecEx()> {
public:
    DataMaker(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec"})) {}

    ThreeVecEx operator()() const override;
};
*/

class DataMaker : public Gaudi::Functional::Producer<std::vector<float>()> {
public:
    DataMaker(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec"})) {}

    std::vector<float> operator()() const override;
};
