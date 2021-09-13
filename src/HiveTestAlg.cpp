/***********************************************************************************\
* (c) Copyright 1998-2019 CERN for the benefit of the LHCb and ATLAS collaborations *
*                                                                                   *
* This software is distributed under the terms of the Apache version 2 licence,     *
* copied verbatim in the file "LICENSE".                                            *
*                                                                                   *
* In applying this licence, CERN does not waive the privileges and immunities       *
* granted to it by virtue of its status as an Intergovernmental Organization        *
* or submit itself to any jurisdiction.                                             *
\***********************************************************************************/
#include "HiveTestAlg.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ThreadLocalContext.h"

#include "GaudiKernel/INTupleSvc.h"
#include <atomic>
#include <map>
//#include "NN/NNPred.h"
#include <time.h>

DECLARE_COMPONENT( HiveTestAlg )

using namespace std;
//////////////
class EleObject : public DataObject {
  int                m_data;
  std::vector<int>   m_cell_id;
  std::vector<float> m_x;
  std::vector<float> m_y;
  static atomic<int> c_instances;
  static atomic<int> d_instances;


public:
  EleObject( int d ) : m_data( d ) { c_instances++; }
  EleObject( const EleObject& o ) : DataObject(), m_data( o.m_data ) { c_instances++; }
  ~EleObject() { d_instances++; }
  int  getData() { return m_data; }
  bool put_x(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_x.push_back(a.at(i));
      }
      return true; 
  }
  bool put_y(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_y.push_back(a.at(i));
      }
      return true; 
  }
  bool put_cell_id(const std::vector<int>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_cell_id.push_back(a.at(i));
      }
      return true; 
  }
  std::vector<float>  get_x() { return m_x; }
  std::vector<float>  get_y() { return m_y; }
  std::vector<int>  get_cell_id() { return m_cell_id; }
  static void dump() { cout << "EleObject (C/D): " << c_instances << "/" << d_instances << endl; }
};

atomic<int> EleObject::c_instances;
atomic<int> EleObject::d_instances;
//////////////

class WFObject : public DataObject {
  int                m_data;
  std::vector<int>   m_cell_id;
  std::vector<float> m_x;
  std::vector<float> m_y;
  std::vector<float> m_peak_time;
  std::vector<float> m_peak_value;
  std::vector<std::vector<float>> m_current_time;
  std::vector<std::vector<float>> m_current_value;
  std::vector<TH1D> m_h_current;
  static atomic<int> c_instances;
  static atomic<int> d_instances;

public:
  WFObject( int d ) : m_data( d ) { c_instances++; }
  WFObject( const WFObject& o ) : DataObject(), m_data( o.m_data ) { c_instances++; }
  ~WFObject() { d_instances++; }
  int  getData() { return m_data; }
  bool put_x(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_x.push_back(a.at(i));
      }
      return true; 
  }
  bool put_y(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_y.push_back(a.at(i));
      }
      return true; 
  }
  bool put_cell_id(const std::vector<int>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_cell_id.push_back(a.at(i));
      }
      return true; 
  }
  bool put_peak_time(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_peak_time.push_back(a.at(i));
      }
      return true; 
  }
  bool put_peak_value(const std::vector<float>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_peak_value.push_back(a.at(i));
      }
      return true; 
  }
  bool put_h_current(const std::vector<TH1D>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_h_current.push_back(a.at(i));
      }
      return true; 
  }
  bool put_current_time(const std::vector<std::vector<float>>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_current_time.push_back(a.at(i));
      }
      return true; 
  }
  bool put_current_value(const std::vector<std::vector<float>>& a) {
      for(unsigned int i=0; i<a.size(); i++){
          m_current_value.push_back(a.at(i));
      }
      return true; 
  }
  std::vector<float>  get_x() { return m_x; }
  std::vector<float>  get_y() { return m_y; }
  std::vector<int>  get_cell_id() { return m_cell_id; }
  std::vector<float>  get_peak_time() { return m_peak_time; }
  std::vector<float>  get_peak_value() { return m_peak_value; }
  std::vector<TH1D>   get_h_current() { return m_h_current; }
  std::vector<std::vector<float>>  get_current_time() { return m_current_time; }
  std::vector<std::vector<float>>  get_current_value() { return m_current_value; }
  static void dump() { cout << "WFObject (C/D): " << c_instances << "/" << d_instances << endl; }
};

