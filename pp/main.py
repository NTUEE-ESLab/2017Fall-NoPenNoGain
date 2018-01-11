import numpy as np
import time
import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray

class TrackCam:
    def __init__(self):
        self.camera = PiCamera()
        self.camera.resolution = (1200, 960)
        self.rawCapture = PiRGBArray(self.camera, size = (1216,960))
        self.scene = self.setScene()
        self.vertex = self.setVertex()
        self.x_start, self.y_start, self.width, self.height = self.setBound()
        self.a = self.b = self.c = self.d = 0
        self.I1, self.I2 = self.setIdealPoint()

    def getIm(self):
        self.camera.capture(self.rawCapture, format="bgr")
        im = self.rawCapture.array

        return im

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
        pixelNum = len(location)
        # If no red part is detected
        if(pixelNum == 0):
            return[-1, -1]
            # If red part is detected
        x_ave = y_ave = 0
        print(location[0])
        input
        for i in range(pixelNum):
            x_ave += location[i][0]
            y_ave += location[i][1]
        
        return [x_ave // pixelNum, y_ave // pixelNum]

    def setScene(self):
        im = self.getIm()
        cv2.imwrite('./SceneIm.jpg', im)
        if(not self.getPoint(im) == [-1, -1]):
            print('Dirty scene!')
            return None
        cv2.imwrite('./sceneIm.jpg', im)
        
        return im

    def setVertex(self):
        ver = []
        for i in range(4):
            again = True
            while(again):
                print('Getting vertex ', i)
                time.sleep(5)
                im = self.getIm()
                x, y = self.getPoint(im)
                if(not x == -1):
                    ver.append([x, y])
                    print('Get vertex ', i, 'as (', x, ', ', y, ')')
                    again = False
        
        return ver

    def drawEdge(self):
        edgeIm = self.scene
        for i in range(4):
            cv2.line(edgeIm, (self.vertex[i%4][0], self.vertex[i%4][1]), (self.vertex[(i+1)%4][0], self.vertex[(i+1)%4][1]), (0, 255, 0), 2)
        cv2.imwrite('./edgeIm.jpg', edgeIm)

        return None

    def setBound(self):
        x_start = min(self.vertex[0][0], self.vertex[3][0])
        y_start = min(self.vertex[0][1], self.vertex[1][1])
        width  = max(self.vertex[1][0], self.vertex[2][0]) - x_start
        height = max(self.vertex[3][1], self.vertex[2][1]) - y_start

        for i in range(4):
            self.vertex[i][0] -= x_start
            self.vertex[i][1] -= y_start

        return [x_start, y_start, width, height]

    def setIdealPoint(self):
        I1 = I2 = [0, 0]
        para1 = para2 = False
        print('Are vertical bounds parallel: ')
        if((self.vertex[3][0]-self.vertex[0][0])*(self.vertex[2][1]-self.vertex[1][1]) == (self.vertex[2][0]-self.vertex[1][0])*(self.vertex[3][1]-self.vertex[0][1])):
            para1 = True
            print('Yes')
        else:
            print('No')
        print('Are horizontal bounds parallel: ')
        if((self.vertex[1][0]-self.vertex[0][0])*(self.vertex[2][1]-self.vertex[3][1]) == (self.vertex[2][0]-self.vertex[3][0])*(self.vertex[1][1]-self.vertex[0][1])):
            para1 = True
            print('Yes')
        else:
            print('No')
        if(para1 == False):
            self.a = (self.vertex[1][1]-self.vertex[0][1]) / (self.vertex[1][0]-self.vertex[0][0])
            self.b = - self.a * self.vertex[0][0] + self.vertex[0][1]
            e = (self.vertex[2][1]-self.vertex[3][1]) / (self.vertex[2][0]-self.vertex[3][0])
            f = - e * self.vertex[3][0] + self.vertex[3][1]

            I1[0] = - (self.b - f) / (self.a - e)
            I1[1] = I1[0] * self.a + self.b

            print('Ideal point 1 at (', I1[0], ', ', I1[1], ')')
        else:
            print('No ideal point 1')
        if(para2 == False):
            self.c = (self.vertex[3][1]-self.vertex[0][1]) / (self.vertex[3][0]-self.vertex[0][0])
            self.d = - self.c * self.vertex[3][0] + self.vertex[3][1]
            g = (self.vertex[2][1]-self.vertex[1][1]) / (self.vertex[2][0]-self.vertex[1][0])
            h = - g * self.vertex[2][0] + self.vertex[2][1]

            I2[0] = - (self.d - h) / (self.c - g)
            I2[1] = I2[0] * self.c + self.d

            print('Ideal point 1 at (', I2[0], ', ', I2[1], ')')
        else:
            print('No ideal point 1')

        return I1, I2

    def transformation(self, x_T, y_T):
        x_E = x_F = 0
        if(para1 == True):
            r = - self.c * x_T + y_T
            x_E = - (self.b - r) / (self.a - self.c)
        else:
            g = (self.I2[1] - y_T) / (self.I2[0] - x_T)
            h = - g * x_T + y_T
            x_E = - (self.b - h) / (self.a - g)
        if(para2 == True):
            r = - self.a * x_T + y_T
            x_F = - (self.d - r) / (self.c - self.a)
        else:
            e = (I1[1] - y_T) / (I1[0] - x_T)
            f = - e * x_T + y_T
            x_F = - (self.d - f) / (self.c - e)

        k = ((self.I1[0]-self.vertex[0][0]) * (self.vertex[1][0]-x_E)) / ((self.vertex[1][0]-self.vertex[0][0]) * (self.I1[0] - x_E))
        l = ((self.I2[0]-self.vertex[0][0]) * (self.vertex[3][0]-x_F)) / ((self.vertex[3][0]-self.vertex[0][0]) * (self.I2[0] - x_F))

        x_trans = (1 - k) * self.width // 1
        y_trans = (1 - l) * self.height // 1

        return [x_trans, y_trans]

    def track(self):
        while(True):
            im = getIm()
            im = im[x_start : (x_start + width), y_start : (y_start + height)]
            x_ave, y_ave = self.getPoint(im)
            x_trans, y_trans = self.transformation(x_ave, y_ave)

            if(x_ave == -1):
                print('(---, ---) | (---, ---)')
            else:
                print('(', x_ave, ', ', y_ave, ') | (', x_trans, ', ', y_trans, ')')



def main():
   trackCam = TrackCam()
   trackCam.track()


if(__name__ == '__main__'):
   main()
