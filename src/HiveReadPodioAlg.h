//
//  HiveReadPodioAlg.h
//
//
//  Created by Benedikt Hegner on 7/21/12.
//  Copyright (c) 2012 __CERN__. All rights reserved.
//
#include "GaudiKernel/DataSvc.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include <Gaudi/Property.h>
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/IConversionSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IHiveWhiteBoard.h"
#include "GaudiKernel/IDataProviderSvc.h"

#include <TROOT.h>
#include <TH1D.h>
#include <TAxis.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "k4FWCore/DataWrapper.h"
#include "k4FWCore/DataHandle.h"
// PODIO
#include "podio/CollectionBase.h"
#include "podio/CollectionIDTable.h"
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include <utility>
#include "edm4hep/SimTrackerHitCollection.h"

class IIncidentSvc;
class NNPred;
class GAUDI_API HiveReadPodioAlg : public GaudiAlgorithm {
public:
  /**
   ** Constructor(s)
   **/
  using GaudiAlgorithm::GaudiAlgorithm;

  /*****************************
   ** Public Function Members **
   *****************************/
  bool isClonable() const override { return true; }//FIXME, be careful for cardinality number, shoud be 1?

  StatusCode initialize() override;
  StatusCode execute() override;
  StatusCode finalize() override;

  StatusCode readCollection(const std::string& collectionName, int collectionID);
  virtual podio::CollectionIDTable* getCollectionIDs() { return m_collectionIDs; }
  void setCollectionIDs(podio::CollectionIDTable* collectionIds);
  /// Resets caches of reader and event store, increases event counter
  void endOfRead();

  // BH: concurrency additions
  virtual const std::vector<std::string> get_inputs();
  virtual const std::vector<std::string> get_outputs();

private:
  /**************************
   ** Private Data Members **
   **************************/

  /**
   ** The total events seen.
   **/
  int m_total = 0;

  /// PODIO reader for ROOT files
  podio::ROOTReader m_reader;
  /// PODIO EventStore, used to initialise collections
  podio::EventStore m_provider;
  SmartIF<IConversionSvc> m_cnvSvc;
  DataSvc* m_DataSvc;
  /// Number of events in the file / to process
  int m_eventMax{-1};
  /// Counter of the event number
  int m_eventNum{0};


  podio::CollectionIDTable* m_collectionIDs;
  std::vector<std::pair<std::string, podio::CollectionBase*>> m_readCollections;
  
  /// ROOT file name the input is read from. Set by option filename
  std::vector<std::string> m_filenames;
  Gaudi::Property<std::vector<std::string>> m_input_files{this, "InputFiles", {}, "List of input root files"};
  Gaudi::Property<std::vector<std::string>> m_outputs{this, "Output", {}, "List of provided outputs"};

  /// Collection IDs (retrieved with CollectionIDTable from ROOT file, using collection names)
  std::vector<int> m_collection_ids;

  DataHandle<edm4hep::SimTrackerHitCollection> r_SimDCHCol{"DCHIonizedEleCollection", Gaudi::DataHandle::Writer, this};

  SmartIF<IIncidentSvc> m_incidentSvc;

  /// A shortcut to the whiteboard
  SmartIF<IHiveWhiteBoard> m_whiteboard;
    
  SmartIF<IDataProviderSvc> m_DataProviderSvc;

  Gaudi::Property<std::vector<std::string>> m_inputs{this, "Input", {}, "List of required inputs"};
  Gaudi::Property<int> m_type{this, "Type", -1, "type of algo"};
  Gaudi::Property<std::vector<float>> m_range{this, "Range", {}, "range"};//percentage
  Gaudi::Property<int> m_max_current{this, "max_current", 5000, ""};
  Gaudi::Property<std::string> m_file{this, "sp_file", "", "sp file"};
  Gaudi::Property<std::vector<float>> m_sp_peak_value{this, "sp_peak_value", {}, "sp_peak_value"};
  Gaudi::Property<int> m_sleep{this, "sleep", 0, ""};

  //std::map<std::string, DataObjectHandleBase*> m_output_dataHandles;
  NNPred* m_NNPred;
  NTuple::Tuple* m_tuple = nullptr ;
  NTuple::Item<long>   m_ncurrent;
  NTuple::Array<float>  m_current;
  NTuple::Array<float>  m_time;
};
