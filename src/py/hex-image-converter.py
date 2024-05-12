
from ctypes import sizeof
import serial
import time

output = []
ser = serial.Serial("COM4", 115200)
time.sleep(2)
ser.write("i".encode())
while True:
    cc=str(ser.readline())

    if "Setup" not in cc:
        print(cc[2:][:-5])
        output.append([hex(int(x)) for x in cc[2:][:-5].split()])

    # this is the ending
    if ("255 217" in cc):
        break

print(output)

image_bytes = bytes([int(x, 16) for sublist in output for x in sublist])

# Write bytes to a JPEG file
with open('Nucleo_1.jpg', 'wb') as f:
   f.write(image_bytes)