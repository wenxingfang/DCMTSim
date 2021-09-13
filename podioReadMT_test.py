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
from Gaudi.Configuration import *
from Configurables import HiveWhiteBoard, HiveSlimEventLoopMgr, AvalancheSchedulerSvc, HiveReadPodioAlg

evtslots = 1

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

slimeventloopmgr = HiveSlimEventLoopMgr(OutputLevel=DEBUG)

scheduler = AvalancheSchedulerSvc(ThreadPoolSize=4, OutputLevel=WARNING)

#a1 = HiveReadPodioAlg("A1", InputFiles=["/cefs/higgs/wxfang/cepc/FastSim/detsim/G4TrackHeed_saveMC//detsim_pi-_K-.root"], Output=['/Event/DCHIonizedEleCollection'])
a1 = HiveReadPodioAlg("A1", InputFiles=["/cefs/higgs/wxfang/cepc/FastSim/detsim/G4TrackHeed_saveMC//detsim_pi-_K-.root"], Output=['DCHIonizedEleCollection'])


ApplicationMgr(
    #EvtMax=10,
    EvtMax=100,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=slimeventloopmgr,
    #TopAlg=[a1, a2, a3, a4],
    TopAlg=[a1],
    HistogramPersistency = "ROOT",
    OutputLevel=DEBUG,
    MessageSvcType="InertMessageSvc")
