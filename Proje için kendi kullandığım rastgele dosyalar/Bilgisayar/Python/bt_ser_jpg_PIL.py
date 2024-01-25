import time
import serial
from PIL import Image,ImageFilter,ImageEnhance

mode = 1 #mode 2 is local photo
processing = False

if (mode!=2):
    print("Taking new photo")
    t = time.time()

    ser = serial.Serial("COM8",115200)
    print(ser)
    ser.setDTR(False)
    ser.setRTS(False)

    for i in range(10):
        time.sleep(0.2)
        print(i,end=" ")




    ser.write(b"x")

    while mode == 0:    
        print(ser.read())




    #while mode == 0:

    rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
    rx_buffer_1 = [b'\x00',b'\x00',b'\x00']
    rx_msg = []

    # @#@ start
    # #@# end

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
                    
                    print("Msg len: ",len(rx_msg[:-3])//1024,"kb")
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

    save_numb = 1
    fh = open("serial_jpg_long_e_{}.jpg".format(save_numb),"wb")
    fh.write(rx_str)
    fh.close()
else:
    print("Using old photo")


if processing:
    print("Processing")
    t=time.time()
    r_factor,g_factor,b_factor = 10,0.5,20

    pre_im = Image.open("serial_jpg_long_e_{}.jpg".format(save_numb))

    width_pre, height_pre = pre_im.size
    joined_im = Image.new("RGB", (width_pre*2, height_pre))
    joined_im.paste(pre_im, (0, 0))

    for y in range(height_pre):
            for x in range(width_pre):
                # Get the current RGB values of the pixel
                r, g, b = pre_im.getpixel((x, y))

                # Adjust the RGB channel values using the specified factors
                r = int(r * r_factor)
                g = int(g * g_factor)
                b = int(b * b_factor)

                # Set the new RGB values for the pixel
                pre_im.putpixel((x, y), (r, g, b))

    #pre_im is now post_im but not renamed

    post_im = pre_im.filter(ImageFilter.GaussianBlur(3))
    joined_im.paste(post_im, (width_pre, 0))

    for _ in range(5):
            post_im = post_im.filter(ImageFilter.SHARPEN)

    #joined_im.paste(post_im, (width_pre*2, 0))
    joined_im.save("serial_jpg_long_e_{}_cc.jpg".format(save_numb),"JPEG",quality=100)

    print("Proccessing took",round(time.time()-t,2),"seconds")

