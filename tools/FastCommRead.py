#The only goal of this program is to make serial reads as quickly as possible.

import os 
import glob
from time import time, sleep
import serial
from datetime import datetime

ser = serial.Serial(port='/dev/ttyUSB0', baudrate=19200, timeout=1)
ser.readlines()

ser.write(b"\x08\x01\x00\x02\x0e\x01\xe2\x58") #Initial Heartbeat
ser.readline()
time1 = time()

ser.write(b"\x08\x02\x00\x02\x06\x00\xf6\x4c") #Commodity request
data = ser.readline()
sleep(1)
data = data.encode('hex')
Etot = float(int(data[58:70],16))

while True:
    time2 = time()
    if (time2-time1) > 240:
        ser.write(b"\x08\x01\x00\x02\x0e\x01\xe2\x58") #Refresh Heartbeat
        ser.readline()
        time1 = time()
    ser.write(b"\x08\x02\x00\x02\x06\x00\xf6\x4c") #Commodity request
    data = ser.readline()
    data = data.encode('hex')
    P = str(int(data[20:32],16))
    E = str(int(data[84:96],16))
    Epercent = int(E)/Etot * 100
    print('---\nEnergy Capacity: ' + E + ' (%.2f%%)' %Epercent + '\nPower: ' + P + '\n---')
    sleep(1)
