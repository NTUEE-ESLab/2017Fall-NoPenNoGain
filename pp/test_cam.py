
from time import sleep
from picamera import PiCamera
from picamera.array import PiRGBArray

camera = PiCamera()
rawCapture = PiRGBArray(camera)
camera.start_preview()

sleep(2)

camera.capture('foo.jpg')
