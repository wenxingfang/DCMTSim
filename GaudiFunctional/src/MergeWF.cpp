#include <GaudiKernel/MsgStream.h>
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/MergingTransformer.h"
#include <cmath>
#include "WFVec.h"
#include "IonVec.h"
#include <vector>

using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<Gaudi::Algorithm>;

struct MergeWF final : Gaudi::Functional::MergingTransformer<WFVec( const Gaudi::Functional::vector_of_const_<WFVec>& ), BaseClass_t> {

  MergeWF( const std::string& name, ISvcLocator* svcLoc )
      : MergingTransformer( name, svcLoc, {"InputLocations", {}},{"OutputLocation", "/Event/MyConcatenatedIntVector"} ) {}

  WFVec operator()( const Gaudi::Functional::vector_of_const_<WFVec>& input ) const override {
      info() << "input size=" << input.size()<<endmsg;
      for(unsigned int i=0; i<input.size();i++){
          info() << "i="<<i<<",size=" << input.at(i).e_id.size()<< endmsg;
      }
      WFVec wf; 
      /*
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
      */
      info() << "save output with size=" << wf.e_id.size()<< endmsg;
      return wf;
  }
};

DECLARE_COMPONENT(MergeWF)
