from bluepy import btle
from threading import Thread
import time
from random import randrange
import cv2
import imutils
#from importlib import import_module
#util = import_module('util')

x = 1.
y = 1.

#class trackCam:

class myStream:
    def __init__(self, src = 0):
        self.stream = cv2.VideoCapture(src)
        (self.grabbed, self.frame) = self.stream.read()
        self.stopped = False

    def start(self):
        Thread(target = self.update, args = ()).start()
        return self

    def update(self):
        while True:
            if self.stopped:
                return
            (self.grabbed, self.frame) = self.stream.read()

    def read(self):
        return self.frame

    def stop(self):
        self.stopped = True
	

def main():
    print('Connecting...')

    #dev = btle.Peripheral("F4:8C:50:AD:0D:A8")
    dev = btle.Peripheral("B8:27:EB:C3:E4:99")
    echo = dev.getCharacteristics()[-1]

    print('Connecting successfully!')

    #vs = myStream(src = 0).start()
    data = ''
    count = 0;
    while True:
        count += 5;
        for i in range(1):
            # similar to crop
            #frame = imutils.resize(vs.read(), width = 400)
            # do the image processing here
            # modified all the i (x,y) into a single string
            #data += str(randrange(1000)).zfill(3)
            #data += str(randrange(1000)).zfill(3)
            data += str(count%1000).zfill(3)
            data += str(count%1000).zfill(3)
        print(data)
        data_byte = str.encode(str(data))
        data = ''
        echo.write(data_byte, withResponse=False)
	
	echo.waitForNotifications(5.0)

        print('writing')
        print(time.time())
		

if __name__ == "__main__":
	main()
