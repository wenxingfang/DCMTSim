#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "NN/NNPred.h"

#include <vector>

class DataMaker_v1 : public Gaudi::Functional::Producer<std::vector<float>()> {
public:
    DataMaker_v1(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec_v1"})) {
     std::string bp_file = "/junofs/users/wxfang/MyGit/tmp/check_G4FastSim_20210121/CEPCSW/Digitisers/SimCurrent/src/NN/model.pt";
     char* cstr = new char[bp_file.size() + 1];
     strcpy(cstr, bp_file.c_str());
     m_NNPred  = new NNPred(cstr);
}
    std::vector<float> operator()() const override;
protected:
    NNPred* m_NNPred;
};
