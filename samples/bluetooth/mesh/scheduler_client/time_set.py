from pytz import utc
import serial
from astropy.time import Time, TimeDelta
from datetime import datetime
tai_utc_delta = 37

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM10'
ser.open()

tai_base = Time('2000-01-01', scale='tai')
today = Time(datetime.today(), scale='tai')
dt = TimeDelta(today - tai_base, format='sec')

ser.write(f"mdl_time time-set {round(dt.value)} 0 0 {tai_utc_delta} 0 0\r\n".encode())