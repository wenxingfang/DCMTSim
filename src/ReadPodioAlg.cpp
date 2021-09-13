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
#include "ReadPodioAlg.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ThreadLocalContext.h"

#include "GaudiKernel/INTupleSvc.h"
#include <atomic>
#include <map>
#include <time.h>

DECLARE_COMPONENT( ReadPodioAlg )

using namespace std;

StatusCode ReadPodioAlg::initialize() {
  info() << "ReadPodioAlg::initialize " << endmsg;
  ISvcLocator* svc_loc = serviceLocator();
  m_DataSvc = dynamic_cast<DataSvc*>( svc_loc->service("EventDataSvc").get() );
  info() << "m_DataSvc="<<m_DataSvc<< endmsg;
  info() << "Hi 1 " << endmsg;
  // Attach data loader facility
  m_cnvSvc = svc_loc->service("EventPersistencySvc");
  info() << "Hi 2 " << endmsg;
  //StatusCode status = m_DataSvc->setDataLoader(m_cnvSvc);
  info() << "Hi 3 " << endmsg;
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
  return StatusCode::SUCCESS;
  info() << "ReadPodioAlg::initialize done" << endmsg;
}

StatusCode ReadPodioAlg::execute() {
  ++m_total;
  int evt = Gaudi::Hive::currentContext().evt();
  m_DataSvc->setRoot("/Event", new DataObject());
  info() << ":ReadPodioAlg::getting inputs... " << evt << endmsg;
  size_t cntr = 0;
  for (auto& id : m_collection_ids) {
    const std::string& collName = m_outputs.value().at(cntr++);
    debug() << "Registering collection to read " << collName << " with id " << id << endmsg;
    if (readCollection(collName, id).isFailure()) {
      return StatusCode::FAILURE;
    }
    info() << "Hi 4 " << endmsg;
  }
  endOfRead();

  const edm4hep::SimTrackerHitCollection* SimHitCol =  r_SimDCHCol.get();
  debug()<<"input sim hit size="<< SimHitCol->size()<<endmsg;
  DataObject* tmp_DataObject=nullptr;
  m_DataSvc->findObject("/Event/DCHIonizedEleCollection",tmp_DataObject);
  debug() << "A: " << tmp_DataObject << endmsg;
  auto ptmp = dynamic_cast<DataWrapperBase*>(tmp_DataObject)->collectionBase();
  edm4hep::SimTrackerHitCollection* tmp_new_DataObject = dynamic_cast<edm4hep::SimTrackerHitCollection*>(ptmp);
  debug() << "B: " << tmp_new_DataObject << endmsg;
  debug()<<tmp_new_DataObject->size()<<endmsg;

  return StatusCode::SUCCESS;
}

StatusCode ReadPodioAlg::finalize() {
  info() << name() << ":ReadPodioAlg::finalize - total events: " << m_total << endmsg;
  //EleObject::dump();
  m_cnvSvc = 0; // release
  return StatusCode::SUCCESS;
}

const std::vector<std::string> ReadPodioAlg::get_inputs() { return m_inputs; }

const std::vector<std::string> ReadPodioAlg::get_outputs() { return m_outputs; }

void ReadPodioAlg::setCollectionIDs(podio::CollectionIDTable* collectionIds) {
  debug() << "ReadPodioAlg::setCollectionIDs, m_collectionIDs="<<m_collectionIDs << endmsg;
  if (m_collectionIDs != nullptr) {
    debug() << "ReadPodioAlg::setCollectionIDs before del" << endmsg;
    delete m_collectionIDs;
    debug() << "ReadPodioAlg::setCollectionIDs after del" << endmsg;
  }
  m_collectionIDs = collectionIds;
  debug() << "ReadPodioAlg::setCollectionIDs done" << endmsg;
}

StatusCode ReadPodioAlg::readCollection(const std::string& collName, int collectionID) {
  podio::CollectionBase* collection(nullptr);
  m_provider.get(collectionID, collection);
  auto wrapper = new DataWrapper<podio::CollectionBase>;
  int id = m_collectionIDs->add(collName);
  collection->setID(id);
  wrapper->setData(collection);
  m_readCollections.emplace_back(std::make_pair(collName, collection));
  //return DataSvc::registerObject("/Event", "/" + collName, wrapper);
  return m_DataSvc->registerObject("/Event", "/" + collName, wrapper);
}

void ReadPodioAlg::endOfRead() {
  StatusCode sc;
  if (m_eventMax != -1) {
    m_provider.clearCaches();
    m_reader.endOfEvent();
    if (m_eventNum++ > m_eventMax) {
      info() << "Reached end of file with event " << m_eventMax << endmsg;
      IEventProcessor* eventProcessor;
      sc = service("ApplicationMgr", eventProcessor);
      sc = eventProcessor->stopRun();
    }
  }
  // todo: figure out sthg to do with sc (added to silence -Wunused-result)
}
