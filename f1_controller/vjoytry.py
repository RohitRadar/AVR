import pyvjoy
from time import sleep
j = pyvjoy.VJoyDevice(1)

"""acc:1
brk:2
lef:3
right:4
pas:5
clt:6
hex: left:1,right:8000
"""

"""
sleep(4)
j.set_button(1,1)
sleep(2)
j.set_button(1,0)
"""

"""
sleep(4)
j.set_axis(pyvjoy.HID_USAGE_X,16383)
for i in range(16383,32767):
    j.set_axis(pyvjoy.HID_USAGE_X,i)
for i in range(0,16383):
    j.set_axis(pyvjoy.HID_USAGE_X,32767-i)
"""

sleep(4)
j.set_axis(pyvjoy.HID_USAGE_X,20000)    
sleep(4)
j.set_axis(pyvjoy.HID_USAGE_X,16383)    

