""" This script calculates and sets the local time of a Time Server via a shell client based on
the computer's clock, then schedules periodic actions to alternate scenes every 30 seconds"""

from time import sleep
import serial
from time_set import time_set

transition_time = 5000   # Transition time in ms
scene_no1 = 1            # Scene number to recall
scene_no2 = 2            # Scene number to recall

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM' + input('Enter shell client COM port: ')
ser.open()
time_set(ser, 2)
sleep(2)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 00 0x7F 2 {transition_time} {scene_no1}\r\n"
    .encode())
ser.write(f"mdl_sched action-set 0\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 30 0x7F 2 {transition_time} {scene_no2}\r\n"
    .encode())
ser.write(f"mdl_sched action-set 1\r\n".encode())
ser.close()
