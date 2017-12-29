from bluepy import btle
import thread
from importlib import import_module
util = import_module('util')

x = 1.
y = 1.

def opencv():
	thread.start_new_thread( print_time, ("Thread-1", 2, ) )
	

def main():
	print('Connecting...')

	dev = btle.Peripheral("b8:27:eb:c3:e4:99")
	

	while True:
		ha = x * 100
		ha_byte = str.encode(ha)
		dev.getCharacteristics()[-1].write(ha_byte, withResponse=True)

		

if __name__ == "__main__":
	main()
