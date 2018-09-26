import serial
from time import time

ser = serial.Serial(port='/dev/ttyUSB0', baudrate=19200, timeout=1)
ser.readlines()

ser.write(b"\x08\x01\x00\x02\x0e\x01\xe2\x58") #Initial Heartbeat
ser.readline()
time1 = time()

while True:
    command = str.lower(raw_input('c = commodity read\ns = shed\ne = end shed\nx = grid emergency\n'))
    
    time2 = time()
    if (time2 - time1) > 240:
        ser.write(b"\x08\x01\x00\x02\x0e\x01\xe2\x58") #Update Heartbeat
        ser.readline()
        time1 = time()

    if command == 'c':
        ser.write(b"\x08\x02\x00\x02\x06\x00\xf6\x4c") #Commodity request
        data = ser.readline()
        data = data.encode('hex')
        P = str(int(data[20:32],16))
        E = str(int(data[84:96],16))
        Etot = float(int(data[58:70],16))
        Epercent = int(E)/Etot * 100
        print('---\nTotal Energy Capacity: ' + str(Etot) + '\nCurrent Energy Capacity: ' + E + ' (%.2f%%)' %Epercent + '\nPower: ' + P + '\n---')

    if command == 's':
        print ('\nSending shed command\n')
        ser.write(b"\x08\x01\x00\x02\x01\x00\x0c\x3d ")
        ser.readlines()

    if command == 'e':
        print ('\nSending end shed command\n')
        ser.write(b"\x08\x01\x00\x02\x02\x00\x09\x3f")
        ser.readlines()

    if command == 'x':
        print ('\nSending grid emergency command\n')
        ser.write(b"\x08\x01\x00\x02\x0b\x00\xed\x51")
        ser.readlines()
        
