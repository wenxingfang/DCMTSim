//
//  DCMTSimAlg.h
//
//

#include "GaudiAlg/GaudiAlgorithm.h"
#include <Gaudi/Property.h>
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/INTupleSvc.h"
#include <TROOT.h>
#include <TH1D.h>
#include <TAxis.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TPCHitCollection.h"
#include "k4FWCore/DataHandle.h"


class NNPred;
class GAUDI_API DCMTSimAlg : public GaudiAlgorithm {
public:
  /**
   ** Constructor(s)
   **/
  using GaudiAlgorithm::GaudiAlgorithm;

  /*****************************
   ** Public Function Members **
   *****************************/
  bool isClonable() const override { return true; }//FIXME, should be used or not, seems no effect ?

  StatusCode initialize() override;
  StatusCode execute() override;
  StatusCode finalize() override;

  // BH: concurrency additions
  virtual const std::vector<std::string> get_inputs();
  virtual const std::vector<std::string> get_outputs();
  TH1D getSingleWF(const std::string& file);
  TH1D transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new);
  void transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new, std::vector<float>& out_value, std::vector<float>& out_time);

private:
  /**************************
   ** Private Data Members **
   **************************/

  /**
   ** The total events seen.
   **/
  int m_total = 0;

  //the map of collection name to its corresponding DataHandle
  std::map<std::string, DataObjectHandleBase*> m_input_dataHandles;
  std::map<std::string, DataObjectHandleBase*> m_output_dataHandles;
  Gaudi::Property<std::vector<std::string>> m_inputs{this, "Input", {}, "List of required inputs"};
  Gaudi::Property<std::vector<std::string>> m_outputs{this, "Output", {}, "List of provided outputs"};
  Gaudi::Property<std::vector<std::string>> m_type_inputs{this, "Input_type", {}, "List of required inputs type"};
  Gaudi::Property<std::vector<std::string>> m_type_outputs{this, "Output_type", {}, "List of required outputs type"};
  Gaudi::Property<int> m_type{this, "Type", -1, "type of algo"};
  Gaudi::Property<std::vector<float>> m_range{this, "Range", {}, "range"};//percentage
  Gaudi::Property<int> m_max_current{this, "max_current", 5000, ""};
  Gaudi::Property<std::string> m_file{this, "sp_file", "", "sp file"};
  Gaudi::Property<std::vector<float>> m_sp_peak_value{this, "sp_peak_value", {}, "sp_peak_value"};
  Gaudi::Property<int> m_sleep{this, "sleep", 0, ""};

  NNPred* m_NNPred;
  NTuple::Tuple* m_tuple = nullptr ;
};
