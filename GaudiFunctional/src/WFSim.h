#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Consumer.h>
#include <GaudiAlg/ScalarTransformer.h>
#include <GaudiAlg/Transformer.h>

#include "WFVec.h"
#include "IonVec.h"

#include <vector>

/*
struct WFSim : public Gaudi::Functional::Transformer< WFSim, WFVec(const IonVec&) > {
public:
    WFSim(const std::string& name, ISvcLocator* svcLoc)
             : Transformer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"}),
               KeyValue("OutputLocation", {"MyOutVec"}) ) {}
    using Transformer::operator();
    WFVec operator()(const IonVec&) const;
    Gaudi::Property<std::vector<float>> m_range{this, "Range", {}, "range"};
};
*/
