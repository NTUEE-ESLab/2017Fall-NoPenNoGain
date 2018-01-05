import numpy as np
import time
import cv2
from picamera import PiCamera

class TrackCam:
   def __init__(self):
      self.camera = PiCamera()
      self.rawCapture = PiRGBArray(self.camera)
      self.scene = self.setScene()
      self.vertex = self.setVertex()

   def getPoint(self):
      # Get the masked image
      self.camera.capture(self.rawCapture, format="bgr")
      im = rawCapture.array
      im_hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
      # lower mask
      lowRed = np.array([0,50,50])
      uppRed = np.array([10,255,255])
      im_mask0 = cv2.inRange(img_hsv, lowRed, uppRed)
      # upper mask
      lowRed = np.array([170,50,50])
      uppRed = np.array([180,255,255])
      im_mask1 = cv2.inRange(img_hsv, lowRed, uppRed)
      # join the masks
      im_mask = im_mask0 + im_mask1
      # Get the non-zero part
      location = cvs.findNonZero(im_mask)
      pixelNum = len(location)
      # If no red part is detected
      if(pixelNum == 0):
         return[-1, -1]
         # If red part is detected
      x_ave = y_ave = 0
      for i in range(pixelNum):
         x_ave += location[i][0]
         y_ave += location[i][1]
      return [x_ave / pixelNum, y_ave / pixelNum]

   def setScene(self):
      self.camera.capture(rawCapture, format="bgr")
      im = self.rawCapture.array
      cv2.imwrite('./SceneIm.jpg', im)
      input()
      if(not self.getPoint() == [-1, -1]):
         print('Dirty scene!')
         return None
      cv2.imwrite('./SceneIm.jpg', im)
      return im

   def setVertex(self):
      for i in range(4):
         end = False
      while(not end):
         print('Getting vertex ', i)
         time.sleep(5)
         x, y = getPoint()
      return None


def main():
   trackCam = TrackCam()


if(__name__ == '__main__'):
   main()
