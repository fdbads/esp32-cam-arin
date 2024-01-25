import time
import serial 
import random 

ser = serial.Serial("COM8",115200)
print(ser)
time.sleep(1)
#ser.setDTR(False)
#ser.setRTS(False)

match_count = 0

print(ser.timeout)

while True:
    to_esp_int = 15#random.randint(10,250)
    to_esp = int.to_bytes(to_esp_int,signed=False)
    ser.write(to_esp)
    print("Sending:",str(to_esp).rjust(5),end=" ")
    
    
    from_esp = ser.read_until()
    from_esp_int = int.from_bytes(from_esp,signed=False)
    from_esp_bin = bin(from_esp_int)[2:].rjust(8,"0")

    print("Recieved:",str(from_esp).rjust(5),end=" ")
    #print(int.from_bytes(from_esp,signed=False))

    if to_esp == from_esp:
        match_count += 1
    else:
        match_count = 0
    
    print("match:",match_count)
    
    if match_count >= 10:
        print("--connected--")
        break
    
    time.sleep(0.05)

for i in range(64):
    
    to_esp = int.to_bytes(10,signed=False)
    to_esp_bin = bin(int.from_bytes(to_esp,signed=False))[2:].rjust(8,"0")
    print("Sending:",to_esp_bin,end=" ")
    
    ser.write(to_esp)
    
    from_esp = ser.read()
    from_esp_bin = bin(int.from_bytes(from_esp,signed=False))[2:].rjust(8,"0")

    print("Recieved:",from_esp_bin)
    #print(int.from_bytes(from_esp,signed=False))
    
    time.sleep(0.1)