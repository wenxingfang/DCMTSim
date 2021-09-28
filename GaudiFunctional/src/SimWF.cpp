#include <GaudiKernel/MsgStream.h>
#include "GaudiAlg/Transformer.h"
#include <cmath>
#include "WFVec.h"
#include "IonVec.h"
#include <vector>

using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<Gaudi::Algorithm>;
struct SimWF final : Gaudi::Functional::Transformer<WFVec( const IonVec& ), BaseClass_t> {

  SimWF( const std::string& name, ISvcLocator* svcLoc )
      : Transformer( name, svcLoc, KeyValue( "InputLocation", "/Event/MyInt" ),
                     KeyValue( "OutputLocation", "/Event/MyFloat" ) ) {}
  WFVec operator()( const IonVec& input ) const override {
      int ele_size = input.x.size(); 
      info() << "input size=" << ele_size<<",lower="<<int(m_range.value().at(0)*ele_size)<<",upper="<<int(m_range.value().at(1)*ele_size)<<endmsg;
      WFVec wf; 
      for(unsigned int i = int(m_range.value().at(0)*ele_size); i<int(m_range.value().at(1)*ele_size); i++){ 
          float peak_time  = input.x.at(i)*100 ;//dummy
          float peak_value = input.y.at(i)*1e-2;//dummy
          int cell_id = input.cell_id.at(i);
          int n_slice = 100;
          std::vector<float> charges;
          for(unsigned j=1; j<n_slice; j++){
              charges.push_back(j*1e-2);//dummy
          }
          wf.e_id.push_back(i);
          wf.cell_id.push_back(cell_id);
          wf.peak_time.push_back(peak_time);
          wf.peak_value.push_back(peak_value);
          wf.charges.push_back(charges);
      }
      info() << "save output with size=" << wf.e_id.size()<< endmsg;
      return wf;
  }
  Gaudi::Property<std::vector<float>> m_range{this, "Range", {}, "range"};
};

DECLARE_COMPONENT(SimWF)