atomic<int> WFObject::c_instances;
atomic<int> WFObject::d_instances;

//////////////

StatusCode HiveTestAlg::initialize() {
  info() << ":HiveTestAlg::initialize " << endmsg;
  /*
  std::string bp_file = "/junofs/users/wxfang/MyGit/tmp/check_G4FastSim_20210121/CEPCSW/Digitisers/SimCurrent/src/NN/model.pt";
  char* cstr = new char[bp_file.size() + 1];
  strcpy(cstr, bp_file.c_str());
  m_NNPred  = new NNPred(cstr);
  */

  NTuplePtr nt( ntupleSvc(), "MyTuples/GF_evt" );
  if ( nt ) m_tuple = nt;
  else {
      m_tuple = ntupleSvc()->book( "MyTuples/GF_evt", CLID_ColumnWiseTuple, "GF_evt" );
      m_tuple->addItem( "N_current"  , m_ncurrent  , 0, m_max_current.value() ).ignore();
      m_tuple->addItem( "m_current"   , m_ncurrent, m_current    ).ignore();
      m_tuple->addItem( "m_time"      , m_ncurrent, m_time    ).ignore();
  }

  int i = 0;
  for ( auto k : m_inputs ) {
    debug() << "adding input key " << k << endmsg;
    m_inputHandles.push_back( new DataObjectHandle<DataObject>( k, Gaudi::DataHandle::Reader, this ) );
    declareProperty( "dummy_in_" + std::to_string( i ), *( m_inputHandles.back() ) );
    i++;
  }

  i = 0;
  for ( auto k : m_outputs ) {
    debug() << "adding output key " << k << endmsg;
    m_outputHandles.push_back( new DataObjectHandle<DataObject>( k, Gaudi::DataHandle::Writer, this ) );
    declareProperty( "dummy_out_" + std::to_string( i ), *( m_outputHandles.back() ) );
    i++;
  }

  return StatusCode::SUCCESS;
}

