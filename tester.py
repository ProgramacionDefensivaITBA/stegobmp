#!/usr/bin/env python3

import subprocess
import os
from pprint import pprint

for file in ['buenosaires']:
    for enc in ['aes128', 'aes192', 'aes256']:
        for steg in ['LSB4', 'LSB1', 'LSBE']:
            for mode in ['ecb', 'cfb', 'ofb', 'cbc']:
                print("Testing: file: %s enc:%s steg:%s mode:%s" % (file, enc, steg, mode))

                out_file = "enc%s-steg%s-mode%s" % (enc, steg, mode)

                cmd = ["./cmake-build-debug/stegobmp", '-extract',
                       '-p', "sample/%s.bmp" % file,
                       '-out', "sample/budapest." + out_file,
                       "-steg", steg,
                       "-a", enc,
                       "-m", mode,
                       "-pass", "wanted"]

                a = subprocess.run(cmd, cwd=os.path.dirname(os.path.realpath(__file__)))
                #
                # pprint(a)
