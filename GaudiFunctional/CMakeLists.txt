#cmake_minimum_required(VERSION 2.8.5)
#
#gaudi_subdir(GaudiFunctional)
#
#gaudi_add_module(GaudiFunctional src/*.cpp
#    LINK_LIBRARIES GaudiKernel GaudiAlgLib )

gaudi_add_module(GaudiFunctional
                 SOURCES src/DataConsumer.cpp
                         src/DataMaker.cpp
                         #src/DataMaker_v1.cpp
                         src/DataConsumerVec.cpp
                         src/DataMakerEDM.cpp
                         src/DataConsumerEDM.cpp
                         src/MakerIons.cpp
                         src/SimWF.cpp
                         src/MergeWF.cpp
                 LINK k4FWCore::k4FWCore
                      Gaudi::GaudiAlgLib Gaudi::GaudiKernel
                      EDM4HEP::edm4hep EDM4HEP::edm4hepDict
                      ${podio_LIBRARIES} podio::podioRootIO
)

target_include_directories(GaudiFunctional PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/include
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS GaudiFunctional
  EXPORT CEPCSWTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)
