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
from Configurables import HiveWhiteBoard, HiveSlimEventLoopMgr, AvalancheSchedulerSvc, MakerIons, SimWF, MergeWF

evtslots = 5

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

slimeventloopmgr = HiveSlimEventLoopMgr(OutputLevel=DEBUG)

scheduler = AvalancheSchedulerSvc(ThreadPoolSize=8, OutputLevel=WARNING)

A1 = MakerIons("IonsProducer")
A1.OutputLocation="/Event/MyIons"
A2 = SimWF("SimA2") 
A2.InputLocation="/Event/MyIons"
A2.OutputLocation="/Event/MySimA2"
A2.Range=[0  ,0.5]
A3 = SimWF("SimA3") 
A3.InputLocation="/Event/MyIons"
A3.OutputLocation="/Event/MySimA3"
A3.Range=[0.5  ,1]

A4 = MergeWF("MergeWF") 
A4.InputLocations=["/Event/MySimA2", "/Event/MySimA3"]
A4.OutputLocation="/Event/MyMergeWF"


ApplicationMgr(
    EvtMax=100,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=slimeventloopmgr,
    TopAlg=[A1, A2, A3, A4],
    #TopAlg=[A1, A2, A3],
    #TopAlg=[A1, A2],
    HistogramPersistency = "ROOT",
    OutputLevel=DEBUG,
    MessageSvcType="InertMessageSvc")
