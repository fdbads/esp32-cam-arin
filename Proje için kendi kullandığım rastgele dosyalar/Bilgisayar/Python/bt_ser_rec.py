#import bluetooth
"""# simple inquiry example

nearby_devices = bluetooth.discover_devices(lookup_names=True)
print("Found {} devices.".format(len(nearby_devices)))

for addr, name in nearby_devices:
    print("  {} - {}".format(addr, name))"""

"""devices = bluetooth.discover_devices(lookup_names=True)
print(type(devices))
 
print("Devices found: %s" % len(devices))
 
for item in devices:
    print(item)"""

"""
sensor_address = 'A8:42:E3:4A:50:1A'
socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

#data = socket.recv(8)
"""

import time
import serial

ser = serial.Serial("COM12")
print(ser)


"""
while True:
    print(ser.read())
"""
rx_buffer = [b'\x00',b'\x00',b'\x00']
i = 0

#fh = open("capture1.txt","w")
fh = open("capture4.jpg","wb")


while True:
        rx_buffer.pop(0)
        rx_buffer.append(ser.read())
        
        #print(rx_buffer)
        
        if rx_buffer[0] == b'@' and rx_buffer[1] == b'#' and rx_buffer[2] == b'@':
            print("Rx start")
            break

"""for j in range(10230):
    read = ser.read()
    rx_buffer.pop(0)
    rx_buffer.append(read)
        
    #print(i,read)
    #fh.write(str(int.from_bytes(read,signed=False)))
    fh.write(read)
    #fh.write(",")
    i += 1"""

t = time.time()
while True:
    read = ser.read()

    #print(read)
    if read == b"\n":
        print(i,"back n")

    rx_buffer.pop(0)
    rx_buffer.append(read)
    
    if rx_buffer[0] == b'@' and rx_buffer[1] == b'#' and rx_buffer[2] == b'@':    
        
        print("breaking")
        print("J",i)
        break
    
    i += 1

print("Rx done in",round(time.time()-t,2),"ms")
fh.close()