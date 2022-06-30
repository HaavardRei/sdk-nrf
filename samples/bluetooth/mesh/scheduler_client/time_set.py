import serial
from astropy.time import Time
from datetime import datetime
""" This script calculates and sets the local time of a Time Server via 
a shell client based on the computer's clock"""

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
