import numpy as np
#import time
#import cv2
#from picamera import PiCamera
#from picamera.array import PiRGBArray

m = 0.00000001
M = 100000000

class TrackCam:
    def __init__(self):
        #self.idx = 0
        #self.camera = PiCamera()
        #self.camera.resolution = (1216, 960)
        #self.scene = self.setScene()
        #self.vertex = self.setVertex()
        self.vertex = [[100, 100], [200, 99], [201, 200], [100, 199]]
        #self.drawEdge()
        self.x_start, self.y_start, self.width, self.height = self.setBound()
        self.a = self.b = self.c = self.d = 0
        self.para1 = self.para2 = False
        self.I1, self.I2 = self.setIdealPoint()

    def setBound(self):
        x_start = min(self.vertex[0][0], self.vertex[3][0])
        y_start = min(self.vertex[0][1], self.vertex[1][1])
        width  = max(self.vertex[1][0], self.vertex[2][0]) - x_start
        height = max(self.vertex[3][1], self.vertex[2][1]) - y_start

        for i in range(4):
            self.vertex[i][0] -= x_start
            self.vertex[i][1] -= y_start

        print('Origin at (', x_start, ', ', y_start, ')')
        print('  width = ', width)
        print('  height = ', height)
        print('  new vertex0: (', self.vertex[0][0], ', ', self.vertex[0][1], ')')
        print('  new vertex1: (', self.vertex[1][0], ', ', self.vertex[1][1], ')')
        print('  new vertex2: (', self.vertex[2][0], ', ', self.vertex[2][1], ')')
        print('  new vertex3: (', self.vertex[3][0], ', ', self.vertex[3][1], ')')

        #boundIm = self.scene
        #cv2.line(boundIm, (x_start, y_start), (x_start+width, y_start), (0, 255, 0), 2)
        #cv2.line(boundIm, (x_start+width, y_start), (x_start+width, y_start+height), (0, 255, 0), 2)
        #cv2.line(boundIm, (x_start+width, y_start+height), (x_start, y_start+height), (0, 255, 0), 2)
        #cv2.line(boundIm, (x_start, y_start+height), (x_start, y_start), (0, 255, 0), 2)
        #cv2.imwrite('./boundIm.jpg', boundIm)

        return [x_start, y_start, width, height]

    def setIdealPoint(self):
        I1 = [0, 0]
        I2 = [0, 0]
        print('Are vertical bounds parallel:', end = ' ')
        if((self.vertex[3][0]-self.vertex[0][0])*(self.vertex[2][1]-self.vertex[1][1]) == (self.vertex[2][0]-self.vertex[1][0])*(self.vertex[3][1]-self.vertex[0][1])):
            self.para1 = True
            print('Yes')
        else:
            print('No')
        print('Are horizontal bounds parallel:', end = ' ')
        if((self.vertex[1][0]-self.vertex[0][0])*(self.vertex[2][1]-self.vertex[3][1]) == (self.vertex[2][0]-self.vertex[3][0])*(self.vertex[1][1]-self.vertex[0][1])):
            self.para2 = True
            print('Yes')
        else:
            print('No')

        if(not self.para1):
            if(self.vertex[0][0] == self.vertex[3][0]):
                I1[0] = self.vertex[0][0]
                I1[1] = self.vertex[1][1] + (self.vertex[0][0]-self.vertex[1][0])*(self.vertex[1][1]-self.vertex[2][1])/(self.vertex[1][0]-self.vertex[2][0])
            elif(self.vertex[1][0] == self.vertex[2][0]):
                I1[0] = self.vertex[1][0]
                I1[1] = self.vertex[0][1] + (self.vertex[1][0]-self.vertex[0][0])*(self.vertex[0][1]-self.vertex[3][1])/(self.vertex[0][0]-self.vertex[3][0])
            else:
                self.c = (self.vertex[3][1]-self.vertex[0][1]) / (self.vertex[3][0]-self.vertex[0][0])
                self.d = - self.c * self.vertex[3][0] + self.vertex[3][1]
                g = (self.vertex[2][1]-self.vertex[1][1]) / (self.vertex[2][0]-self.vertex[1][0])
                h = - g * self.vertex[2][0] + self.vertex[2][1]

                I1[0] = - (self.d - h) / (self.c - g)
                I1[1] = I1[0] * self.c + self.d

            print('Ideal point 1 at (', I1[0], ', ', I1[1], ')')
        else:
            print('Ideal point 1 at inf')
        if(not self.para2):
            self.a = (self.vertex[1][1]-self.vertex[0][1]) / (self.vertex[1][0]-self.vertex[0][0])
            self.b = - self.a * self.vertex[0][0] + self.vertex[0][1]
            e = (self.vertex[2][1]-self.vertex[3][1]) / (self.vertex[2][0]-self.vertex[3][0])
            f = - e * self.vertex[3][0] + self.vertex[3][1]

            I2[0] = - (self.b - f) / (self.a - e)
            I2[1] = I2[0] * self.a + self.b

            print('Ideal point 2 at (', I2[0], ', ', I2[1], ')')
        else:
            print('Ideal point 2 at inf')

        return [I1, I2]

    def transformation(self, x_T, y_T):
        x_E = 0
        y_F = 0
        if(self.I1[0] == x_T):
            x_E = x_T
        elif(self.para1):
            if(self.vertex[0][0] == self.vertex[3][0]):
                x_E = x_T
            else:
                r = - self.c * x_T + y_T
                x_E = - (self.b - r) / (self.a - self.c)
        else:
            g = (self.I1[1] - y_T) / (self.I1[0] - x_T)
            h = - g * x_T + y_T
            x_E = - (self.b - h) / (self.a - g)
        if(self.para2):
            r = - self.a * x_T + y_T
            if(self.vertex[0][0] == self.vertex[3][0]):
                y_F = r
            else:
                x_F = - (self.d - r) / (self.c - self.a)
                y_f = c * x_F + d
        else:
            e = (self.I2[1] - y_T) / (self.I2[0] - x_T)
            f = - e * x_T + y_T
            if(self.vertex[0][0] == self.vertex[3][0]):
                y_F = f
            else:
                x_F = - (self.d - f) / (self.c - e)
                y_F = self.c * x_F + self.d

        print('x_E = ', x_E)
        print('y_F = ', y_F)

        k = ((self.I2[0]-self.vertex[0][0]) * (self.vertex[1][0]-x_E)) / ((self.vertex[1][0]-self.vertex[0][0]) * (self.I2[0] - x_E))
        l = ((self.I1[1]-self.vertex[0][1]) * (self.vertex[3][1]-y_F)) / ((self.vertex[3][1]-self.vertex[0][1]) * (self.I1[1] - y_F))
        
        if((k < 0) or (k > 1) or (l < 0) or (l > 1)):
            return [-1, -1]
        return [(1 - k), (1 - l)]

    def track(self):
        for i in range(len(d)):
            #im = self.getIm()
            #im = im[self.y_start : (self.y_start + self.height), self.x_start : (self.x_start + self.width)]
            #x_ave, y_ave = self.getPoint(im, True)
            print('==================================================')
            print('(x_t, y_T) = ', '(', d[i][0]-self.x_start, ', ', d[i][1]-self.y_start, ')')
            x_ratio, y_ratio = self.transformation(d[i][0]-self.x_start, d[i][1]-self.y_start)

            print('(', x_ratio, ', ', y_ratio, ')')

d = ([
    [1, 1], 
    [100, 0], 
    [100, 100], 
    [2, 100], 
    [50, 50]
    ])

def main():
   trackCam = TrackCam()
   trackCam.track()


if(__name__ == '__main__'):
   main()
