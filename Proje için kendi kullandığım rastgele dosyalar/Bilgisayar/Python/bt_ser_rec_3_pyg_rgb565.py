import time
import serial
import numpy as np
from PIL import Image,ImageShow


def bit_expand(input_val,in_bit,out_bit=8):
    return round(input_val/(2**in_bit-1)*(2**out_bit-1))
        

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

    r_888 = bit_expand(r_565,5)
    g_888 = bit_expand(g_565,5)
    b_888 = bit_expand(b_565,5)

    return (r_888,g_888,b_888)

ser = serial.Serial("COM8",115200)
print(ser)
ser.setDTR(False)
ser.setRTS(False)

for i in range(10):
    time.sleep(0.2)
    print(i,end=" ")
print()    
mode = 1

ser.write(b"x")

handshake = int.from_bytes(ser.read(),signed=False)

print(handshake)
#print(bin(handshake))

while mode == 0:    
    print(ser.read())


rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
rx_buffer_1 = [b'\x00',b'\x00',b'\x00']
rx_msg = []
"""
@#@ start
#@# end
"""
msg_ct = 0

frame_dims = [b"",b""] #0 -> x, 1 -> y

t1 = time.time()
t2 = time.time()
while mode == 1:
    msg_start = ser.read()
    rx_buffer_0.pop(0)
    rx_buffer_0.append(msg_start)
    
    if rx_buffer_0[0] == b'@' and rx_buffer_0[1] == b'#' and rx_buffer_0[2] == b'@':
        print("---Rx start---")
        frame_dims_bytes = [b"",b""] #0 -> x, 1 -> y

        frame_dims_bytes[0] = ser.read()+ser.read()+ser.read()+ser.read()
        frame_dims_bytes[1] = ser.read()+ser.read()+ser.read()+ser.read()
        
        img_fmt_byte = ser.read()

        frame_dims = [int.from_bytes(frame_dims_bytes[0],"little",signed=False),
                      int.from_bytes(frame_dims_bytes[1],"little",signed=False)]
        

        if   int.from_bytes(img_fmt_byte,signed=False) == 15: img_fmt = "RGB565"
        elif int.from_bytes(img_fmt_byte,signed=False) == 25: img_fmt = "OTHER"
        else:                                                 img_fmt = "ERROR"
        print("frame_dims:",frame_dims,"| img_fmt:",img_fmt)
    
        packet_expected_size = frame_dims[0]*frame_dims[0]*2

        rx_str =b""
        while True:
            msg_end = ser.read()
            msg_ct += 1
            rx_buffer_1.pop(0)
            rx_buffer_1.append(msg_end)
            #rx_msg.append(msg_end)
            rx_str += msg_end
            
            if msg_ct % (10*(2**10)) == 0 :
                print(str(msg_ct//(2**20)).rjust(2),"mb",end=" ")
                print(str((msg_ct%(2**20))//(2**10)).rjust(5),"kb",end=" ")
                data_rate = round((10)/(time.time()-t2),2)
                print(str(data_rate).rjust(5),"KByte/s" ,end=" ")
                print("%{} done".format(str(round(msg_ct*100/packet_expected_size,2)).ljust(5,"0")),end=" ")
                print(str(round((packet_expected_size-msg_ct)/(1024*data_rate))).rjust(3),"seconds left")
                t2 = time.time()

            
            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
                rx_str = rx_str[:-3]
                print("Msg len: ",len(rx_str))
                print("---Rx end---")
                break
        break
print()
print(str(round(len(rx_str)/(time.time()-t1),2)).rjust(5),"Bytes/s")
print()

print(rx_str[:10])

file_contents = rx_str

pixels = []
t = time.time()
for i in range(len(file_contents)//2):
    
    two_byte_int = int.from_bytes(file_contents[2*i:2*i+2],byteorder="big", signed=False)
    pixels.append(rgb565_to_rgb888pixel(two_byte_int))
    #print(pixels[-1],type(pixels[-1]),type(pixels[-1][0]))

print("took",round(time.time()-t,2),"seconds for rgb565 -> rgb888\n")
#print(pixels[:3])


pixels_2d = np.array(pixels,dtype=np.uint8)
pixels_3d = np.reshape(pixels_2d,(frame_dims[0],frame_dims[0],3)) #numpy indexes => (y,x,color)
#print_3d(pixels_3d[0:5,0:3,:])

#im_rgb888_blank = Image.new("RGB",(dimsX,dimsY))

im_rgb888_arr = Image.fromarray(pixels_3d)#,mode="RGB"
im_rgb888_arr.save("dora_dg.jpeg","JPEG")
#im_rgb888_blank.show()
ImageShow.show(im_rgb888_arr)