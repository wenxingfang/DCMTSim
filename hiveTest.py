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
from Configurables import HiveWhiteBoard, HiveSlimEventLoopMgr, HiveTestAlg, AvalancheSchedulerSvc

evtslots = 10

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

slimeventloopmgr = HiveSlimEventLoopMgr(OutputLevel=DEBUG)

scheduler = AvalancheSchedulerSvc(ThreadPoolSize=8, OutputLevel=WARNING)


a1 = HiveTestAlg("A1", Output=['/Event/a1'], Type = 0, sp_file="/junofs/users/wxfang/FastSim/GAN/CEPC/DCH/FastSim/simulateQT/sp_x0p25.txt", sp_peak_value=[87.25,0.0041667])
a2 = HiveTestAlg("A2", Input=['/Event/a1'], Output=['/Event/a2'], Type = 1, Range=[0  ,0.4], sp_file="/junofs/users/wxfang/FastSim/GAN/CEPC/DCH/FastSim/simulateQT/sp_x0p25.txt", sp_peak_value=[87.25,0.0041667])
a3 = HiveTestAlg("A3", Input=['/Event/a1'], Output=['/Event/a3'], Type = 1, Range=[0.4,1  ], sp_file="/junofs/users/wxfang/FastSim/GAN/CEPC/DCH/FastSim/simulateQT/sp_x0p25.txt", sp_peak_value=[87.25,0.0041667])
a4 = HiveTestAlg(
    "A4", Input=['/Event/a2', '/Event/a3'], Output=['/Event/a4'], Type = 2)

a1.Cardinality = 3
a2.Cardinality = 3
a3.Cardinality = 3
a4.Cardinality = 3

from Configurables import NTupleSvc
ntsvc = NTupleSvc("NTupleSvc")
ntsvc.Output = ["MyTuples DATAFILE='dummy.root' OPT='NEW' TYP='ROOT'"]


ApplicationMgr(
    EvtMax=100,
    EvtSel='NONE',
    #ExtSvc=[whiteboard],
    ExtSvc=[whiteboard, ntsvc],
    EventLoop=slimeventloopmgr,
    TopAlg=[a1, a2, a3, a4],
    HistogramPersistency = "ROOT",
    MessageSvcType="InertMessageSvc")
