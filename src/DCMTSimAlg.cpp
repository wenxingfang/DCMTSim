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
#include "DCMTSimAlg.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ThreadLocalContext.h"

#include "GaudiKernel/INTupleSvc.h"
#include <atomic>
#include <map>
//#include "NN/NNPred.h"
#include <time.h>

DECLARE_COMPONENT( DCMTSimAlg )

using namespace std;

StatusCode DCMTSimAlg::initialize() {
  info() << ":DCMTSimAlg::initialize " << endmsg;
  /*
  std::string bp_file = "/junofs/users/wxfang/MyGit/tmp/check_G4FastSim_20210121/CEPCSW/Digitisers/SimCurrent/src/NN/model.pt";
  char* cstr = new char[bp_file.size() + 1];
  strcpy(cstr, bp_file.c_str());
  m_NNPred  = new NNPred(cstr);
  */

  for (unsigned i=0; i<m_inputs.value().size(); i++) {
    std::string colName = m_inputs.value().at(i);
    std::string colType = m_type_inputs.value().at(i);
    debug() << "adding input key " << colName<<", with type "<< colType << endmsg;
    if ( colType == "SimTrackerHit" ) {
        m_input_dataHandles[colName] =  new DataHandle<edm4hep::SimTrackerHitCollection>(colName, Gaudi::DataHandle::Reader, this);
    }
    else if ( colType == "TPCHit" ) {
        m_input_dataHandles[colName] =  new DataHandle<edm4hep::TPCHitCollection>(colName, Gaudi::DataHandle::Reader, this);
    }
    else{
        debug() << "Unknown collection type "<< colType << endmsg;
        return StatusCode::FAILURE;
    }
  }

  for (unsigned i=0; i<m_outputs.value().size(); i++) {
    std::string colName = m_outputs.value().at(i);
    std::string colType = m_type_outputs.value().at(i);
    debug() << "adding output key " << colName<<", with type "<< colType << endmsg;
    if ( colType == "TPCHit" ) {
        m_output_dataHandles[colName] =  new DataHandle<edm4hep::TPCHitCollection>(colName, Gaudi::DataHandle::Writer, this);
    }
    else{
        info() << "Unknown collection type "<< colType << endmsg;
        return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode DCMTSimAlg::execute() {
  ++m_total;
  int evt = Gaudi::Hive::currentContext().evt();
  info() << ":DCMTSimAlg::getting inputs... " << evt << endmsg;
  if(m_type.value()==1){
      static TH1D sp_wf = getSingleWF(m_file.value());
      const edm4hep::SimTrackerHitCollection* in_obj = nullptr;
      if ( m_input_dataHandles.find("DCHIonizedEleCollection") != m_input_dataHandles.end() ){
          in_obj = dynamic_cast<DataHandle<edm4hep::SimTrackerHitCollection>*>(m_input_dataHandles["DCHIonizedEleCollection"])->get();
      }
      if ( !in_obj ) {
        fatal() << "Unable to dcast inputHandles object" << endmsg;
        return StatusCode::FAILURE;
      }
      auto output_col = new edm4hep::TPCHitCollection();
      info() <<" input collection size="<< in_obj->size()<< endmsg;
      int ele_size = in_obj->size();
      info() << "simulating single WF, evt=" << evt<<",range from "<<m_range.value().at(0)<<" to "<<m_range.value().at(1) << endmsg;
      for(unsigned int i = int(m_range.value().at(0)*ele_size); i<int(m_range.value().at(1)*ele_size); i++){
          edm4hep::SimTrackerHit SimHit = in_obj->at(i);
          edm4hep::TPCHit tpc_hit = output_col->create();
          tpc_hit.setCellID(SimHit.getCellID());
          float c_time = -1; 
          float c_peak = -1;
          float x = SimHit.getPosition()[0]; 
          float x_scale = 0.5; 
          float x_shift = 0; 
          float y = SimHit.getPosition()[1]; 
          float y_scale = 0.5; 
          float y_shift = 0; 
          float t_scale = 99.0; 
          float t_shift = 166.4; 
          float p_scale = 1e-2; 
          float p_shift = 0; 
          //m_NNPred->get( x, x_scale, x_shift, y, y_scale, y_shift, t_scale, t_shift, c_time, p_scale, p_shift, c_peak);
          c_time = 100; //dummy
          c_peak = 1e-2;//dummy
          //info()<<"c_time="<<c_time<<",c_peak="<<c_peak<<endmsg;
          tpc_hit.setTime(c_time);
          tpc_hit.setCharge(c_peak);
          std::vector<float> ttmp_current_time(sp_wf.GetNbinsX(), 0);
          std::vector<float> ttmp_current_value(sp_wf.GetNbinsX(), 0);
          transform(sp_wf, m_sp_peak_value.value().at(0), m_sp_peak_value.value().at(1), c_time, c_peak, ttmp_current_value, ttmp_current_time);
          for(unsigned int j=0; j<ttmp_current_value.size();j++){
              int scaled_value = int(1e6*ttmp_current_value.at(j));
              tpc_hit.addToRawDataWords(scaled_value);
          }
      }
      info() << "start save outputs, evt="<<evt<<",size="<<output_col->size() << endmsg;
      std::map<std::string, DataObjectHandleBase*>::iterator iter;
      iter = m_output_dataHandles.begin();
      bool out_saved = false;
      while(iter != m_output_dataHandles.end()){
          std::string out_name = iter->first;
          if (out_name.find("SingleWF") != out_name.npos) {
              auto handle = dynamic_cast<DataHandle<edm4hep::TPCHitCollection>*>( iter->second );
              handle->put(output_col);
              out_saved = true;
              break;
          }
          iter++;
      }
      if(out_saved==false){
          info() << "Error, can't save output"<< endmsg;
          return StatusCode::FAILURE;
      }
      info() << "saved outputs for evt="<<evt<< endmsg;
  }
  else if(m_type.value()==2){
      auto output_col = new edm4hep::TPCHitCollection();
      std::map<unsigned long long, edm4hep::TPCHit> map_id_hit;
      for(std::map<std::string, DataObjectHandleBase*>::iterator iter=m_input_dataHandles.begin(); iter != m_input_dataHandles.end(); iter++){
        std::string in_name = iter->first;
        if (in_name.find("SingleWF") == in_name.npos) continue;
        const edm4hep::TPCHitCollection* in_col = nullptr;
        in_col = dynamic_cast<DataHandle<edm4hep::TPCHitCollection>*>(iter->second)->get();
        if ( !in_col ) {
          fatal() << "Unable to dcast inputHandles object" << endmsg;
          return StatusCode::FAILURE;
        }
        info() << "Got data with size="<< in_col->size()<< endmsg;
        for(unsigned int i=0; i<in_col->size(); i++){
            edm4hep::TPCHit in_tpc_hit = in_col->at(i);
            unsigned long long tmp_id = in_tpc_hit.getCellID();
            edm4hep::TPCHit out_tpc_hit (tmp_id,int(0),0.0, 0.0);
            if(map_id_hit.find(tmp_id) == map_id_hit.end()){
                map_id_hit[tmp_id] = in_tpc_hit;
            }
            else{
                for(unsigned int j=0;j<map_id_hit[tmp_id].rawDataWords_size();j++){
                    out_tpc_hit.addToRawDataWords( map_id_hit[tmp_id].getRawDataWords(j) + in_tpc_hit.getRawDataWords(j) );
                    map_id_hit[tmp_id] = out_tpc_hit;
                }
            }
        }
      }
      for(std::map<unsigned long long, edm4hep::TPCHit>::iterator iter=map_id_hit.begin(); iter !=map_id_hit.end(); iter++){
          edm4hep::TPCHit tpc_hit_save = output_col->create();
          tpc_hit_save = iter->second;    
      }
      std::map<std::string, DataObjectHandleBase*>::iterator iter;
      iter = m_output_dataHandles.begin();
      bool out_saved = false;
      while(iter != m_output_dataHandles.end()){
          std::string out_name = iter->first;
          if (out_name.find("MergedWF") != out_name.npos) {
              auto handle = dynamic_cast<DataHandle<edm4hep::TPCHitCollection>*>( iter->second );
              handle->put(output_col);
              out_saved = true;
              break;
          }
          iter++;
      }
      if(out_saved==false){
          info() << "Error, can't save output"<< endmsg;
          return StatusCode::FAILURE;
      }
  }
  else{
      fatal() << "Wrong type !!" << endmsg;
      return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode DCMTSimAlg::finalize() {
  info() << name() << ":DCMTSimAlg::finalize - total events: " << m_total << endmsg;
  return StatusCode::SUCCESS;
}

const std::vector<std::string> DCMTSimAlg::get_inputs() { return m_inputs; }

const std::vector<std::string> DCMTSimAlg::get_outputs() { return m_outputs; }

TH1D DCMTSimAlg::getSingleWF(const std::string& file) {
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
        //info() <<"bin="<<tmp_x_axis->FindBin(tmp_vec_time[i])<<",value="<<-1*tmp_vec_value[i]<<endmsg;
    }
    return tmp_h; 
}
TH1D DCMTSimAlg::transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new){
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

void DCMTSimAlg::transform(const TH1D& h_template, double time0, double value0, double time_new, double value_new, std::vector<float>& out_value, std::vector<float>& out_time){
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
