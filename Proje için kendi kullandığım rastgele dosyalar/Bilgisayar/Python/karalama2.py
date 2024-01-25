# print(rgb565_to_rgb888pixel(0xffff))
# (248, 252, 248)
import time
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

dimsX,dimsY = 256,256
im_rgb888_blank = Image.new("RGB",(dimsX*3,dimsY))
im_rgb565_blank = Image.new("RGB",(dimsX*3,dimsY))

pixels_2d = np.zeros((256*256,3),dtype=np.uint8)
pixels_3d = np.reshape(pixels_2d,(dimsY,dimsX,3)) #numpy indexes => (y,x,color)
#print_3d(pixels_3d[0:5,0:3,:])

for i in range(3):
    for y in range(np.shape(pixels_3d)[0]):
        for x in range(np.shape(pixels_3d)[1]):
        
            if   i == 0:
                pixels_3d[y,x,0] = x
                pixels_3d[y,x,1] = y
                pixels_3d[y,x,2] = 0
            elif i == 1:
                pixels_3d[y,x,0] = x
                pixels_3d[y,x,1] = 0
                pixels_3d[y,x,2] = y
            elif i == 2:
                pixels_3d[y,x,0] = 0
                pixels_3d[y,x,1] = x
                pixels_3d[y,x,2] = y

    im_rgb888_blank.paste(Image.fromarray(pixels_3d), (dimsX*i, 0))


#im_rgb888_arr = Image.fromarray(pixels_3d)#,mode="RGB"
#im_rgb888_blank.paste(Image.fromarray(pixels_3d), (0, 0))




ImageShow.show(im_rgb888_blank)