# NoPenNoGain
This is a project demonstrates how to use a laser pointer to control the cursor on the projection screen.

## Introduction
![scenario](https://user-images.githubusercontent.com/30167968/34891826-f82e2282-f811-11e7-87b4-62c12ccd8726.png)
### Scenario
Most of the time, like in a class or meeting, the most comment and convenient way to present is to project the screen of the speecher's computer(or some other devices like iPad) to a projection screen and using a mouse or touching on the host computer(or a pad) to control the cursor. Often, there's a need for writing on the screen. An obvious limitation for presenting while writing on the screen is that you need to stay in front of the computer's screen. Now, imagine that you can just use a laser pointer, walk around the stage, present with gestures, and control the cursor with the pointer right in your hand! And that what NoPenNoGain want to do for you.
## Requirements
### Hardware
* A Windows device.
* Two [***Raspberry Pi computer***](https://www.raspberrypi.org/documentation/setup/) : 
    * one with [***RPI camera module installed***](https://www.raspberrypi.org/documentation/usage/camera/),
    * the other with a two switches connecting on pin 11 and 13.
### Software
* A compiled ***OpenCV*** module on the Pi with camera.
* Both Pis need to install ***nodejs***.
* The Windows device need to be able to run ***nodejs*** files.
## Description
The main part of this project is an Raspberry Pi with a camera, detecting the light point in the screen, and returning the position to the host computer. There's another Raspberry Pi with two button binded with the laser pointer. The second Raspberry Pi return the right or left click of a mouse to the fisrt Pi for it to pack the position and click information into a package to transfer to the host computer. All the communications mentioned aboved are Bluetooth Low Energy(ble).
There are three seperated parts in the whole project. One in ```/camera```, another in ```/pen```, the other in ```/toobar```.
### /camera
This part is where the image detection taking place. Once executed, the Pi will first connect to the host computer and the pen(the second Pi), and request the user to manually configure the four vertexes of the screen starting from up-left side counter-clockwisely. After configuration, the cursor controll starts.
The ```/camera/main.py``` has three threads: one is the main thread, which handles the establishment of the ble connection, initialization of the image processor, and an infinite while loop for detecting and computing the right position of the light point; the other two threads deal with the read/write operations to pen/computer respectively.
### /pen
The part use ***nodejs*** for higher speed, using [***rpi-gpio-buttons***](https://www.npmjs.com/package/rpi-gpio-buttons) to detect button switch and [***bleno***](https://github.com/sandeepmistry/bleno) to implement ble.
### /host-computer
There are two separated sub-parts for host computer. One is a Nodejs file for receiving cursor event using ble and implement the mouse moving and click. The other is a GUI writing in ***QT***
## Installation and how to run
### /camera
On the Pi with camera, 
* Compile and install OpenCV on your Pi.(https://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/)
* Check your camera on Pi is in operation.(https://www.raspberrypi.org/documentation/usage/camera/)
* Place this folder in the working space of your Pi with camera, and execute

      python3 main.py

### /pen
On the second Pi, 
* Set up the two switches.(link: https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/robot/buttons_and_switches/)
* Install nodejs in your second Pi.(link: https://nodejs.org/en/download/package-manager/)
* git clone [***bleno***](https://github.com/sandeepmistry/bleno) and replace all the files in ```bleno/examples/echo/``` with ```NoPenNoGain/pen/main.js```.
* Implement 
     
      sudo node main

### /host-computer
On the Windows device(host computer),
* Dowload the whole folder ```/host-computer/toolbar```, and implement the .exe
* Place the nodejs project in your Visual Studio to run the ble and mouse control.
 
## 
