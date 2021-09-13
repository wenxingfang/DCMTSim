#####################################################################################
# (c) Copyright 1998-2019 CERN for the benefit of the LHCb and ATLAS collaborations #
#                                                                                   #
# This software is distributed under the terms of the Apache version 2 licence,     #
# copied verbatim in the file "LICENSE".                                            #
#                                                                                   #
# In applying this licence, CERN does not waive the privileges and immunities       #
# granted to it by virtue of its status as an Intergovernmental Organization        #
# or submit itself to any jurisdiction.                                             #
#####################################################################################
# GaudiHive subdirectory

# Register the header only library (install include/)
#gaudi_add_header_only_library(GaudiHiveHeaders)

#include_directories("/junofs/users/wxfang/MyGit/tmp/check_G4FastSim_20210121/CEPCSW/Digitisers/SimCurrent/src/")
#LINK_DIRECTORIES("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/libtorch/1.6.0/lib/")

# Build the plugin
gaudi_add_module(DCMTSimTest
                 SOURCES src/DCMTSimAlg.cpp
                 SOURCES src/HiveReadPodioAlg.cpp
                 SOURCES src/HiveTestAlg.cpp
                 SOURCES src/ReadPodioAlg.cpp
                 LINK Gaudi::GaudiAlgLib
                      Gaudi::GaudiKernel
                      #/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/Gaudi-v35r0-build/GaudiKernel/libGaudiKernel.so
                      #/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/Gaudi-v35r0-build/GaudiAlg/libGaudiAlgLib.so
                      #/junofs/users/wxfang/MyGit/tmp/check_G4FastSim_20210121/CEPCSW/Digitisers/SimCurrent/src/NN/libNNPred.so
                      k4FWCore::k4FWCore
                      EDM4HEP::edm4hep
                      EDM4HEP::edm4hepDict
                      ${podio_LIBRARIES} podio::podioRootIO
                      Boost::headers
                      Boost::system
                      Boost::filesystem
                      Boost::graph
                      ROOT::Hist
                      ROOT::RIO
                      TBB::tbb
                      Rangev3::rangev3
                      ${rt_LIBRARY})

install(TARGETS DCMTSimTest
  EXPORT CEPCSWTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)


## Build executable
#gaudi_add_executable(makeThesis
#                     SOURCES src/bin/makeThesis.cpp)
#gaudi_add_executable(concurrentRun
#                     SOURCES src/bin/concurrentRun.cpp
#                     LINK GaudiKernel
#                          Boost::headers
#                          Boost::program_options)
#
## QMTest
#gaudi_add_tests(QMTest)
#
## Install python modules
#gaudi_install(PYTHON)
## Install other scripts
#gaudi_install(SCRIPTS)
