#!/usr/bin/env python

from Gaudi.Configuration import *

from Configurables import HiveReadPodioAlg
podioinput = HiveReadPodioAlg("A1", InputFiles=["/cefs/higgs/wxfang/cepc/FastSim/detsim/G4TrackHeed_saveMC//detsim_pi-_K-.root"], Output=['DCHIonizedEleCollection'])
##############################################################################
# Geometry Svc
##############################################################################


# geometry_option = "CepC_v4-onlyTracker.xml"
#geometry_option = "det.xml"
#
#if not os.getenv("DETDRIFTCHAMBERROOT"):
#    print("Can't find the geometry. Please setup envvar DETDRIFTCHAMBERROOT." )
#    sys.exit(-1)
#
#geometry_path = os.path.join(os.getenv("DETDRIFTCHAMBERROOT"), "compact", geometry_option)
#if not os.path.exists(geometry_path):
#    print("Can't find the compact geometry file: %s"%geometry_path)
#    sys.exit(-1)
#
#from Configurables import GeomSvc
#geosvc = GeomSvc("GeomSvc")
#geometry_path = 'Detector/DetCRD/compact/CRD_o1_v01/CRD_o1_v01.xml'
#geosvc.compact = geometry_path
#print('compact=',geometry_path)

##############################################################################
# NTuple Svc
##############################################################################

#from Configurables import NTupleSvc
#ntsvc = NTupleSvc("NTupleSvc")
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_2000evt_TrackHeed.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed_primary_max_step_10.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed_all_max_step_10.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed_noIoni_max_step_10.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed_updateKE_save_length_0p5mmSteplimit.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='Ana_pi-_1GeV_1000evt_TrackHeed_updateKE_save_length_0p5mmSteplimit_w.root' OPT='NEW' TYP='ROOT'"]
#ntsvc.Output = ["MyTuples DATAFILE='test.root' OPT='NEW' TYP='ROOT'"]

##############################################################################
# DumpAlg
##############################################################################

#from Configurables import DigiSaveAlg
#alg = DigiSaveAlg("DigiSaveAlg")
#alg.plotDrift = False
#alg.max_hit = 100000
#alg.max_e0  = 100000
#alg.max_e   = 100000

#from Configurables import PodioOutput
#out = PodioOutput("out")
#out.filename = "detsim_Second_out.root"
#out.outputCommands = ["keep *"]
# ApplicationMgr
from Configurables import ApplicationMgr
ApplicationMgr( TopAlg = [podioinput],
#ApplicationMgr( TopAlg = [podioinput,alg,out],
#ApplicationMgr( TopAlg = [podioinput,alg],
                EvtSel = 'NONE',
                EvtMax = 2,
                HistogramPersistency = "ROOT",
                OutputLevel=DEBUG
)
