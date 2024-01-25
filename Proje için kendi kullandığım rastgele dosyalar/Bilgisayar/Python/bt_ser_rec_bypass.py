import time
import serial
t = time.time()

mode = 1

ser = serial.Serial("COM12")
print(ser)


while mode == 0:
    print(ser.read())

rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
rx_buffer_1 = [b'\x00',b'\x00',b'\x00']
rx_msg = []
"""
@#@ start
#@# end
"""
while mode == 1:
    msg_start = ser.read()
    rx_buffer_0.pop(0)
    rx_buffer_0.append(msg_start)
    
    if rx_buffer_0[0] == b'@' and rx_buffer_0[1] == b'#' and rx_buffer_0[2] == b'@':
        print("---Rx start---")
        rx_msg = []
        while True:
            msg_end = ser.read()
            rx_buffer_1.pop(0)
            rx_buffer_1.append(msg_end)
            rx_msg.append(msg_end)
            #rx_str =b""
            
            #for i in rx_msg[:-3]:
            #    rx_str += i
            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
                print(rx_msg[:-3])
                #print(rx_msg[:3],"|",rx_msg[-6:-3])
                
                print("Msg len: ",len(rx_msg[:-3]))
                print(str(round(1/(time.time()-t),2)).rjust(5),"Chunks per second")
                t = time.time()
                print("---Rx end---")
                break

while mode == 2:
    msg_start = ser.read()
    rx_buffer_0.pop(0)
    rx_buffer_0.append(msg_start)
    
    if rx_buffer_0[0] == b'@' and rx_buffer_0[1] == b'#' and rx_buffer_0[2] == b'@':
        print("---Rx start---")
        rx_msg = []
        while True:
            msg_end = ser.read()
            rx_buffer_1.pop(0)
            rx_buffer_1.append(msg_end)
            rx_msg.append(msg_end)
            #print(msg_end,end=" ")
            rx_str =b""
            for i in rx_msg[:-3]:
                rx_str += i
            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
                try:
                    print(rx_str)
                    print(int.from_bytes(rx_str,byteorder="little",signed=False))
                    #print(int.from_bytes(rx_str,byteorder="big",signed=False))
                    
                except:print("-*-*-")
                
                print("---Rx end---")
                break
    
        

#print(rx_msg)

rx_msg = rx_msg[:-3]



