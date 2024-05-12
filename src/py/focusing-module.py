import serial
import time
import os

def send_command(cmd):
    formatted_command = f'>{cmd}\r'
    ser.write(formatted_command.encode())
    time.sleep(0.1)
    response = ser.readline().decode().strip()
    print(response)



ser = serial.Serial("COM3", 115200, timeout=1)
time.sleep(1)

#while True:
#    send_command(input())

#setup
send_command("inform")
send_command("status")
send_command("home")
send_command("auto")

#Focussing Algorithm

#save position

ser.close()