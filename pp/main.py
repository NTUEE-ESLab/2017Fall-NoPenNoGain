import numpy as np
import time
import cv2

class TrackCam:
	def __init__(self):
		self.cap = cv2.VideoCapture(0)
		self.lowRed = np.array([80, 80, 60])
		self.uppRed = np.array([100, 255, 255])
		self.scene = self.setScene()
		self.vertex = self.setVertex()

	def getPoint(self):
		# Get the masked image
		im = self.cap.read()
		im_inv = cv2.bitwise_not(sceneIm)
		im_hsv = cv2.cvtColor(im_inv, cv2.COLOR_BGR2HSV)
		im_mask = cv2.inRange(im_hsv, self.lowRed, self.uppRed)

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
		im = self.cap.read()
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