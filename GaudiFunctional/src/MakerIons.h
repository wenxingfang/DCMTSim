#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "IonVec.h"

#include <vector>

class MakerIons : public Gaudi::Functional::Producer<IonVec()> {
public:
    MakerIons(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyIonVec"})) {}

    IonVec operator()() const override;
};
