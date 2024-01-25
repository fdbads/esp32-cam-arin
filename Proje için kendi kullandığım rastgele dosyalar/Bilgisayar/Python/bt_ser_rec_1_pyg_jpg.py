import time
import serial
t = time.time()

ser = serial.Serial("COM8",115200)
print(ser)
ser.setDTR(False)
ser.setRTS(False)

for i in range(10):
    time.sleep(0.2)
    print(i,end=" ")

mode = 1

ser.write(b"x")

while mode == 0:    
    print(ser.read())




#while mode == 0:

rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
rx_buffer_1 = [b'\x00',b'\x00',b'\x00']
rx_msg = []
"""
@#@ start
#@# end
"""
msg_ct = 0
while mode == 1:
    msg_start = ser.read()
    rx_buffer_0.pop(0)
    rx_buffer_0.append(msg_start)
    
    if rx_buffer_0[0] == b'@' and rx_buffer_0[1] == b'#' and rx_buffer_0[2] == b'@':
        print("---Rx start---")
        rx_msg = []
        while True:
            msg_end = ser.read()
            msg_ct += 1
            rx_buffer_1.pop(0)
            rx_buffer_1.append(msg_end)
            rx_msg.append(msg_end)
            #print(msg_end)
            
            if msg_ct % 10000 == 0 : print(msg_ct//1024,"kb")

            #for i in rx_msg[:-3]:
            #    rx_str += i
            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
                #print(rx_msg[:-3])
                #print(rx_msg[:3],"|",rx_msg[-6:-3])
                
                print("Msg len: ",len(rx_msg[:-3]))
                print(str(round(1/(time.time()-t),2)).rjust(5),"Chunks per second")
                t = time.time()
                print("---Rx end---")
                break
        break

rx_str =b""
print(rx_msg[:10])
for i in rx_msg[:-3]:
    rx_str += i
print(rx_str[:10])


fh = open("serial_jpg11.jpg","wb")
fh.write(rx_str)
fh.close()
