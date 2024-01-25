import time
import serial

# print(rgb565_to_rgb888pixel(0xffff))
# (248, 252, 248)
def rgb565_to_rgb888pixel(two_bytes_int):

    mask5 = 0b00011111
    mask6 = 0b00111111
    
    #.rjust(8,"0")
    #print("X", two_bytes_int_str[ 0: 8],end=" ")
    
    r_565 = (two_bytes_int >> 11) & mask5
    g_565 = (two_bytes_int >>  5) & mask6
    b_565 = (two_bytes_int >>  0) & mask5
    
    #print("\nr", bin(r_565)[2:].rjust(8,"0"))
    #print(  "g", bin(g_565)[2:].rjust(8,"0"))
    #print(  "b", bin(b_565)[2:].rjust(8,"0"))

    r_888 = r_565*(2**3)
    g_888 = g_565*(2**2)
    b_888 = b_565*(2**3)

    return (r_888,g_888,b_888)

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


packet_expected_size = (320*240*2)

#while mode == 0:

rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
rx_buffer_1 = [b'\x00',b'\x00',b'\x00']
rx_msg = []
"""
@#@ start
#@# end
"""
msg_ct = 0

t1 = time.time()
t2 = time.time()
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
            
            if msg_ct % (10*(2**10)) == 0 :
                print(str(msg_ct//(2**20)).rjust(2),"mb",end=" ")
                print(str((msg_ct%(2**20))//(2**10)).rjust(5),"kb",end=" ")
                data_rate = round((10)/(time.time()-t2),2)
                print(str(data_rate).rjust(5),"KByte/s" ,end=" ")
                print("%{} done".format(str(round(msg_ct*100/packet_expected_size,2)).ljust(5,"0")),end=" ")
                print(str(round((packet_expected_size-msg_ct)/(1024*data_rate))).rjust(3),"seconds left")
                t2 = time.time()

            #for i in rx_msg[:-3]:
            #    rx_str += i
            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
                #print(rx_msg[:-3])
                #print(rx_msg[:3],"|",rx_msg[-6:-3])
                
                print("Msg len: ",len(rx_msg[:-3]))
                #print(str(round(1/(time.time()-t),2)).rjust(5),"Chunks per second")
                
                #t = time.time()
                print("---Rx end---")
                break
        break
print()
print(str(round(len(rx_msg[:-3])/(time.time()-t1),2)).rjust(5),"Bytes/s")
print()

rx_str =b""
print(rx_msg[:10])

fh = open("serial_rgb565.txt","wb")
for i in rx_msg[:-3]:
    fh.write(i)
    #rx_str += i
#print(rx_str[:10])
fh.close()

for i in range(0):

    two_bytes_int  = int.from_bytes(rx_msg[i*2  ],signed=False) << 8
    two_bytes_int += int.from_bytes(rx_msg[i*2+1],signed=False)

    print(str(two_bytes_int).rjust(5),end="|")
    print(rgb565_to_rgb888pixel(two_bytes_int))
    """
    print_str  = bin(int.from_bytes(rx_msg[i*2  ],signed=False))[2:].rjust(8,"0")
    print_str += bin(int.from_bytes(rx_msg[i*2+1],signed=False))[2:].rjust(8,"0")
    print(print_str[0:5],end=" ")
    print_str = print_str[6:]
    print(print_str[0:6],end=" ")
    print_str = print_str[5:]
    print(print_str)
    """
    




"""
"""