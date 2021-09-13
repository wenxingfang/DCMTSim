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
from Configurables import HiveWhiteBoard, HiveSlimEventLoopMgr, AvalancheSchedulerSvc, HiveReadPodioAlg, DCMTSimAlg

evtslots = 3

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

slimeventloopmgr = HiveSlimEventLoopMgr(OutputLevel=DEBUG)

scheduler = AvalancheSchedulerSvc(ThreadPoolSize=8, OutputLevel=WARNING)

a1 = HiveReadPodioAlg("A1", InputFiles=["/cefs/higgs/wxfang/cepc/FastSim/detsim/G4TrackHeed_saveMC//detsim_pi-_K-.root"], Output=['DCHIonizedEleCollection'])
a2 = DCMTSimAlg("A2", Input=['DCHIonizedEleCollection'], Input_type=['SimTrackerHit'], Output=['SingleWF_a2'], Output_type=['TPCHit'], Type = 1, Range=[0  ,0.5], sp_file="/junofs/users/wxfang/FastSim/GAN/CEPC/DCH/FastSim/simulateQT/sp_x0p25.txt", sp_peak_value=[87.25,0.0041667])
a3 = DCMTSimAlg("A3", Input=['DCHIonizedEleCollection'], Input_type=['SimTrackerHit'], Output=['SingleWF_a3'], Output_type=['TPCHit'], Type = 1, Range=[0.5,1  ], sp_file="/junofs/users/wxfang/FastSim/GAN/CEPC/DCH/FastSim/simulateQT/sp_x0p25.txt", sp_peak_value=[87.25,0.0041667])
a4 = DCMTSimAlg("A4", Input=['SingleWF_a2', 'SingleWF_a3'], Input_type=['TPCHit','TPCHit'], Output=['MergedWF_a4'], Output_type=['TPCHit'], Type = 2)

a1.Cardinality = 1# shoud be 1 here
a2.Cardinality = 3
a3.Cardinality = 3
a4.Cardinality = 3

ApplicationMgr(
    EvtMax=6,
    #EvtMax=-1,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=slimeventloopmgr,
    TopAlg=[a1, a2, a3, a4],
    HistogramPersistency = "ROOT",
    OutputLevel=DEBUG,
    MessageSvcType="InertMessageSvc")
