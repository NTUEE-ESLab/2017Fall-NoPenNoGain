
from time import sleep
from picamera import PiCamera

camera = PiCamera()
camera.start_preview()

sleep(2)

camera.capture('foo.jpg')
