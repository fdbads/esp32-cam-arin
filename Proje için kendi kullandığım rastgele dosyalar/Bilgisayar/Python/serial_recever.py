import time
import serial

ser = serial.Serial("COM8",9600)
print(ser)

while True:
    print(ser.read())