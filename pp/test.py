import numpy as np
import time
import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray 

camera = PiCamera()
camera.resolution = (1216, 960)

output = np.empty((1216*960*3), dtype = np.uint8)
camera.capture(output, 'bgr')
output = output.reshape(960,1216, 3)
output = output[:, :1200, ]

im = 255 - output
x = y = 0

# Get the masked image
im_hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)


lowRed = np.array([70,90,50])
uppRed = np.array([100,255,255])
im_mask = cv2.inRange(im_hsv, lowRed, uppRed)
'''
# lower mask
lowRed = np.array([0,100,100])
uppRed = np.array([10,255,255])
im_mask0 = cv2.inRange(im_hsv, lowRed, uppRed)
# upper mask
lowRed = np.array([160,100,100])
uppRed = np.array([180,255,255])
im_mask1 = cv2.inRange(im_hsv, lowRed, uppRed)
# join the masks
im_mask = im_mask0 + im_mask1
'''
# Get the non-zero part
location = cv2.findNonZero(im_mask)
if(location is None):
    x = y = -1
else:
    pixelNum = len(location)
    x_ave = y_ave = 0
    for i in range(pixelNum):
        x_ave += location[i][0][0]
        y_ave += location[i][0][1]
    x = x_ave // pixelNum
    y = y_ave // pixelNum

print('(', x, ', ', y, ')')

cv2.imwrite('im.jpg', im)
cv2.imwrite('im_hsv.jpg', im_hsv)
cv2.imwrite('mask.jpg', im_mask)












