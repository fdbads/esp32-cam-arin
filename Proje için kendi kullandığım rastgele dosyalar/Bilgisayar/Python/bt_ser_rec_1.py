import serial

mode = 1

ser = serial.Serial("COM12")
print(ser)


while mode == 0:
    print(ser.read())

rx_buffer_0 = [b'\x00',b'\x00',b'\x00']
rx_buffer_1 = [b'\x00',b'\x00',b'\x00']

"""
@#@ start
#@# end
"""
while mode == 1:
    msg_start = ser.read()
    rx_buffer_0.pop(0)
    rx_buffer_0.append(msg_start)
    
    if rx_buffer_0[0] == b'@' and rx_buffer_0[1] == b'#' and rx_buffer_0[2] == b'@':
        #print("---Rx start---")

        rx_msg = []
        while True:
            msg_end = ser.read()
            rx_buffer_1.pop(0)
            rx_buffer_1.append(msg_end)
            rx_msg.append(msg_end)
            #print(msg_end,end=" ")

            if rx_buffer_1[0] == b'#' and rx_buffer_1[1] == b'@' and rx_buffer_1[2] == b'#':
            
                #print("\n---Rx end---")
                break
    
        rx_msg = rx_msg[:-3]

        rx_str = b""

        for i in rx_msg:
            rx_str += i

        print(rx_str)
        

#print(rx_msg)



