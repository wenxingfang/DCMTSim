#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "ThreeVecEx.h"

#include <vector>
#include "edm4hep/SimTrackerHitCollection.h"
#include "k4FWCore/DataHandle.h"

class DataMakerEDM : public Gaudi::Functional::Producer<edm4hep::SimTrackerHitCollection()> {
//class DataMakerEDM : public Gaudi::Functional::Producer<edm4hep::SimTrackerHitCollection*()> {
public:
    DataMakerEDM(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec"} )) {}

    edm4hep::SimTrackerHitCollection operator()() const override;
//    edm4hep::SimTrackerHitCollection* operator()() const override;
};
