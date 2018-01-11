import numpy as np
import time
import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray 

class Test:
    def getIm(self):
        output = np.empty((1216*960*3), dtype = np.uint8)
        self.camera.capture(output, 'rgb')
        output = output.reshape(1216, 960, 3)
        output = output[:1200, :960, ]

        return output

    def getPoint(self, im):
        # Get the masked image
        im_hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
        # lower mask
        lowRed = np.array([0,50,50])
        uppRed = np.array([10,255,255])
        im_mask0 = cv2.inRange(im_hsv, lowRed, uppRed)
        # upper mask
        lowRed = np.array([170,50,50])
        uppRed = np.array([180,255,255])
        im_mask1 = cv2.inRange(im_hsv, lowRed, uppRed)
        # join the masks
        im_mask = im_mask0 + im_mask1
        # Get the non-zero part
        location = cv2.findNonZero(im_mask)
        if(location is None):
            return[-1, -1]
        pixelNum = len(location)
        x_ave = y_ave = 0
        for i in range(pixelNum):
            x_ave += location[i][0][0]
            y_ave += location[i][0][1]
        
        return [x_ave // pixelNum, y_ave // pixelNum]


t = Test()
while(true):
    im = t.getIm
    x, y = getPoint(im)
    print('(', x, ', ', y, ')', end = '')
    input