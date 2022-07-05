""" This script sets up two scenes that are stored in the Scene Register. """

from time import sleep
import serial

scene_no1 = 1            # Scene number to store
scene_no2 = 2            # Scene number to store

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM6'        # Serial port of client DK
ser.open()
ser.write(f"mdl_ctrl mode-set 1\r\n".encode())
sleep(0.3)
ser.write(f"mdl_ctrl light-onoff-set 1\r\n".encode())
sleep(0.3)
ser.write(f"mdl_scene store {scene_no1}\r\n".encode())
sleep(1)
ser.write(f"mdl_ctrl light-onoff-set 1\r\n".encode())
sleep(0.3)
ser.write(f"mdl_scene store {scene_no2}\r\n".encode())
ser.close()
