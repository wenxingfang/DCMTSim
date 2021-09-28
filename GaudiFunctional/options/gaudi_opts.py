from Gaudi.Configuration import *
from Configurables import (DataMaker, DataMaker_v1, DataConsumer, DataConsumerVec, DataMakerEDM, DataConsumerEDM)

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"
ApplicationMgr().OutputLevel=DEBUG


alg1 = DataMaker()
alg1_v1 = DataMaker_v1()
alg2 = DataConsumer()
alg3 = DataConsumerVec()


a1 = DataMakerEDM()
a1.OutputLocation="/Event/MySimHit"
a2 = DataConsumerEDM()
a2.InputLocation="/Event/MySimHit"
a2.OutputLocation="/Event/MyOut"


ApplicationMgr().TopAlg.append(a1)
ApplicationMgr().TopAlg.append(a2)
