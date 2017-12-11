#!/usr/bin/env python
# Log data from serial port

# Author: Diego Herranz

import argparse
import serial
import datetime
import time
import os

path = "C:/Users/Joseph/Documents/Stanford/Autumn 2017/EE 255/Final Project/TestData/Testing/"
parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-d", "--device", help="device to read from", default="COM8")
parser.add_argument("-s", "--speed", help="speed in bps", default=38400, type=int)
args = parser.parse_args()

outputFilePath = os.path.join(os.path.dirname(path),
                 datetime.datetime.now().strftime("%Y-%m-%dT%H.%M.%S") + ".txt")

with serial.Serial(args.device, args.speed) as ser, open(outputFilePath, mode='wb+') as outputFile:
    print("Logging started. Ctrl-C to stop.")
    try:
        while True:
            time.sleep(1)
            outputFile.write(bytes(ser.read(ser.inWaiting())))
            outputFile.flush()
    except KeyboardInterrupt:
        print("Logging stopped")