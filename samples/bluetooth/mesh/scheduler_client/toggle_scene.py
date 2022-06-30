from time import sleep
import serial
from astropy.time import Time
from datetime import datetime

""" This script calculates and sets the local time of a Time Server via a shell client based on the computer's clock, 
then schedules periodic actions to alternate between scenes every 30 seconds"""

transition_time = 5000  # Transition time in ms
scene_no1 = 1  # Scene number to recall
scene_no2 = 1  # Scene number to recall

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM6'
ser.open()

mesh_tai_base = Time('2000-01-01', scale='tai')
today = Time(datetime.today(), scale='utc')
tai_utc_delta = round(today.unix_tai - today.unix)
mesh_tai = today.unix_tai - mesh_tai_base.unix_tai

ser.write(f"mdl_time time-set {round(mesh_tai)} 0 0 {tai_utc_delta} 0 0\r\n".
          encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 00 0x7F 2 {transition_time} {scene_no1}\r\n"
    .encode())
ser.write(f"mdl_sched action-set 0\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 30 0x7F 2 {transition_time} {scene_no2}\r\n"
    .encode())
ser.write(f"mdl_sched action-set 1\r\n".encode())