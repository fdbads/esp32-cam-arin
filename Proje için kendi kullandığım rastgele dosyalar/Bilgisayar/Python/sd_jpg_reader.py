"""
Read everything from Volume F
Get all the images in root
Copy them to computer and label the batch with a new index (prev max +1)
Convert them to array of images or video (depending on mode)

I can't remember or understand what "img_conv" is for
I guess to only move images to computer ???
"""
import time
import numpy as np
from PIL import Image,ImageShow
import os
import subprocess
from moviepy.editor import ImageSequenceClip
from natsort import natsorted

"""print(os.listdir("F:"))
print(len(os.listdir("F:")))

quit()
"""
#-------------------

#PARAMS---
frame_dims = 800,600
mode = 1
#MAIN----

def get_sorted_files_in_folder(folder_path):
    files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]
    return natsorted(files)


def copy_silent(source, destination):
    try:
        # Run the copy command in silent mode
        subprocess.run(['copy', source, destination], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, shell=True)
    except subprocess.CalledProcessError as e:
        # Handle any error if the copy command fails
        print(f"Error: {e}")


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
        all_pics_Dir_list_ = os.listdir(all_pics_Dir)
        all_pics_Dir_list_lst = []
        for i in all_pics_Dir_list_:
            if "_" in i:
                all_pics_Dir_list_lst.append(int(i.strip("_")))
        current_batch_index = max(all_pics_Dir_list_lst)                

    print(current_batch_index)
    new_Dir = os.path.join(all_pics_Dir,"_"+str(current_batch_index+1)+"_")
    current_batch = "_{}_".format(current_batch_index+1)
    os.mkdir(new_Dir)

    
    

    print("copy start")
    sd_len = len(os.listdir("F:"))
    sd_index = 0
    print("sd_len:",sd_len)
    for sd_file in os.listdir("F:"):
        sd_file_dir = os.path.join("F:",sd_file)
        command = "copy {} {}".format(sd_file_dir,new_Dir)
        
        if "System Volume Information" in command:
            continue
        #os.system(command)
        copy_silent(sd_file_dir,new_Dir)
        os.system("del /Q {}".format(sd_file_dir))
        if (100*sd_index/sd_len)%1 == 0: print("*",end="")
        sd_index+=1
    print("\ncopy end")

global all_pics_Dir
global img_conv
img_conv = False
all_pics_Dir = os.path.join(r"C:\Users\Arın\Desktop\KAREL\esp32-cam\cam_photos\sd_jpg")
current_batch = "_0_"

move_sd_to_pc()

batch_dir = os.path.join(all_pics_Dir,current_batch)
print("current_batch:",current_batch)


big_frame = 9,6
big_im_rgb888_arr = Image.new("RGB",(big_frame[0]*frame_dims[0],big_frame[1]*frame_dims[1]))

image_index = 0


if mode == 0:
    if not img_conv: quit()

    for jpg_file in os.listdir(batch_dir):
        #print("\n",image_index,"Starting",jpg_file)
        jpg_file_dir = os.path.join(batch_dir,jpg_file)
        
        im_jpg_arr = Image.open(jpg_file_dir)

        paste_x = (image_index % big_frame[0])*frame_dims[0]
        paste_y = (image_index //big_frame[0])*frame_dims[1]
        #print("pasting to",paste_x,paste_y)
        
        big_im_rgb888_arr.paste(im_jpg_arr,(paste_x,paste_y))

        image_index += 1

    print("Saving merged")
    big_im_rgb888_arr.save(batch_dir+"\_merged.png")
    print("Saving merged finished")
    #ImageShow.show(big_im_rgb888_arr)

elif mode == 1:
    
    def images_to_video(image_folder, output_video_path, fps=10): #10 for realtime
        image_files = [os.path.join(batch_dir,each_img) for each_img in get_sorted_files_in_folder(image_folder) if each_img.endswith(('.png', '.jpg', '.jpeg'))]

        clip = ImageSequenceClip([os.path.join(image_folder, img) for img in image_files], fps=fps)
        clip.write_videofile(output_video_path)

    # Example usage
    image_folder_path = batch_dir
    output_video_path = os.path.join(batch_dir,"output_video.mp4")

    print(output_video_path)
    images_to_video(image_folder_path, output_video_path)
