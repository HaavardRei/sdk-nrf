""" This script calculates and sets the local time of a Time Server via a
shell client based on the computer's clock, then schedules periodic actions
to toggle between "ON" and "OFF" every 15 seconds """

from time import sleep
import serial
from time_set import time_set

transition_time = 2000  # Transition time in ms
action_on = 1
action_off = 0

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM' + input('Enter shell client COM port: ')

ser.open()

time_set(ser,2)

sleep(2)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 00 0x7F {action_on} {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 0\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 15 0x7F {action_off} {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 1\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 30 0x7F {action_on} {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 2\r\n".encode())
sleep(0.4)
ser.write(
    f"mdl_sched action-ctx-set 0x64 0xFFF 0 24 60 45 0x7F {action_off} {transition_time} 0\r\n"
    .encode())
ser.write(f"mdl_sched action-set 3\r\n".encode())
ser.close()
