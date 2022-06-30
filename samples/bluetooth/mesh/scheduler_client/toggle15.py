from time import sleep
import serial
from astropy.time import Time
from datetime import datetime
""" This script calculates and sets the local time of a Time Server via a 
shell client based on the computer's clock, then schedules periodic actions 
to toggle between "ON" and "OFF" every 15 seconds """

transition_time = 5000  # Transition time in ms

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
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 00 0x7F 1 {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 0\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 15 0x7F 0 {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 1\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 30 0x7F 1 {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 2\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 45 0x7F 0 {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 3\r\n".encode())