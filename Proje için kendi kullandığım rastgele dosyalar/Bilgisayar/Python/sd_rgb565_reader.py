import time
import numpy as np
from PIL import Image,ImageShow
import os

"""print(os.listdir("F:"))
print(len(os.listdir("F:")))

quit()
"""
#-------------------

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

#PARAMS---
frame_dims = 1600,1200

#MAIN----
def move_sd_to_pc():
    global current_batch

    if len(os.listdir("F:")) > 1:
        global img_conv
        img_conv = True
        print("conv")
    else: 
        print("no conv")
        return

    if os.listdir(all_pics_Dir) == []:
        current_batch_index = -1
    else:
        current_batch = os.listdir(all_pics_Dir)[-1]
        current_batch_index = int((str(current_batch)[1:2]))
    new_Dir = os.path.join(all_pics_Dir,"_"+str(current_batch_index+1)+"_")
    os.mkdir(new_Dir)

    
    

    for sd_file in os.listdir("F:"):
        sd_file_dir = os.path.join("F:",sd_file)
        command = "copy {} {}".format(sd_file_dir,new_Dir)
        
        if "System Volume Information" in command:
            continue
        os.system(command)
        os.system("del /Q {}".format(sd_file_dir))


global all_pics_Dir
global img_conv
img_conv = False
all_pics_Dir = os.path.join(r"C:\Users\Arın\Desktop\KAREL\esp32-cam\cam_photos\sd_rgb565")
current_batch = "_0_"
move_sd_to_pc()

batch_dir = os.path.join(all_pics_Dir,current_batch)
print(current_batch)


big_frame = 7,7
big_im_rgb888_arr = Image.new("RGB",(7*frame_dims[0],7*frame_dims[1]))

image_index = 0

if not img_conv: quit()

for rgb_file in os.listdir(batch_dir):
    print(image_index,"Starting",rgb_file)
    rgb_file_dir = os.path.join(batch_dir,rgb_file)
    fh = open(rgb_file_dir,"rb")
    print("opening",rgb_file_dir)
    file_contents = fh.read()
    fh.close()

    pixels = []
    t = time.time()
    for i in range(len(file_contents)//2):
        
        two_byte_int = int.from_bytes(file_contents[2*i:2*i+2],byteorder="big", signed=False)
        pixels.append(rgb565_to_rgb888pixel(two_byte_int))
        #print(pixels[-1],type(pixels[-1]),type(pixels[-1][0]))

    print("took",round(time.time()-t,2),"seconds for rgb565 -> rgb888")
    #print(pixels[:3])


    pixels_2d = np.array(pixels,dtype=np.uint8)
    pixels_3d = np.reshape(pixels_2d,(frame_dims[1],frame_dims[0],3)) #numpy indexes => (y,x,color)
    #print_3d(pixels_3d[0:5,0:3,:])

    #im_rgb888_blank = Image.new("RGB",(dimsX,dimsY))

    im_rgb888_arr = Image.fromarray(pixels_3d)#,mode="RGB"
    
    paste_x = (image_index % big_frame[0])*1600
    paste_y = (image_index //big_frame[0])*1200
    print("pasting to",paste_x,paste_y)
    big_im_rgb888_arr.paste(im_rgb888_arr,(paste_x,paste_y))

    image_index += 1

print("Saving merged")
big_im_rgb888_arr.save(batch_dir+"\merged.png")
print("Saving merged finished")
ImageShow.show(big_im_rgb888_arr)

