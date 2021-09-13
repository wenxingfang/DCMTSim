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
#include "HiveReadPodioAlg.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ThreadLocalContext.h"
#include "GaudiKernel/IEventProcessor.h"

#include "GaudiKernel/INTupleSvc.h"
#include <atomic>
#include <map>
#include <time.h>

DECLARE_COMPONENT( HiveReadPodioAlg )

using namespace std;

StatusCode HiveReadPodioAlg::initialize() {
  info() << "HiveReadPodioAlg::initialize " << endmsg;
  m_incidentSvc = service( "IncidentSvc", true  );
  ISvcLocator* svc_loc = serviceLocator();
  //m_whiteboard = svc_loc->service("EventDataSvc");
  m_DataProviderSvc  = svc_loc->service("EventDataSvc");
  //info() << "m_whiteboard="<<m_whiteboard.get()<< endmsg;
  //info() << "m_DataProviderSvc="<<m_DataProviderSvc.get()<< endmsg;
  // Attach data loader facility
  m_cnvSvc = svc_loc->service("EventPersistencySvc");
  m_collectionIDs = new podio::CollectionIDTable();
  for ( auto k : m_input_files ) {
    debug() << "adding input files " << k << endmsg;
    m_filenames.push_back(k);
  }
  if (m_filenames.size() > 0) {
    if (m_filenames[0] != "") {
      m_reader.openFiles(m_filenames);
      m_eventMax = m_reader.getEntries();
      debug() << "m_eventMax= " << m_eventMax << endmsg;
      auto idTable = m_reader.getCollectionIDTable();
  
      setCollectionIDs(idTable);
      m_provider.setReader(&m_reader);
    }
  }

  auto idTable = getCollectionIDs();
  for ( auto name : m_outputs ) {
    debug() << "Finding collection " << name << " in collection registry." << endmsg;
    if (!idTable->present(name)) {
      error() << "Requested product " << name << " not found." << endmsg;
      return StatusCode::FAILURE;
    }
    m_collection_ids.push_back(idTable->collectionID(name));
  }

  /*
  for (unsigned i=0; i<m_outputs.value().size(); i++) {
    std::string colName = m_outputs.value().at(i);
    std::string colType = m_type_outputs.value().at(i);
    debug() << "adding output key " << colName<<", with type "<< colType << endmsg;
    if ( colType == "SimTrackerHit" ) {
        m_output_dataHandles[colName] =  new DataHandle<edm4hep::SimTrackerHitCollection>(colName, Gaudi::DataHandle::Writer, this);
    }
    else{
        info() << "Unknown collection type "<< colType << endmsg;
        return StatusCode::FAILURE;
    }
  }
  */

  info() << "HiveReadPodioAlg::initialize done" << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HiveReadPodioAlg::execute() {
  debug() << "m_total= " << m_total<<",m_eventNum="<<m_eventNum << endmsg;
  ++m_total;
  int evt = Gaudi::Hive::currentContext().evt();
  //m_DataSvc->setRoot("/Event", new DataObject());
  info() << "HiveReadPodioAlg::getting inputs... " << evt << endmsg;
  size_t cntr = 0;
  for (auto& id : m_collection_ids) {
    const std::string& collName = m_outputs.value().at(cntr++);
    debug() << "Registering collection to read " << collName << " with id " << id << endmsg;
    if (readCollection(collName, id).isFailure()) {
      return StatusCode::FAILURE;
    }
  }
  endOfRead();
  //sleep(30);//FIXME, seems the A1 must be finished, then A2 and A3 can start.
  const edm4hep::SimTrackerHitCollection* SimHitCol =  r_SimDCHCol.get();
  debug()<<"input sim hit size="<< SimHitCol->size()<<",evt="<<evt<<endmsg;
  DataObject* tmp_DataObject=nullptr;
  m_DataProviderSvc->findObject("/Event/DCHIonizedEleCollection",tmp_DataObject);
  debug() << "A: " << tmp_DataObject << endmsg;
  auto ptmp = dynamic_cast<DataWrapperBase*>(tmp_DataObject)->collectionBase();
  edm4hep::SimTrackerHitCollection* tmp_new_DataObject = dynamic_cast<edm4hep::SimTrackerHitCollection*>(ptmp);
  debug() << "B: " << tmp_new_DataObject << endmsg;
  debug()<<tmp_new_DataObject->size()<<endmsg;

  return StatusCode::SUCCESS;
}

StatusCode HiveReadPodioAlg::finalize() {
  info() << name() << ":HiveReadPodioAlg::finalize - total events: " << m_total << endmsg;
  m_cnvSvc = 0; // release
  return StatusCode::SUCCESS;
}

const std::vector<std::string> HiveReadPodioAlg::get_inputs() { return m_inputs; }

const std::vector<std::string> HiveReadPodioAlg::get_outputs() { return m_outputs; }

void HiveReadPodioAlg::setCollectionIDs(podio::CollectionIDTable* collectionIds) {
  if (m_collectionIDs != nullptr) {
    delete m_collectionIDs;
  }
  m_collectionIDs = collectionIds;
}

StatusCode HiveReadPodioAlg::readCollection(const std::string& collName, int collectionID) {
  podio::CollectionBase* collection(nullptr);
  m_provider.get(collectionID, collection);
  int id = m_collectionIDs->add(collName);
  collection->setID(id);
  // datahandle //
  r_SimDCHCol.put(dynamic_cast<edm4hep::SimTrackerHitCollection*>(collection));
  debug() << "Registered " << collName << " with id " << id << endmsg;
  return StatusCode::SUCCESS;
  /*
  auto wrapper = new DataWrapper<podio::CollectionBase>;
  wrapper->setData(collection);
  m_readCollections.emplace_back(std::make_pair(collName, collection));
  return m_DataProviderSvc->registerObject("/Event", "/" + collName, wrapper);//can we use datahandle to do this?
  */
}

void HiveReadPodioAlg::endOfRead() {
  StatusCode sc;
  if (m_eventMax != -1) {
    m_provider.clearCaches();
    m_reader.endOfEvent();
    if (m_eventNum++ > m_eventMax) {
      info() << "Reached end of file with event " << m_eventMax << endmsg;
      IEventProcessor* eventProcessor;
      sc = service("HiveSlimEventLoopMgr", eventProcessor);
      info() << "sc= " << sc << endmsg;
      sc = eventProcessor->stopRun();
      //m_incidentSvc->fireIncident( Incident( name(), IncidentType::AbortEvent  )  );
      //auto ep = serviceLocator()->as<IEventProcessor>();
      //auto sc1 = ep->stopRun();
    }
  }
  // todo: figure out sthg to do with sc (added to silence -Wunused-result)
}
