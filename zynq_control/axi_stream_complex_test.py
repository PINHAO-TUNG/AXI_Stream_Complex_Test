from __future__ import print_function

import sys, os
import numpy as np
from time import time 

sys.path.append('/home/xilinx')
os.environ['XILINX_XRT'] = '/usr'
from pynq import Overlay
from pynq import allocate

if __name__ == "__main__":
    print("Entry:", sys.argv[0])
    print("System argument(s):", len(sys.argv))

    print("Start of \"" + sys.argv[0] + "\"")
    ol = Overlay("/home/xilinx/axist_vchi.bit")
    print("Bitstream loaded")
    ipDMA = ol.axi_dma_0

    fiSamples = open("data_5m.txt", "r+")
    numSamples = 0
    line = fiSamples.readline()

    while line:
        numSamples = numSamples + 1
        line = fiSamples.readline()

    inBuffer0 = allocate(shape=(numSamples,), dtype=np.float32)
    outBuffer0 = allocate(shape=(10,), dtype=np.float32)
    fiSamples.seek(0)

    for i in range(numSamples):
        line = fiSamples.readline()
        inBuffer0[i] = float(line)
    fiSamples.close()

    timeKernelStart = time()
    ipDMA.sendchannel.transfer(inBuffer0)
    ipDMA.recvchannel.transfer(outBuffer0)
    ipDMA.sendchannel.wait()
    ipDMA.recvchannel.wait()
    timeKernelEnd = time()

    print("============================")
    print("Kernel execution time: " + str(timeKernelEnd - timeKernelStart) + " s")
    print("============================")
    print("outBuffer0:", outBuffer0) 

    print("============================")
    print("Exit process")