StatusCode HiveTestAlg::execute() {
  ++m_total;
  int evt = Gaudi::Hive::currentContext().evt();
  info() << ":HiveTestAlg::getting inputs... " << evt << endmsg;
  if(m_type.value()==0){
      for ( auto& handle : m_inputHandles ) {
        auto obj = dynamic_cast<EleObject*>( handle->get() );
        if ( !obj ) {
          fatal() << "Unable to dcast inputHandles object" << endmsg;
          return StatusCode::FAILURE;
        }
        info() << "Got data with value " << obj->getData()<<",vet size="<< obj->get_x().size()<< endmsg;
      }
      info() << ":HiveTestAlg::registering outputs... " << evt << endmsg;
      for ( auto& outputHandle : m_outputHandles ) { 
          std::vector<float> tmp_x;
          for(unsigned int i=0;i<100;i++){
              tmp_x.push_back(i+0.5);
          }
          std::vector<float> tmp_y;
          for(unsigned int i=0;i<100;i++){
              tmp_y.push_back(i+0.5);
          }
          std::vector<int> tmp_cell_id;
          for(unsigned int i=0;i<100;i++){
              tmp_cell_id.push_back(i);
          }
          outputHandle->put( std::make_unique<EleObject>( evt ) ); 
          auto obj = dynamic_cast<EleObject*>( outputHandle->get() );
          obj->put_x(tmp_x);
          obj->put_y(tmp_y);
          obj->put_cell_id(tmp_cell_id);
      }
  }
  else if(m_type.value()==1){
      static TH1D sp_wf = getSingleWF(m_file.value());
      EleObject* in_obj = nullptr;
      for ( auto& handle : m_inputHandles ) {
        in_obj = dynamic_cast<EleObject*>( handle->get() );
      }
      if ( !in_obj ) {
        fatal() << "Unable to dcast inputHandles object" << endmsg;
        return StatusCode::FAILURE;
      }
      info() << "Got data with value " << in_obj->getData()<<",x size="<< in_obj->get_x().size()<< endmsg;
      int ele_size = in_obj->get_x().size();
      info() << ":HiveTestAlg::registering outputs... " << evt<<",range from "<<m_range.value().at(0)<<" to "<<m_range.value().at(1) << endmsg;
      for ( auto& outputHandle : m_outputHandles ) { 
          std::vector<float> tmp_x;
          std::vector<float> tmp_y;
          std::vector<int> tmp_cell_id;
          std::vector<float> tmp_peak_time;
          std::vector<float> tmp_peak_value;
          std::vector<std::vector<float>> tmp_current_time;
          std::vector<std::vector<float>> tmp_current_value;
          for(unsigned int i = int(m_range.value().at(0)*ele_size); i<int(m_range.value().at(1)*ele_size); i++){
              std::vector<float> ttmp_current_time(sp_wf.GetNbinsX(), 0);
              std::vector<float> ttmp_current_value(sp_wf.GetNbinsX(), 0);
              tmp_x.push_back( in_obj->get_x().at(i));
              tmp_y.push_back( in_obj->get_y().at(i));
              tmp_cell_id.push_back( in_obj->get_cell_id().at(i));
              ///////////////////////////
              float c_time = -1; 
              float c_peak = -1;
              float x = in_obj->get_x().at(i); 
              float x_scale = 0.5; 
              float x_shift = 0; 
              float y = in_obj->get_y().at(i); 
              float y_scale = 0.5; 
              float y_shift = 0; 
              float t_scale = 99.0; 
              float t_shift = 166.4; 
              float p_scale = 1e-2; 
              float p_shift = 0; 
              //m_NNPred->get( x, x_scale, x_shift, y, y_scale, y_shift, t_scale, t_shift, c_time, p_scale, p_shift, c_peak);
              c_time = 100 ;// dummy
              c_peak = 1e-2;//dummy
              tmp_peak_time.push_back( c_time );
              tmp_peak_value.push_back( c_peak );
              transform(sp_wf, m_sp_peak_value.value().at(0), m_sp_peak_value.value().at(1), c_time, c_peak, ttmp_current_value, ttmp_current_time);
              tmp_current_time .push_back(ttmp_current_time);
              tmp_current_value.push_back(ttmp_current_value);
          }
          outputHandle->put( std::make_unique<WFObject>( evt ) ); 
          auto obj = dynamic_cast<WFObject*>( outputHandle->get() );
          obj->put_x(tmp_x);
          obj->put_y(tmp_y);
          obj->put_cell_id(tmp_cell_id);
          obj->put_peak_time(tmp_peak_time);
          obj->put_peak_value(tmp_peak_value);
          obj->put_current_time(tmp_current_time);
          obj->put_current_value(tmp_current_value);
      }
  }
  else if(m_type.value()==2){
      std::vector<float> out_current_time;
      std::map<int, std::vector<float>> map_id_current;
      std::map<int, TH1D> map_id_h_current;
      for ( auto& handle : m_inputHandles ) {
        WFObject* in_obj = nullptr;
        in_obj = dynamic_cast<WFObject*>( handle->get() );
        if ( !in_obj ) {
          fatal() << "Unable to dcast inputHandles object" << endmsg;
          return StatusCode::FAILURE;
        }
        info() << "Got data with value " << in_obj->getData()<<",peak time size="<< in_obj->get_peak_time().size()<< endmsg;
        for(unsigned int i=0; i<in_obj->get_peak_time().size(); i++){
            int tmp_id = in_obj->get_cell_id().at(i);
            //////////////
            if(map_id_current.find(tmp_id) == map_id_current.end()){
                map_id_current[tmp_id] = in_obj->get_current_value().at(i);
            }
            else{
                for(unsigned int j=0;j<map_id_current[tmp_id].size();j++){
                    map_id_current[tmp_id][j] = map_id_current[tmp_id][j] + in_obj->get_current_value().at(i).at(j);
                }
            }
            ////////////
            /*
            if(map_id_h_current.find(tmp_id) == map_id_h_current.end()){
                map_id_h_current[tmp_id] = in_obj->get_h_current().at(i);
            }
            else{
                map_id_h_current[tmp_id].Add(&(in_obj->get_h_current().at(i)));
                }
            */
            ////////////
        }
      }
  }
      /* 
      m_ncurrent = 0;//rise thread error
      for(int ibin=1; ibin<= 10; ibin++){
          if( m_ncurrent >= m_max_current) continue;
          m_time   [m_ncurrent] = ibin+0.1;
          m_current[m_ncurrent] = ibin+0.5; 
          m_ncurrent ++ ;
      }
      StatusCode status1 = m_tuple->write();
      if ( status1.isFailure() ) {
          error() << "    Cannot fill N-tuple:" << long( m_tuple ) << endmsg;
          return StatusCode::FAILURE;
      }
      */
  else{
      fatal() << "Wrong type !!" << endmsg;
      return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode HiveTestAlg::finalize() {
  info() << name() << ":HiveTestAlg::finalize - total events: " << m_total << endmsg;
  EleObject::dump();
  return StatusCode::SUCCESS;
}

const std::vector<std::string> HiveTestAlg::get_inputs() { return m_inputs; }

const std::vector<std::string> HiveTestAlg::get_outputs() { return m_outputs; }

TH1D HiveTestAlg::getSingleWF(const std::string& file) {
    info() <<"start read "<<file<<endmsg;
    sleep(m_sleep.value());
    ifstream inf;
    inf.open(file.c_str());
    std::string sline;//每一行
    std::string s1,s2;
    std::vector<double> tmp_vec_time;
    std::vector<double> tmp_vec_value;
    while(getline(inf,sline))
    {
       istringstream sin(sline);
       sin>>s1>>s2;
       double tmp_time  = std::stod(s1);
       double tmp_value = std::stod(s2);
       tmp_vec_time .push_back(tmp_time );
       tmp_vec_value.push_back(tmp_value);
    }
    TH1D tmp_h("single_current" , "", int((tmp_vec_time.back()-tmp_vec_time.at(0))/(tmp_vec_time.at(1)-tmp_vec_time.at(0))), tmp_vec_time.at(0), tmp_vec_time.back());
    TAxis* tmp_x_axis = tmp_h.GetXaxis();
    for(unsigned int i=0; i<tmp_vec_time.size();i++){
        tmp_h.SetBinContent(tmp_x_axis->FindBin(tmp_vec_time[i]), -1*tmp_vec_value[i]);
        info() <<"bin="<<tmp_x_axis->FindBin(tmp_vec_time[i])<<",value="<<-1*tmp_vec_value[i]<<endmsg;
    }
    return tmp_h; 
}

TH1D HiveTestAlg::transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new){
        std::string tmp_name = std::to_string(time_new)+"_"+std::to_string(value_new);
        TH1D h_new(tmp_name.c_str(),"",h_template.GetNbinsX(), h_template.GetBinLowEdge(1), h_template.GetBinLowEdge( h_template.GetNbinsX() ) + h_template.GetBinWidth(1) );
        h_new.Scale(0);
        TAxis* h_x_axis = h_new.GetXaxis();
        double tmp_shift = time_new - time0;
        double tmp_scale = value_new/value0;
        for(unsigned int i=1; i<h_template.GetNbinsX()+1;i++){
            double new_x = h_template.GetBinCenter(i)+tmp_shift;
            if (new_x < h_new.GetBinLowEdge(1)){continue;}
            if (new_x > ( h_new.GetBinLowEdge( h_new.GetNbinsX() ) + h_new.GetBinWidth(1) )){continue;}
            h_new.SetBinContent(h_x_axis->FindBin(new_x), h_template.GetBinContent(i)*tmp_scale);
        }
        return h_new;
}

void HiveTestAlg::transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new, std::vector<float>& out_value, std::vector<float>& out_time){
        const TAxis* tmp_x_axis = h_template.GetXaxis();
        double tmp_shift = time_new - time0;
        double tmp_scale = value_new/value0;
        for(unsigned int i=1; i<h_template.GetNbinsX()+1;i++){
            double new_x = h_template.GetBinCenter(i)+tmp_shift;
            if (new_x < h_template.GetBinLowEdge(1)){continue;}
            if (new_x > ( h_template.GetBinLowEdge( h_template.GetNbinsX() ) + h_template.GetBinWidth(1) )){continue;}
            out_value[tmp_x_axis->FindBin(new_x)-1] = h_template.GetBinContent(i)*tmp_scale;
            out_time[i-1] = h_template.GetBinCenter(i);
        }
}
