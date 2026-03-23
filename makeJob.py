#!/cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-dbg/bin/python3
import ROOT, math, random, os, sys


tmpMac = """/random/setSeeds {r1} {r2}
/run/numberOfThreads 10

/detector/targetMaterial G4_W
/detector/targetLength 15 cm

/analysis/setFileName {fName}

/run/initialize

/gun/particle e-
/gun/position 0 0 -8 cm
/gun/direction 0 0 1
/gun/energy 8 GeV

/run/beamOn 1000
"""


tmpSh = """#!/bin/sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-dbg/setup.sh
cd {batchPath}
{g4Path}/ALPGun {gMac}
"""

condorSub = """executable              = $(filename)
universe                = vanilla
getenv                  = True
RequestCpus     = 10
RequestMemory       = 15360
+JobBatchName = "{batchName}"
queue filename matching {shFiles}*.sh
"""

g4Path = os.getcwd()

batchPath = g4Path+ '/batch/eBeam_8_GeV_W_15_cm_target_1E6_EOT'
os.makedirs(batchPath, exist_ok=True)
os.chdir(batchPath)
for r in range(1000):
    tmpName = "eBeam_8_GeV_W_15_cm_target_1E3_EOT_{r:03d}".format(r=r)
    tmpC = open(tmpName+'.mac','w')
    r1 = "%d"%(random.random()*1000000)
    r2 = "%d"%(random.random()*1000000)
    tmpC.write(tmpMac.format(
        r1=r1, r2=r2,fName=tmpName
    ))
    tmpC.close()
    tmpS = open(tmpName+'.sh','w')
    tmpS.write(tmpSh.format(g4Path=g4Path,batchPath=batchPath,gMac=tmpName+'.mac'))
    tmpS.close()

os.system("chmod 755 *.sh")
condorSubmit = open("condor.sub","w")
condorSubmit.write(condorSub.format(shFiles='eBeam_',batchName=batchPath.split("/")[-1]))
condorSubmit.close()
os.system("condor_submit condor.sub")
os.chdir(g4Path)
