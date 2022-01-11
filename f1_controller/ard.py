import pyfirmata
import pyvjoy
import time

j = pyvjoy.VJoyDevice(1)
board = pyfirmata.Arduino('COM3')
it = pyfirmata.util.Iterator(board)
it.start()

analog_input = board.get_pin('a:1:i')
analog_input2 = board.get_pin('a:0:i')

while True:
    try:
        value = analog_input.read()
        value= (5 - value*5)*(32600.0/5.0) 
        val = int(value)
        j.set_axis(pyvjoy.HID_USAGE_X,val)
        print(value)
        speed = analog_input2.read()
        if (speed*5>2.0):
            j.set_button(1,1)
            print("speed")
        else:
            j.set_button(1,0)
        #time.sleep(0.1)
    except:
        print('none')
