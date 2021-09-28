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
from Configurables import HiveWhiteBoard, HiveSlimEventLoopMgr, AvalancheSchedulerSvc, DataMakerEDM, DataConsumerEDM

evtslots = 1

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

slimeventloopmgr = HiveSlimEventLoopMgr(OutputLevel=DEBUG)

scheduler = AvalancheSchedulerSvc(ThreadPoolSize=8, OutputLevel=WARNING)

a1 = DataMakerEDM()
a1.OutputLocation="/Event/MySimHit"
a2 = DataConsumerEDM()
a2.InputLocation="/Event/MySimHit"
a2.OutputLocation="/Event/MyOut"

ApplicationMgr(
    EvtMax=10,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=slimeventloopmgr,
    TopAlg=[a1, a2],
    HistogramPersistency = "ROOT",
    MessageSvcType="InertMessageSvc")
