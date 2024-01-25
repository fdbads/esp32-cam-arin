# print(rgb565_to_rgb888pixel(0xffff))
# (248, 252, 248)
import time
import numpy as np
from PIL import Image,ImageShow

dimsX,dimsY = 1600,1200
dimsX,dimsY = 320,240

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

def print_3d(ar):
    shape = ar.shape
    
    for y in range(shape[0]):
        print("|",end="")
        for x in range(shape[1]):
            print(" (",end="")
            for color in range(shape[2]):
                print(str(ar[y,x,color]).rjust(3,),end="")
                if color+1<shape[2]:print(",",end="")
            print(") |",end="")
        print()    

fh = open("serial_rgb565.txt","rb")
file_contents = fh.read()
fh.close()

#file_contents=b'\xff'+b'\xff'+b'\x00'+b'\xff'
pixels = []
t = time.time()
for i in range(len(file_contents)//2):
    
    two_byte_int = int.from_bytes(file_contents[2*i:2*i+2],byteorder="big", signed=False)
    pixels.append(rgb565_to_rgb888pixel(two_byte_int))
    #print(pixels[-1],type(pixels[-1]),type(pixels[-1][0]))

print("took",round(time.time()-t,2),"seconds for rgb565 -> rgb888\n")
#print(pixels[:3])


pixels_2d = np.array(pixels,dtype=np.uint8)
pixels_3d = np.reshape(pixels_2d,(dimsY,dimsX,3)) #numpy indexes => (y,x,color)
#print_3d(pixels_3d[0:5,0:3,:])

im_rgb888_blank = Image.new("RGB",(dimsX,dimsY))

im_rgb888_arr = Image.fromarray(pixels_3d)#,mode="RGB"

#im_rgb888_blank.show()
ImageShow.show(im_rgb888_arr)